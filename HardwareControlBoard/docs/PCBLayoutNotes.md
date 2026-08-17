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
| L4 (Bottom) | Power/supply polygons (gate drive supply, logic rails, etc.) |

## 5. Trace Widths

Applies to local routed traces; power/ground polygons are covered separately (see Section 8).

| Class | Width | Notes |
|-------|-------|-------|
| Signal | 0.200 mm | |
| Analog | 0.3 mm | |
| Power | 0.300 mm | Local connections |
| Gate driver | 0.7 mm | |

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

High-current nets are distributed primarily through copper bus bars (Würth Elektronik RedCube, 74650173R), sized 10x3 mm with provision to mount 12x3 mm, with PCB copper providing supplemental current spreading and local interconnection, per [`docs/ElectricalRequirements.md`](ElectricalRequirements.md) §4.4.

## 9. Grounding Strategy

L2 is maintained as a continuous ground plane. Local perforation by power via fields is accepted where the affected area carries only non-critical signals with no dependency on ground return continuity through that specific zone.
