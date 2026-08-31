# Gate Supply Calculations

## 1. Purpose

This document records calculations for the `HardwareControlBoard` Gate_Supply rail — switching frequency, output voltage, UVLO, inductor selection, and related derivations — verified against the LM5164 datasheet formulas.

## 2. Scope

This document covers the LM5164-based Gate_Supply auxiliary 12V rail. DC-link capacitor sizing is covered separately in [`docs/DCLinkCalculations.md`](DCLinkCalculations.md). Component derating rules are in [`docs/ElectricalRequirements.md`](ElectricalRequirements.md) §3.5. This document is a reference/working record, not a set of normative requirements.

## 3. Gate_Supply (LM5164DDAR, Auxiliary 12V Supply)

Reference: TI LM5164 datasheet (SNVSAU4D).

### 3.1 Switching frequency

```
FSW[kHz] = VOUT[V] × 2500 / RRON[kΩ]     (Eq. 1)
```

RRON = R6 = 50.6kΩ, VOUT = 12.15V:
```
FSW = 12.15 × 2500 / 50.6 ≈ 600 kHz
```
Matches the schematic's stated target (600kHz).

### 3.2 Output voltage

```
VOUT = VREF × (1 + RFB1/RFB2),  VREF = 1.2V
```

RFB1 = R2 = 511kΩ, RFB2 = R7 = 56kΩ:
```
VOUT = 1.2 × (1 + 511/56) = 12.15V
```
Matches the schematic's stated output (12.15V).

### 3.3 UVLO (EN/UVLO divider) — sized against the gate voltage ceiling

**Design policy:** in direct mode (SB1 selecting VDD_IN as Gate_Supply), gate voltage = VDD_IN directly, unregulated. With VGS abs-max = 20V on the populated MOSFETs (STL160N4F7, PJA3406), the direct-mode ceiling is set at **15V** (5V / 25% margin for switching-induced ringing/overshoot). Above 15V, regulated mode (12.15V output, §3.2) must be active — which requires the LM5164 to already be regulating *before* VDD_IN reaches 15V, or there is a dead zone where direct mode is unsafe but regulated mode isn't yet running.

```
VIN(on)  = 1.5V × (1 + RUV1/RUV2)     (Eq. 13)
VIN(off) = 1.4V × (1 + RUV1/RUV2)     (Eq. 14)
```

Target: VIN(on) ≈14V, giving 1V of margin below the 15V ceiling (regulation active with margin, not right at the boundary). Solving for RUV1 with RUV2=75kΩ fixed gives 625kΩ (non-standard); nearest E96 value:

RUV1 = R4 = **619kΩ** (E96), RUV2 = R8 = 75kΩ (bottom, to GND):
```
VIN(on)  = 1.5 × (1 + 619/75) = 13.88V
VIN(off) = 1.4 × (1 + 619/75) = 12.96V
```

With R4=619kΩ, regulated mode is active from ~13.9V onward, comfortably before the 15V direct-mode ceiling. Both MOSFETs (STL160N4F7, PJA3406) are characterized/rated at VGS=10V with RDS(on) already near-saturated at that point, so the full 12.15V-14V+ operating range stays well within the region of good gate drive performance, not just abs-max safety.

### 3.4 Buck inductor selection (L1)

**Gate_Supply load:** VDD rail feeding gate-charge delivery to all H-bridge switches plus the six UCC21551 drivers' own VDDx overhead. Average load ≈35-40mA. Design current target: **200mA** (≈5x margin over the measured average, covering BOM/transistor variants with higher gate charge).

**Saturation current requirement** — per §7.2.2.3, Isat must stay "well above the current limit setting of the LM5164," which is a fixed device spec (IPEAK1/IPEAK2 = 1.25-1.75A min-max, §5.5), independent of application current. This protects against fault conditions (output short, startup inrush), not normal operation.

**Selected: Bourns 652-SRN6045TA-330M, 33µH**
- DCR: 145mΩ
- Isat: 2.5A (43% margin over the 1.75A floor)
- SRF: 13MHz (clears the ~6MHz floor at 600kHz, 10x rule of thumb)
- Tolerance: ±20%

Confirmed via the manufacturer's DC-current-vs-temperature-rise and inductance-vs-current curves that at the real operating current (200mA design target, worst-case IL,peak≈484mA including -20% tolerance — see §3.5) the part sits on the flat, unstressed region of both curves, nowhere near either knee.

### 3.5 Inductor ripple current and peak current

```
ΔIL = VOUT/(FSW×LO) × (1 - VOUT/VIN_nom)     (Eq. 18)
IL(peak) = IOUT(max) + ΔIL/2                  (Eq. 19)
```

With VOUT=12.15V, FSW=600kHz, LO=33µH, VIN_nom=48V, IOUT(design)=200mA:
```
ΔIL = 12.15/(600e3×33e-6) × (1-12.15/48) ≈ 458 mA
IL(peak) = 200mA + 458mA/2 ≈ 429 mA
```

Comfortably below IPEAK1 min (1.25A). Note ΔIL (458mA) exceeds the 200mA design current itself — per §6.4.2, this means the converter operates in diode-emulation/pulse-skipping (DEM) mode almost continuously at real load, not sustained CCM. This is consistent with normal LM5164 operation at light load (ultra-low IQ sleep mode).

At the -20% inductance tolerance corner (26.4µH): ΔIL≈568mA, IL(peak)≈484mA — still 61% below the 1.25A current-limit floor.

### 3.6 Output capacitor (COUT = C18)

```
COUT = ΔIL / (8 × FSW × VOUT(ripple))     (Eq. 21)
```

Targeting 0.5% ripple (60.75mV): COUT ≥ 458mA/(8×600kHz×60.75mV) ≈ 1.57µF. C18=22µF, ~14x margin over this minimum.

### 3.7 Input capacitor (CIN = C19+C20)

TI's recommendation (§7.2.2.5) is 2.2µF or higher for high-frequency input decoupling. C19+C20 = 4.4µF total, 2x the recommended floor.

### 3.8 Type-3 ripple injection network (RA = R3, CA = C16, CB = C17)

```
CA ≥ 10 / (FSW × (RFB1‖RFB2))                              (Eq. 24)
RA×CA ≤ tON(nom) × (VIN(nom) − VOUT) / 20mV                 (Eq. 25)
tON(nom) = RRON / (VIN(nom) × 2.5)                           (Eq. 11)
```

RFB1‖RFB2 ≈ 50.5kΩ. CA minimum ≈ 330pF. C16=3.3nF, ~10x margin.

tON(nom) = 50.6kΩ/(48V×2.5) ≈ 422ns.

Solving for RA at the 20mV target ripple:
```
RA ≈ [422ns × (48−12.15)] / (20mV × 3.3nF) ≈ 229 kΩ
```

R3 = **220kΩ** (comfortably under the ceiling), giving ~20.8mV ripple at VIN(nom)=48V.

**Ripple at minimum VIN:** at VIN=15V (the boundary of this regulator's operating range — below 15V, VDD_IN drives the gate rail directly via SB1, bypassing the LM5164), ripple with R3=220kΩ works out to ~5.3mV, below TI's recommended 12mV floor for reliable low-VIN COT operation (§7.2.2.6). Closing this gap would require shrinking RA×CA by ~2.3x, which would raise ripple (and per §7.2.2.6, DC regulation error) at VIN(nom)=48V to roughly double the current ~20.8mV. Since VOUT only feeds gate driver supply rails (UCC21551, 2EDN7533) that tolerate a real voltage range, and 15V is an edge condition rather than the design center, this is accepted as-is.
