"""Plot and summarise a ``datalog.txt`` recorded by the multichannel voltage logger.

Problem: the logger writes raw CSV lines to an SD card. This script turns a
copied log into a per-channel voltage chart plus summary statistics, and flags
readings at a channel's full scale (a clipped, out-of-range input).

How it works: reads lines of five comma-separated voltages. Malformed lines,
such as a partial last line after a power cut or card-removal glitches, are
skipped and counted instead of aborting. It prints min/mean/max per channel and
saves a stacked plot, one panel per channel, with its full-scale limit.

Usage::

    python plot_datalog.py DATALOG [--rate HZ] [--out plot.png]

``--rate`` gives the sampling rate to label the x axis in seconds. The logger
writes no timestamps, so without it the x axis is the sample index. Requires
``numpy`` and ``matplotlib``.
"""
from __future__ import annotations

import argparse
import sys
from dataclasses import dataclass
from pathlib import Path

import numpy as np

CHANNELS = ("A0", "A1", "A2", "A3", "A4")
FULL_SCALE_V = (7.86, 6.06, 5.97, 5.0, 5.0)   # must match FULL_SCALE_V in the sketch


@dataclass
class Datalog:
    volts: np.ndarray          # shape (n_samples, 5)
    skipped: list[int]         # 1-based line numbers of malformed lines

    @property
    def n(self) -> int:
        return len(self.volts)


def parse(text: str) -> Datalog:
    """Parse datalog text. Lines that aren't exactly five numbers are skipped."""
    rows, skipped = [], []
    for lineno, line in enumerate(text.splitlines(), 1):
        line = line.strip()
        if not line:
            continue
        parts = line.split(",")
        try:
            values = [float(p) for p in parts]
        except ValueError:
            skipped.append(lineno)
            continue
        if len(values) != len(CHANNELS):
            skipped.append(lineno)
            continue
        rows.append(values)
    volts = np.asarray(rows, dtype=float).reshape(-1, len(CHANNELS))
    return Datalog(volts=volts, skipped=skipped)


def summary(log: Datalog) -> list[dict[str, float | str | int]]:
    """Per-channel min/mean/max and the number of samples at full scale (clipped)."""
    out = []
    for i, name in enumerate(CHANNELS):
        col = log.volts[:, i] if log.n else np.array([])
        clipped = int(np.sum(col >= FULL_SCALE_V[i] - 0.01)) if log.n else 0
        out.append({"channel": name, "min": float(col.min()) if log.n else float("nan"),
                    "mean": float(col.mean()) if log.n else float("nan"),
                    "max": float(col.max()) if log.n else float("nan"), "clipped": clipped})
    return out


def plot(log: Datalog, out: str | Path, rate_hz: float | None = None, title: str = "Voltage log") -> None:
    """Stacked per-channel plot with each channel's full-scale limit."""
    import matplotlib
    matplotlib.use("Agg")
    import matplotlib.pyplot as plt

    t = np.arange(log.n) / rate_hz if rate_hz else np.arange(log.n)
    fig, axes = plt.subplots(len(CHANNELS), 1, sharex=True, figsize=(9, 8))
    for i, ax in enumerate(axes):
        ax.plot(t, log.volts[:, i], lw=1)
        ax.axhline(FULL_SCALE_V[i], color="r", ls="--", lw=0.8, label=f"full scale {FULL_SCALE_V[i]} V")
        ax.set_ylabel(f"{CHANNELS[i]} (V)")
        ax.set_ylim(0, FULL_SCALE_V[i] * 1.05)
        ax.legend(loc="upper right", fontsize=7)
    axes[-1].set_xlabel("time (s)" if rate_hz else "sample index")
    fig.suptitle(title)
    fig.tight_layout()
    fig.savefig(out, dpi=110)
    plt.close(fig)


def main(argv: list[str] | None = None) -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("datalog", help="datalog.txt copied from the SD card")
    ap.add_argument("--rate", type=float, help="sampling rate in Hz (for a time axis)")
    ap.add_argument("--out", default="datalog.png", help="output PNG (default: datalog.png)")
    args = ap.parse_args(argv)

    try:
        log = parse(Path(args.datalog).read_text(encoding="ascii", errors="replace"))
    except OSError as exc:
        print(f"cannot read {args.datalog}: {exc}", file=sys.stderr)
        return 1
    if log.n == 0:
        print("no valid samples found", file=sys.stderr)
        return 1

    print(f"{log.n} samples" + (f", {len(log.skipped)} malformed line(s) skipped: {log.skipped[:10]}" if log.skipped else ""))
    print(f"{'channel':<8}{'min V':>8}{'mean V':>8}{'max V':>8}{'clipped':>9}")
    for row in summary(log):
        print(f"{row['channel']:<8}{row['min']:>8.2f}{row['mean']:>8.2f}{row['max']:>8.2f}{row['clipped']:>9}")
    plot(log, args.out, args.rate, title=f"Voltage log: {Path(args.datalog).name}")
    print(f"saved {args.out}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
