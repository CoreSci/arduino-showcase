"""Tests for plot_datalog.py. Run from this folder: ``python -m pytest -q``"""
from pathlib import Path

import numpy as np

import make_synthetic_datalog as gen
import plot_datalog as pdl

SAMPLE = Path(__file__).resolve().parent / "sample-data" / "datalog_synthetic.txt"


def test_parse_valid_and_malformed_lines():
    text = "1.00,2.00,3.00,4.00,5.00\n\n1.5,2.5\nabc,1,2,3,4\n0.10,0.20,0.30,0.40,0.50\n7.1,5.9,5"
    log = pdl.parse(text)
    assert log.n == 2
    assert log.skipped == [3, 4, 6]
    np.testing.assert_allclose(log.volts[1], [0.1, 0.2, 0.3, 0.4, 0.5])


def test_empty_input():
    log = pdl.parse("")
    assert log.n == 0 and log.volts.shape == (0, 5)


def test_summary_and_clipping():
    log = pdl.parse("7.86,1,1,1,1\n7.00,2,2,2,2\n")
    rows = {r["channel"]: r for r in pdl.summary(log)}
    assert rows["A0"]["clipped"] == 1 and rows["A1"]["clipped"] == 0
    assert rows["A1"]["mean"] == 1.5


def test_full_scale_matches_sketch():
    sketch = (Path(__file__).resolve().parents[1] / "multichannel-voltage-logger.ino").read_text()
    assert "{7.86, 6.06, 5.97, 5.0, 5.0}" in sketch
    assert pdl.FULL_SCALE_V == (7.86, 6.06, 5.97, 5.0, 5.0)


def test_generator_matches_committed_sample():
    volts = gen.build()
    assert volts.shape == (600, 5)
    assert np.all(volts <= gen.FULL_SCALE_V + 1e-9)          # ADC saturates at full scale
    log = pdl.parse(SAMPLE.read_text())
    assert log.n == 599 and log.skipped == [600]              # truncated last line is skipped
    np.testing.assert_allclose(log.volts, volts[:599], rtol=0, atol=0.005 + 1e-9)   # 2-decimal text


def test_sample_exercises_clipping_and_dropout():
    rows = {r["channel"]: r for r in pdl.summary(pdl.parse(SAMPLE.read_text()))}
    assert rows["A2"]["clipped"] > 50                          # ramp saturates
    assert rows["A1"]["min"] < 1.0                             # dropout visible
    assert rows["A3"]["clipped"] == 0


def test_cli_end_to_end(tmp_path, capsys):
    out = tmp_path / "log.png"
    assert pdl.main([str(SAMPLE), "--rate", "1", "--out", str(out)]) == 0
    assert out.stat().st_size > 5000
    text = capsys.readouterr().out
    assert "599 samples" in text and "1 malformed line(s) skipped" in text
    assert pdl.main([str(tmp_path / "missing.txt")]) == 1
