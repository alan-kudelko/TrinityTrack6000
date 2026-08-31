# PCB Layout Notes

## 1. Purpose

This document records the PCB layout decisions for the `HardwareControlBoard`, including the layer stack, layer function assignment, trace width classes, clearance rules, and via catalog.

Unless stated otherwise, the values in this document reflect decisions already made for the current hardware revision, informed by manufacturer capabilities (PCBWay) and applicable reference designs.

## 2. Scope

This document covers:

- PCB layer stack (manufacturer, layer count, copper/dielectric build-up)
- Layer function assignment
- Trace width classes
- Clearance rules
- Via catalog (signal, thermal, gate)
- High-current distribution strategy
- Grounding strategy

Schematic design, component selection, and BOM are covered in [`docs/ElectricalRequirements.md`](ElectricalRequirements.md), [`docs/FunctionalRequirements.md`](FunctionalRequirements.md), and [`PCB/`](../PCB/). This document focuses on layout implementation.

## 3. Layer Stack

| No. | Stackup | Thickness | Material |
|-----|---------|-----------|----------|
| L1 | Copper | 1 oz | |
| | PP | 0.17 mm | S1000HB |
| L2 | Copper | 1 oz | |
| | Core | 1.1 mm (includes copper) | S1000H |
| L3 | Copper | 1 oz | |
| | PP | 0.17 mm | S1000HB |
| L4 | Copper | 1 oz | |

## 4. Layer Function Assignment

| Layer | Function |
|-------|----------|
| L1 (Top) | Component placement, power polygons, and signals |
| L2 | Continuous GND plane |
| L3 | Signals and power polygons for current distribution |
| L4 (Bottom) | Mainly power/supply polygons (gate drive supply, logic rails, etc.); signals permitted locally where they materially simplify routing, provided current distribution is not compromised |

## 5. Trace Widths

Applies to local routed traces; power/ground polygons are covered separately (see Section 8).

| Class | Width | Notes |
|-------|-------|-------|
| Signal | 0.200 mm | |
| Analog | 0.3 mm | |
| Power | 0.300 mm | Local connections |
| Gate driver | 0.7 mm | |

Winch and MPS low-side gate drive traces use 0.4mm rather than the 0.7mm Gate driver default — justified by substantially lower gate charge on these switches versus the HPS MOSFET the 0.7mm class was set for (ISC011N04NM7V, 73nC; Winch's PJQ4548P-AU is 13nC), and applied to save space in tightly constrained sections.

## 6. Clearances

| Class | Clearance | Notes |
|-------|-----------|-------|
| Power nets | 0.5 mm (20 mil) | |
| Signal | 0.254 mm (10 mil) | |

## 7. Via Catalog

| Type | Pad / Drill | Notes |
|------|-------------|-------|
| Standard / signal (incl. power current-transfer) | 0.620 mm / 0.310 mm | |
| Thermal (component) | 0.505 mm / 0.200 mm | |
| Gate | 0.700 mm / 0.350 mm | |

## 8. High-Current Distribution

High-current nets are distributed primarily through copper bus bars (Würth Elektronik RedCube, 74650173R), sized 12x3 mm, with PCB copper providing supplemental current spreading and local interconnection, per [`docs/ElectricalRequirements.md`](ElectricalRequirements.md) §4.4.

Lower-current (10A-class) connections — winch and MPS heater/low-side outputs — use direct screw-terminal pads sized for a standard ring terminal on 18 AWG wire. Standard MPS ring terminal: Panduit `PMV1-3RB-CY` (M3 stud, vinyl insulated, 105°C max, ring outer width 5.84 mm, ring hole 3.30 mm). M3 stud is sized specifically for this connector's 18 AWG duty, separate from HPS's #6/M3.5 stud. Pad clearance hole/diameter to be sized to match the M3 stud and 3.30 mm ring hole.

HPS motor output terminals (HPS_A-D) use a dedicated ring-terminal pad: #6 stud (M3.5), 10.5 mm pad diameter (oversized for a washer), sized for Molex `19071-0176` (D-656-06X), 8 AWG max, ring outer width 9.50 mm. B1-B6 busbar mounting uses RedCube (74650173R).

### 8.1 Field Wiring Wire Gauge

| Section | Wire gauge |
|---|---|
| HPS | 8 AWG |
| MPS | 18 AWG |

MPS gauge is sized against a 60°C temperature rise (space-constrained choice). With ambient assumed at 40°C (per [`docs/ThermalBudget.md`](ThermalBudget.md)), this puts continuous wire jacket temperature at ~100°C — insulation must be rated ≥105°C (e.g. silicone or high-temperature PVC/XLPE), not standard/low-temperature PVC.

## 9. Grounding Strategy

L2 is maintained as a continuous ground plane. Local perforation by power via fields is accepted where the affected area carries only non-critical signals with no dependency on ground return continuity through that specific zone.
