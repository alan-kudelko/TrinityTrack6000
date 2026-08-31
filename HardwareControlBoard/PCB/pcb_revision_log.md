# PCB Revision Log

This document tracks the evolution of the HardwareControlBoard throughout the development process.

---

# RevA — Initial Prototype

> 🟡 **Status:** Requirements Definition • **Current Revision**

## Objectives

- Validate the first HardwareControlBoard revision.
- Verify MCU power-up sequence.
- Validate communication with the MainControlBoard.
- Validate motor and servo interfaces.
- Bring up the bare-metal firmware on custom hardware.
- Verify power electronics interfaces.

---

## Requirements

### Mechanical

- PCB dimensions: 100 × 100/150/200 mm
- 4-layer PCB stackup
- Standard SMD assembly
- High-current routing support

### Electrical

- 3.3 V logic domain
- Continuous GND plane
- Dedicated analog and digital filtering
- Local decoupling for all ICs
- Hardware fault inputs
- Hardware ENABLE input
- External watchdog support

### Motion Control

- DC motor interfaces
- Servo outputs
- Future stepper motor support
- Incremental encoder inputs
- Current sensing
- Temperature monitoring

### Communication

- SPI interface to MainControlBoard
- I²C diagnostics interface
- UART debug interface

### Debug

- Cortex-M Debug connector
- Full ETM / Trace support
- Test points
- Diagnostic LEDs

---

## Design Rationale

### System Architecture

- Dedicated low-level motion controller
- Bare-metal firmware
- Hardware-assisted peripherals (DMA, timers, interrupts)
- Separation between high-level coordination and deterministic control

### PCB Layout

- Functional block separation
- Continuous ground plane
- Short routing of timing-critical signals
- Separation between logic and power electronics
- Modular board architecture
- DC-link capacitor placement: MLCC placed close to the switching MOSFETs (fast-transient response), electrolytics placed farther out via short, wide stub branches off the main current-return polygon (bulk reservoir role) — not equidistant from the switching node, since the two cap types serve different frequency ranges
- Current-return polygon reshaping: shorten path *length* (current-flow direction), not *width* (cross-section), for space savings — a length cut lowers resistance/inductance as a free side benefit, while a width cut needs a full ampacity check. Validated on HPS (shunt repositioned into an existing narrower polygon step, ~11mm height + ~13mm length reclaimed on that section without touching current-carrying cross-section)
- Shunt/busbar co-location: shunt stacked directly above its RedCube GND return terminal minimizes both trace length and Kelvin-sense loop area simultaneously
- HPS_A/B vs HPS_C/D floor plan: attempted separating the two HPS bridges with MPS Winch as a low-heat spacer (thermal isolation per ThermalBudget.md §6) plus relocating Steppers away from the gap; abandoned — routing corridor couldn't accommodate the signal count (22 Steppers + 30 remaining power-section signals) through the compacted layout. Current plan: accept HPS_A/B and HPS_C/D adjacent, mitigate the resulting thermal concentration with increased via density and larger connecting copper polygons to reduce spreading resistance into the rest of the board (not yet executed)
- MPS ring terminal reselected: Panduit EV14-6RB-Q (rated 16-14 AWG) was mismatched against the actual 18 AWG MPS wire gauge — replaced with Panduit PMV1-3RB-CY (M3 stud, properly 18 AWG rated). Deliberately not stud-matched to HPS's #6/M3.5 — sized for its own lighter duty instead
- HPS ring terminal (Molex 19071-0176) has an unresolved RedCube busbar clearance question — 3D fit-check showed ~1.5mm clearance on the un-squashed library model (comfortable margin at ≤48V, since required creepage is well under 1mm); a slimmer-sleeve alternative (Panduit, not yet found on Mouser) is also being evaluated

### Component Selection

