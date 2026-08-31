# DC Link Calculations

## 1. Purpose

This document records DC-link capacitor sizing calculations for the `HardwareControlBoard` — ripple current/voltage analysis and final component selection per switching node — along with the methodology used, so results can be reproduced and extended to additional nodes.

## 2. Scope

This document covers:

- Ripple current and ripple voltage methodology for DC-link capacitor sizing
- Per-switching-node worked calculations and final component selection
- Capacitor ESR/impedance data used in those calculations

Component derating rules (including the voltage margin used here) are defined in [`docs/ElectricalRequirements.md`](ElectricalRequirements.md) §3.5. Layer stack, trace, and via specifications are in [`docs/PCBLayoutNotes.md`](PCBLayoutNotes.md). This document is a reference/working record, not a set of normative requirements.

## 3. Methodology

### 3.1 Ripple current (RMS)

For a full H-bridge driving a DC load, the RMS ripple current the DC-link capacitor bank must absorb, as a function of duty cycle D:

```
I_ripple(RMS) = I_load × √(D × (1-D))
```

This is maximized at D=0.5 (worst case): `I_ripple(RMS) = 0.5 × I_load`.

### 3.2 Target ripple voltage

Ripple voltage target is a design choice, not a fixed physical constraint. Default working target: **5% of the lowest nominal voltage tier the rail operates at**, unless stated otherwise for a specific node.

### 3.3 Capacitor impedance at switching frequency

Capacitors are combined as complex impedances at the switching frequency, not by simple resistor-style parallel combination of ESR alone — reactance is not always negligible relative to ESR, especially for smaller capacitance banks, and must be checked per case rather than assumed.

```
Z = ESR - j·Xc,  where Xc = 1/(2π·Fsw·C)
```

For a bank of N identical capacitors in parallel: `ESR_bank = ESR/N`, `Xc_bank = Xc/N` (i.e. C_bank = N×C).

Multiple different capacitor banks (e.g. electrolytic + ceramic) are combined via admittance summation:

```
Y_total = Y_1 + Y_2 = 1/Z_1 + 1/Z_2
Z_total = 1/Y_total
ΔV = I_ripple(RMS) × |Z_total|
```

### 3.4 Ceramic (MLCC) DC-bias derating

Ceramic (X7R) capacitance is derated for DC bias per the voltage selected for the BOM variant. Working assumption for this project: **80% of nominal capacitance retained** (X7R parts selected specifically for higher voltage rating than nominal exposure to keep this derating mild — see §3.5 of `ElectricalRequirements.md`).

### 3.5 Electrolytic/hybrid-polymer ESR

Use published ESR at (or near) the switching frequency of interest where available. Low-frequency tan δ (e.g. at 120Hz) does **not** reliably predict high-frequency ESR — hybrid aluminum polymer and standard wet electrolytic capacitors can share similar tan δ while differing in ESR at 40-100kHz by more than an order of magnitude. Confirm construction type (standard electrolytic vs. hybrid polymer) and published ESR at the actual frequency of interest before relying on a derived estimate.

### 3.6 Per-capacitor ripple current rating

Passing the ripple *voltage* target (§3.2) does not by itself confirm a capacitor selection is valid — each electrolytic/hybrid-polymer capacitor also has its own datasheet-published maximum ripple *current* rating, and this must be checked independently. The ripple current a parallel electrolytic bank carries is not simply I_ripple(RMS) divided evenly by capacitor count: within a bank of banks (e.g. electrolytic bank in parallel with a ceramic bank), current splits between branches according to each branch's own impedance, via the current-divider relationship:

```
I_branch = I_ripple(RMS) × |Z_other_branches| / |Z_branch + Z_other_branches|   (complex division)
```

The electrolytic branch's own bank current is then divided evenly across its N identical capacitors. This per-capacitor figure must be checked against the datasheet's rated ripple current at (or near) the switching frequency of interest, including a safety margin (this project targets ≥20-25%) — not just against the resulting ripple voltage.

## 4. Worked Calculations

### 4.1 HPS (High Power Section, 40A per bridge, 40kHz)

**Inputs:**
- I_load = 40A (worst case, per bridge)
- Fsw = 40kHz
- Duty cycle: D=0.5 (worst case) → I_ripple(RMS) = 20A
- Target ripple: 5% of 12V (lowest voltage tier) = 0.6V

**Selected components:**
- Electrolytic: 4× KEMET `A781MN686M1JLAS035`, 68µF/63V, ESR=35mΩ @100kHz, hybrid aluminum polymer, rated ripple current 5.9A @100kHz, 10mm dia × 10.2mm, 135°C max
- Ceramic: 6× 10µF MLCC (KGM series, 1210, X7R), 80% derated → 48µF effective

**Result:**
```
Electrolytic bank: ESR_bank≈0.00875Ω, Xc_bank≈0.01463Ω, |Z_elec|≈0.01705Ω
Ceramic bank: Xc_ceramic≈0.08290Ω (ESR negligible at 40kHz — see §3.3)
Combined: Z_total≈0.01444Ω
ΔV = 20A × 0.01444Ω ≈ 0.289V → 2.41% ripple at 12V
```

Passes the 5% ripple voltage target with comfortable margin.

**Per-capacitor ripple current check (§3.6):**
```
Electrolytic bank current (current-divider vs. ceramic branch) → per-capacitor: ≈4.23A
Rated (A781MN686M1JLAS035): 5.9A @100kHz
Margin: (5.9 − 4.23) / 4.23 ≈ 39%
```

