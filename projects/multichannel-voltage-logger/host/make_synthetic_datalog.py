"""Generate ``sample-data/datalog_synthetic.txt``: a synthetic log in the logger's format.

All values are synthetic (seeded), quantised exactly as the sketch does:
``round(analogRead) x full_scale / 1023``, printed with two decimals. Scenario
(1 Hz, 10 minutes):

* A0: a battery-like rail sagging from 7.4 V to 6.9 V under load
* A1: a regulated 5.9 V rail with ripple and one brief dropout
* A2: a slow ramp that saturates at full scale (to exercise clipping detection)
* A3: a 3.3 V rail with noise
* A4: a slow sine (sensor-like signal) around 2.5 V

The last line is deliberately truncated, as after a power loss mid-write.

Run: ``python make_synthetic_datalog.py``
"""
from pathlib import Path

import numpy as np

FULL_SCALE_V = np.array([7.86, 6.06, 5.97, 5.0, 5.0])
OUT = Path(__file__).resolve().parent / "sample-data" / "datalog_synthetic.txt"


def build(n: int = 600, seed: int = 7) -> np.ndarray:
    rng = np.random.default_rng(seed)
    t = np.arange(n, dtype=float)
    a0 = 7.4 - 0.5 * t / n + rng.normal(0, 0.02, n)
    a1 = 5.9 + 0.03 * np.sin(2 * np.pi * t / 7) + rng.normal(0, 0.01, n)
    a1[300:305] = 0.4                                   # brief dropout
    a2 = np.clip(4.0 + 3.0 * t / n, 0, None) + rng.normal(0, 0.02, n)   # ramps past full scale
    a3 = 3.3 + rng.normal(0, 0.02, n)
    a4 = 2.5 + 1.2 * np.sin(2 * np.pi * t / 120) + rng.normal(0, 0.02, n)
    volts = np.column_stack([a0, a1, a2, a3, a4])
    raw = np.clip(np.rint(volts / FULL_SCALE_V * 1023), 0, 1023)    # 10-bit ADC, saturates at 1023
    return raw * FULL_SCALE_V / 1023


def main() -> None:
    volts = build()
    lines = [",".join(f"{v:.2f}" for v in row) for row in volts]
    lines[-1] = lines[-1][: len(lines[-1]) // 2]                  # truncated final line
    OUT.parent.mkdir(exist_ok=True)
    with open(OUT, "w", encoding="ascii", newline="\n") as f:
        f.write("\n".join(lines) + "\n")
    print(f"wrote {OUT} ({len(lines)} lines)")


if __name__ == "__main__":
    main()
