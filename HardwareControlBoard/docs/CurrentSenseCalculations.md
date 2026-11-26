# Current Sense Calculations

## 1. Purpose

This document records the current-sense amplifier (INA296A2IDGKR) configuration — REF pin assignment, gain, output range and scaling — for the `HardwareControlBoard`, verified against its datasheet.

## 2. Scope

This document covers per-channel INA296 REF pin assignment, gain selection and output range. DC-link capacitor sizing is covered separately in [`docs/DCLinkCalculations.md`](DCLinkCalculations.md). Component derating rules are in [`docs/ElectricalRequirements.md`](ElectricalRequirements.md) §3.5. This document is a reference/working record, not a set of normative requirements.

## 3. Current-Sense Amplifier (INA296A2IDGKR)

Reference: TI INA296A/INA296B datasheet (SBOSA04D).

The INA190A1 used earlier was too slow for PWM-synchronous low-side sampling at 40 kHz (about 45 kHz bandwidth, 0.3 V/µs slew rate, about 30 µs settling). The INA296 replaces it on all four channels: 1.1 MHz bandwidth, 8 V/µs slew rate, 1 µs settling to 1 %, offset ±10 µV max, gain error ±0.01 % max (§7.3.1.4).

### 3.1 Gain

All channels use the INA296A2 variant:

```
GAIN = 20 V/V
```

### 3.2 REF pin assignment

```
VOUT = (ILOAD × RSENSE × GAIN) + (VREF1 + VREF2) / 2     (Eq. 1)
```

There is no operational difference between REF1 and REF2 (§7.4.1). The linear output range is GND + 50 mV to VS − 200 mV (datasheet gain-error conditions).

**Bidirectional channels — HPS (×2) and Winch:** these are H-bridge motor drives where current can flow in either direction. REF1 and REF2 are both tied to the shared 1.65 V precision reference (REF3016E, see `PCB/pcb_revision_log.md`), so the output rests at 1.65 V at zero current and moves up or down with the current direction. The datasheet calls this the most accurate way to bias the output to a precise voltage (§7.4.3.1).

**Unidirectional channel — Heater:** a single low-side switch driving a resistive element; current only ever flows one direction. Both REF pins are tied to GND (ground-referenced output, §7.4.2.1), so the output rests at 0 V and rises with load current. The input polarity is positive (IN+ on the FET side of the shunt, IN− on the ground side).

In both cases pin 4 (NC) is connected to GND and each VS pin has a 100 nF bypass capacitor to GND.

### 3.3 Scaling and range

With VS = 3.3 V the linear output range is about 0.05 V to 3.1 V. ADC resolution: 3.3 V / 4096 = 0.8 mV per LSB (12 bit).

| Channel | RSENSE | Sensitivity (GAIN × RSENSE) | Resolution | Linear range | Required peak current |
|---------|--------|-----------------------------|------------|--------------|-----------------------|
| HPS A, HPS B | 1 mΩ | 20 mV/A | about 40 mA/LSB | ±72 A around 1.65 V | ±40 A |
| Winch | 5 mΩ | 100 mV/A | about 8 mA/LSB | ±14.5 A around 1.65 V | ±10 A |
| Heater | 5 mΩ | 100 mV/A | about 8 mA/LSB | 0 to about 31 A | 5–10 A |

The bidirectional range is limited by the upper end: (3.1 V − 1.65 V) / sensitivity = 1.45 V / sensitivity.

The amplifier input offset (±10 µV max) corresponds to ±10 mA on the 1 mΩ shunts and ±2 mA on the 5 mΩ shunts, below one ADC LSB on every channel.

The zero point of the bidirectional channels is a different matter: it is set by the absolute 1.65 V reference, while the ADC scale follows VAREF (the filtered +3.3VA rail, derived from the 3.3 V LDO, ±1.75 % over temperature). The zero-current code can therefore shift by about ±35 LSB (about ±29 mV), which is about ±1.4 A on the HPS channels and about ±0.3 A on the Winch channel. Firmware must measure the zero-current code while the bridge is idle and subtract it. The Heater channel is ground-referenced (zero at 0 V) and has no such shift.

### 3.4 Heater output range check

Using RSENSE = 5 mΩ, GAIN = 20 V/V, REF1 = REF2 = GND and ILOAD = 5–10 A (per `DCLinkCalculations.md` §4.3, Heater 5–10 A range):

```
VOUT(5A)  = 5A  × 5mΩ × 20 = 0.50 V
VOUT(10A) = 10A × 5mΩ × 20 = 1.00 V
```

Both values are well inside the linear range (above GND + 50 mV, below VS − 200 mV) and use about a third of the ADC range with VAREF = 3.3 V.