Clears the ≥20-25% target margin with room to spare. Standardized on a 63V rating (not BOM-tier-dependent) since the 30% voltage margin rule (`ElectricalRequirements.md` §3.5) already requires ≥62.4V for the 48V-tier BOM variant (48V × 1.30) — a 50V-rated part was never valid for that tier, so one part across all tiers avoids per-variant capacitor swaps.

### 4.2 Winch (10A)

Same methodology as HPS §4.1, evaluated at the lower current. Winch is a full H-bridge, so the same rectangular-current-pulse ripple model (worst case D=0.5) applies directly.

**Inputs:**
- I_load = 10A (worst case)
- Fsw = 40kHz
- Duty cycle: D=0.5 (worst case) → I_ripple(RMS) = 5A
- Target ripple: 5% of 12V (lowest voltage tier) = 0.6V

**Selected components:**
- Electrolytic: 1× KEMET `A781MN686M1JLAS035`, 68µF/63V, ESR=35mΩ @100kHz, rated ripple current 5.9A — same part used for HPS (§4.1), standardized to keep the BOM from carrying multiple near-identical capacitor variants
- Ceramic: 1× 10µF MLCC (KGM series, 1210, X7R), 80% derated → 8µF effective — HF-transient coverage, not sized against the ripple target (see §3.3/§4.1)

**Result:**
```
Electrolytic: ESR≈0.035Ω, Xc≈0.0585Ω, |Z_elec|≈0.0682Ω
Ceramic bank: Xc_ceramic≈0.4974Ω (ESR negligible)
Combined: Z_total≈0.0609Ω
ΔV = 5A × 0.0609Ω ≈ 0.304V → 2.54% ripple at 12V
```

Passes the 5% ripple voltage target with real margin (~49% relative).

**Per-capacitor ripple current check (§3.6):**
```
Electrolytic bank current (N=1, so bank current = per-capacitor current) ≈ 4.47A
Rated (A781MN686M1JLAS035): 5.9A @100kHz
Margin: (5.9 − 4.47) / 4.47 ≈ 32%
```

Clears the ≥20-25% target margin — 1 unit is sufficient at this current level with the higher-ripple-rated part, same count as the original leaner plan.

### 4.3 Heater (5-10A, worst case 10A)

Heater is a single low-side chopper, not a full bridge — but the bus-side ripple current model (rectangular current pulse, worst case D=0.5) is the same statistically, so the methodology and result are identical to §4.2.

**Inputs:**
- I_load = 10A (worst case)
- Fsw = 40kHz
- Duty cycle: D=0.5 (worst case) → I_ripple(RMS) = 5A
- Target ripple: 5% of 12V (lowest voltage tier) = 0.6V

**Selected components:**
- Electrolytic: 1× KEMET `A781MN686M1JLAS035`, 68µF/63V, ESR=35mΩ @100kHz, rated ripple current 5.9A — same part used for HPS (§4.1), standardized to keep the BOM from carrying multiple near-identical capacitor variants
- Ceramic: 1× 10µF MLCC (KGM series, 1210, X7R), 80% derated → 8µF effective — HF-transient coverage, not sized against the ripple target (see §3.3/§4.1)

**Result:**
```
Electrolytic: ESR≈0.035Ω, Xc≈0.0585Ω, |Z_elec|≈0.0682Ω
Ceramic bank: Xc_ceramic≈0.4974Ω (ESR negligible)
Combined: Z_total≈0.0609Ω
ΔV = 5A × 0.0609Ω ≈ 0.304V → 2.54% ripple at 12V
```

Passes the 5% ripple voltage target with real margin (~49% relative).

**Per-capacitor ripple current check (§3.6):**
```
Electrolytic bank current (N=1, so bank current = per-capacitor current) ≈ 4.47A
Rated (A781MN686M1JLAS035): 5.9A @100kHz
Margin: (5.9 − 4.47) / 4.47 ≈ 32%
```

Clears the ≥20-25% target margin — 1 unit is sufficient at this current level with the higher-ripple-rated part, same count as the original leaner plan.

### 4.4 MPS Steppers

No custom DC-link calculation performed. The stepper driver section (DRV8434S ×6) follows TI's recommended external component values from the datasheet application circuit directly (VCP=220nF, CPH-CPL≈22nF, VM/PGND bulk + bypass capacitors, DVDD bypass=470nF — confirmed during schematic review to match TI's reference design). Treated as resolved by reference-design compliance rather than an open item.

### 4.5 LPS (Low Power Section, 4 channels active simultaneously, worst case)

**Inputs:**
- I_load = 4A (4 of 8 channels active simultaneously, 1A each, worst-case perfectly correlated switching)
- Fsw = 40kHz
- Duty cycle: D=0.5 (worst case) → I_ripple(RMS) = 2A
- Target ripple: 5% of 12V (lowest voltage tier) = 0.6V

No electrolytic bank — current is low enough that ceramic alone suffices.

**Selected components:**
- Ceramic: 4× 10µF MLCC (KGM series, 1210, X7R), 80% derated → 32µF effective — count set by layout (symmetrical, repeatable placement) and trace length to the section rather than the ripple target alone

**Result:**
```
Xc_ceramic ≈ 0.1243Ω (ESR negligible)
ΔV = 2A × 0.1243Ω ≈ 0.249V → 2.07% ripple at 12V
```

Passes the 5% target with comfortable margin.