- Gate_Supply (LM5164 auxiliary 12V buck) reworked from its original 68µH/300kHz design (a direct carryover of TI's reference-design current target, IOUT=1A from datasheet Fig 7-1) to **33µH/600kHz**. Root cause: the board's actual Gate_Supply load is only ~35-40mA average (6× UCC21551 VDDx overhead + HPS/Winch/Heater gate-charge delivery) — the inherited 1A target had no relationship to real load and was driving the inductor's physical size unnecessarily. Corrected design current target: 200mA (~5x margin). Saturation current requirement (>1.75A) does not scale down with this correction — it's set by the LM5164's fixed peak current-limit spec (protects against fault conditions, not normal operation), so it remained the binding constraint on inductor selection even after the current-rating fix. Final inductor: Bourns 652-SRN6045TA-330M (33µH, 145mΩ DCR, Isat 2.5A/43% margin, SRF 13MHz, ±20% tolerance) — chosen over a much smaller alternative considered for the 300kHz/68µH case (Würth WE-TPC 3816, 3.8×1.6mm footprint) because that part's Isat margin (1.8A, only 3% over the 1.75A floor) and tolerance (±30%) were both weaker; the safety-critical current-limit margin took priority over footprint size. RRON and the Type-3 ripple network (RA) were recalculated for the new frequency (100kΩ→50.6kΩ, 453kΩ→220kΩ respectively); see `docs/GateSupplyCalculations.md` for the full derivation. One gap knowingly accepted rather than fixed: FB ripple at minimum VIN (15V, the edge of this regulator's operating range) is ~5.3mV, below TI's 12mV recommended floor — present in both the old and new design, and fixing it would cost roughly double the DC regulation error at nominal 48V for a rail that only feeds voltage-tolerant gate driver ICs, so it wasn't judged worth the trade.
- DC-link electrolytic capacitors (KEMET/Yageo A7C0 series) were found undersized on ripple *current* rating, not ripple voltage — the original count passed the ΔV target comfortably while individual capacitors were carrying up to ~3.8x their rated ripple current (see `DCLinkCalculations.md` §3.6 for the general check now added to the methodology). Standardized on a single part across HPS, Winch, and Heater — KEMET `A781MN686M1JLAS035` (68µF/63V, 5.9A rated ripple, 135°C max) — to avoid carrying multiple near-identical capacitor variants in the BOM: 4 units on HPS (~39% ripple current margin, 2.41% ripple voltage), 1 unit each on Winch/Heater (~32% margin, 2.54% ripple voltage). Standardized at 63V across all BOM voltage tiers rather than per-tier, since the 30% margin rule already requires ≥62.4V for the 48V tier
- HPS MOSFET reselected: STL160N4F7 (2.5mΩ RDS(on)) → Infineon ISC011N04NM7V (OptiMOS 7, 1.1mΩ RDS(on)), cutting HPS conduction+switching dissipation from 14.07W to 6.337W (~55%, reflected in `ThermalBudget.md` §4.3/§5/§6). Two other candidates rejected on the RDS(on)×Qg figure-of-merit check (Toshiba TPM1R006PL — clear FOM regression; Infineon BSC010N04LS6/OptiMOS 6 — FOM roughly a wash). Package footprint confirmed against the actual datasheet outline (also modestly smaller than STL160N4F7, not just equal); layout being reworked around the smaller part

### Reliability

- Hardware fault detection
- Hardware ENABLE / KILL support
- Deterministic execution

---

## Known Issues

- Board-half thermal asymmetry was a live concern while HPS_A/B and HPS_C/D were planned adjacent at 14.07W dissipation; substantially less severe now that the MOSFET reselection dropped HPS to 6.337W, but the via-density/copper-spreading mitigation still hasn't been formally verified
- HPS current-sense (INA190A1QDDFRQ1, U20-U23) REF pin is tied to a net that resolves to the same voltage as VS — per the INA190 datasheet (§7.4.2), this is a documented unidirectional-only special case, not the bidirectional current measurement this design needs. Fix decided (REF3016E, ~1.65V, powered from the precision +3.3REF rail) but not yet implemented in the schematic.

---

## Lessons Learned

- Ripple *voltage* passing the target does not mean a DC-link capacitor bank is safe — each capacitor's individual ripple *current* must be checked against its own datasheet rating separately; the two can diverge badly (caught late in this revision on the HPS A7C0 bank)
- RDS(on)×Qg is a real figure of merit, roughly constant within a given silicon process/generation — a lower-RDS(on) MOSFET candidate should be checked on this product, not just on headline RDS(on), and raw datasheet switching-time (ns) comparisons are unreliable across parts tested at different external gate resistances
- Connector wire-gauge rating must be cross-checked explicitly against the actual wire gauge in use, not assumed to still match once either one changes independently — a documented "16-14 AWG" connector silently ended up serving 18 AWG wire because the two facts were established in separate decisions and never compared side by side
- Current-return polygon reshaping: cutting path *length* is close to a free win (space + lower resistance/inductance); cutting path *width* needs a full ampacity check, since it reduces the current-carrying cross-section
- No formal via-ampacity table exists yet for this project; working estimate used so far is ~1–1.5A per via (IPC-2221-style) for the standard/power via class

---

# RevB

> 🔄 **Status:** Planned

To be defined after validation of RevA.
