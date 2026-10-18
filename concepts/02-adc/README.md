# Concept 2: ADC, reading analog input you can trust

**Level:** intermediate · **Board:** Arduino Uno (Vcc measurement is ATmega328P-specific; the rest works on any Arduino) · **Time:** 45 minutes, plus a multimeter for calibration

## Goal

Understand what `analogRead()` actually returns: resolution, reference voltage and noise. Then turn raw counts into values you can trust by averaging, measuring the real reference, and calibrating against known points.

## Parts

- 1 potentiometer (10 kΩ)
- A multimeter (for calibration)
- Breadboard and jumper wires

## Wiring

```
 5V  ── potentiometer outer leg
 A0  ── potentiometer wiper (middle leg)
 GND ── potentiometer other outer leg
```

Open the Serial Monitor at 9600 baud, or the **Serial Plotter** to watch the columns as lines.

## What the ADC measures

The Uno's analog-to-digital converter compares the input voltage with a **reference voltage** and returns a 10-bit number:

```
            Vin × 1024                       ADC × Vref
   ADC  =  ────────────        so    Vin  =  ────────────
               Vref                             1024

   0 V ──────────────────────────────────────────► Vref
   │ 0 │ 1 │ 2 │ 3 │  ...                  │1022│1023│
    ◄─►
    one step (1 LSB) = Vref / 1024 = 4.9 mV at 5 V
```

- **Resolution** is 10 bits: 1024 steps. At a 5 V reference, one step is about 4.9 mV. A change smaller than that is invisible, and every reading has up to ±½ step of rounding.
- **The reference sets the scale.** A reading of 512 means "half of Vref", *not* "2.5 V". If Vref is really 4.80 V, that 512 is 2.40 V.
- **`/1024` or `/1023`?** The ATmega328P datasheet uses 1024: the top code 1023 means "Vref minus one step". Many examples (including this repo's voltage logger) divide by 1023 so that full scale reads exactly Vref. The difference is 0.1%, well below the reference's own uncertainty. Pick one convention and state it.

| Reference (`analogReference(...)`) | Value | Use when |
|---|---|---|
| `DEFAULT` | the 5 V supply (AVcc) | Measuring something powered from the same 5 V (see *ratiometric* below) |
| `INTERNAL` | ~1.1 V bandgap | Small signals: 1 step = 1.07 mV. Stable, but varies ±10% between chips, so calibrate it |
| `EXTERNAL` | whatever is on the AREF pin | A precision reference chip. Never apply a voltage to AREF while `DEFAULT` or `INTERNAL` is selected |

## Ratiometric vs absolute measurements

This distinction decides whether you need to know Vref at all:

- **Ratiometric:** the sensor is powered from the same supply as the reference, like the potentiometer here or a resistive sensor in a divider. If the supply sags 4%, the sensor's output and the reference sag together, and the **raw count doesn't change**. Use the counts directly (or as a percentage). The `cal` column does this.
- **Absolute:** the voltage doesn't depend on the Arduino's supply, as with a battery, a TMP36 temperature sensor or an external 3.3 V rail. Now a 4% error in Vref becomes a 4% error in the result, and you need the real Vref. Compare the `V@5.00` and `V@Vcc` columns.

## Measuring the real supply (the bandgap trick)

The ATmega328P can route its internal 1.1 V bandgap reference to the ADC input while using the 5 V supply as the reference. Now the ADC measures a *known* voltage against an *unknown* reference, and the equation above solves for Vref:

```
   counts = 1100 mV × 1024 / Vcc      →      Vcc = 1100 mV × 1024 / counts
```

`readVccMillivolts()` does this with three register writes (`ADMUX`, `ADCSRA`). The first `analogRead()` afterwards is thrown away, because the reference needs a moment to settle after switching.

## Noise and averaging

Watch `raw` with the knob untouched: it flickers by a count or two. That's normal. Averaging **N** readings reduces random noise by about √N, so 16 readings give 4× less noise. The `avg` column keeps the fraction (e.g. `511.4`), which carries real information when the input sits between two steps. Averaging can't remove a constant offset or a wrong reference. That's what calibration is for.

The input also needs a **source impedance under ~10 kΩ**. The ADC charges a small internal capacitor for each reading, and a higher-impedance source can't charge it in time, so readings lag or bleed over from the previous channel. A 10 kΩ potentiometer is at the limit, and a 100 nF capacitor from the input to GND fixes it if needed.

## Calibration

### 1. The bandgap (once per board)

1. Measure the **5V** pin with a multimeter: `Vcc_meter` (e.g. 4.93 V).
2. Read the `Vcc(mV)` column: `Vcc_reported` (e.g. 5037).
3. Set `BANDGAP_MV = 1100 × Vcc_meter(mV) / Vcc_reported`. In the example, 1100 × 4930 / 5037 = **1077**. Re-upload. `Vcc(mV)` now matches the meter.

### 2. Two-point calibration (per sensor)

Any linear sensor can be calibrated from two known points: `real = REAL_LO + (raw − RAW_LO) × (REAL_HI − REAL_LO) / (RAW_HI − RAW_LO)`. This corrects both **offset** (the line doesn't pass through zero) and **gain** (the slope is off):

1. Apply a known low input (e.g. a reference at 10 %) and note the `avg` value as `RAW_LO`, with its true value as `REAL_LO`.
2. Repeat near the top of the range for `RAW_HI` and `REAL_HI`.
3. Choose points near the ends of the range you'll actually use. Errors grow outside the two points.

## Try this

1. **See the reference move:** plug the board into a different USB port or a powered hub, and compare `Vcc(mV)` and `V@5.00` vs `V@Vcc` at the same knob position. Which columns change? Which stay put? (Hint: *ratiometric*.)
2. **Resolution in practice:** switch to `analogReference(INTERNAL)` in `setup()`, and add a 10 kΩ + 2.2 kΩ divider so the knob's output stays under 1.1 V. Also remove the `readVccMillivolts()` call from `loop()`: switching references on every pass never gives the 1.1 V reference time to settle. How many millivolts is one step now?
3. **Averaging trade-off:** try `SAMPLES` = 1, 4, 64 in the Serial Plotter. What happens to the noise, and to how fast the value follows the knob?
4. **Calibrate a real sensor:** replace the knob with a light-dependent resistor in a divider, and calibrate `cal` so a covered sensor reads 0 and a phone flashlight reads 100.

## Why this matters

Every measurement an embedded system makes (temperature, pressure, current, battery level, a sensor's analog output) goes through an ADC, and the number it reports is only as good as the chain behind it: reference, resolution, noise, source impedance and calibration. The discipline here is the same one used in any measurement work. Know what you are comparing against, average away random noise, calibrate against known references at points that bracket the working range, and write down the convention you used. The [multichannel voltage logger](../../projects/multichannel-voltage-logger/) in this repo applies the same ideas to five channels.
