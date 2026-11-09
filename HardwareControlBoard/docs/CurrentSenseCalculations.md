# Current Sense Calculations

## 1. Purpose

This document records the current-sense amplifier (INA190A1QDDFRQ1) configuration — REF pin assignment, gain, and zero-current margin — for the `HardwareControlBoard`, verified against its datasheet.

## 2. Scope

This document covers per-channel INA190 REF pin assignment and gain selection. DC-link capacitor sizing is covered separately in [`docs/DCLinkCalculations.md`](DCLinkCalculations.md). Component derating rules are in [`docs/ElectricalRequirements.md`](ElectricalRequirements.md) §3.5. This document is a reference/working record, not a set of normative requirements.

## 3. Current-Sense Amplifier (INA190A1QDDFRQ1)

Reference: TI INA190-Q1 datasheet (SBOS871A).

### 3.1 Gain

All channels use the INA190A1 variant:

```
GAIN = 25 V/V
```

### 3.2 REF pin assignment

```
VOUT = (ILOAD × RSENSE × GAIN) + VREF     (Eq. 1)
```

**Bidirectional channels — HPS (×2) and Winch:** these are H-bridge motor drives where current can flow in either direction. REF is tied to the shared precision reference (§4), giving VREF ≈ VS/2 for equal signal range in both directions (§7.4.3 of the INA190 datasheet).

**Unidirectional channel — Heater:** a single low-side switch driving a resistive element; current only ever flows one direction. REF is tied to GND, per the datasheet's default recommendation for unidirectional applications (§7.4.2: *"for most unidirectional applications the REF pin is simply grounded"*) — this maximizes the usable output swing/resolution for this channel rather than wasting half the range on a direction that can't occur.

### 3.3 Heater zero-current margin check (VZL)

With REF=GND, the output at the low end of the operating current range must clear the device's zero-current output voltage floor, VZL (§6.5: 1-3mV for A1 devices).

Using RSENSE=5mΩ, GAIN=25V/V, ILOAD(min)=5A (per `DCLinkCalculations.md` §4.3, Heater 5-10A range):

```
VOUT(5A) = 5A × 5mΩ × 25 = 625 mV
```

625mV clears the 1-3mV VZL floor by over 200x — no margin concern at this current level.
