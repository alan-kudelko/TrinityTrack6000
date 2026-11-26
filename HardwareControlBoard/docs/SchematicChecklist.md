# Schematic Checklist

## 1. Purpose

This document tracks the verification status of each schematic sheet of the `HardwareControlBoard`.

A sheet is marked verified only after it has been checked against the project requirements, the relevant calculation documents, and the manufacturers' reference designs and datasheets.

## 2. Status Legend

| Status | Meaning |
|--------|---------|
| ⬜ Not reviewed | Sheet has not been checked yet |
| 🟡 In review | Review started, findings still open |
| ✅ Verified | Checked in full, no open findings |

## 3. Sheet Status

| Sheet | Blocks / ICs | Checked against | Status | Date | Notes |
|-------|--------------|-----------------|--------|------|-------|
| 01_XMC_Core (1/13) | XMC4500-F144 pin-to-net wiring, supply decoupling and ADC supply filter, 12 MHz crystal, reset, 20-pin Cortex debug and trace connector, debug UART header, MainControlBoard interface connector | `SignalList.md`, DAVE project, XMC4500 reference manual §27.6.2, Infineon XMC design guidelines | 🟡 In review | 2026-11-16 | Pending: synchronise the DAVE project with the final pins and names after the routing; revise the interboard pin order after the layout (keep a ground beside each signal) |
| 02_Power_Input (2/13) | Input connector, input TVS diode, DC-link entry, auxiliary +5 V / +6 V supply input with a fuse on each line | `ElectricalRequirements.md` §3.5, §4, §7.4, ST SM30T-series datasheet (DS8599), Yageo JB12S datasheet | ✅ Verified | 2026-11-16 | |
| 03_Gate_Driver_Supply (3/13) | LM5164DDAR buck (12 V gate supply), gate-supply source solder bridge, UVLO / feedback / ripple-injection networks, SW snubber | TI LM5164 datasheet (SNVSAU4D), `GateSupplyCalculations.md`, `SignalList.md`, `ElectricalRequirements.md` §3.5, §8.4 | ✅ Verified | 2026-11-10 | |
| 04_Logic_Power (4/13) | TLV76133DCYR 3.3 V LDO, input and output capacitors, power LED | TI TLV761 datasheet, `ThermalBudget.md` §4.5.1, `ElectricalRequirements.md` §4.2, §8.4 | ✅ Verified | 2026-11-09 | |
| 05_Precision_Voltage_Reference (5/13) | REF3016EAIDBZR 1.65 V reference, input and output capacitors | TI REF30/REF30E datasheet (SBVS032K), `CurrentSenseCalculations.md`, `ElectricalRequirements.md` §3.5, §8.4 | ✅ Verified | 2026-11-09 | |
| 06_Low_Power_Section (6/13) | Eight low-side outputs (PJA3406 MOSFETs), four 2EDN7533FXTMA1 dual gate drivers, POWER_ENABLE gating, local decoupling | Infineon 2EDN7533 datasheet (Rev. 1.0), Panjit PJA3406 datasheet, `ThermalBudget.md` LPS section, `FunctionalRequirements.md` §4.1, `ElectricalRequirements.md` §7.2, §8.4 | ✅ Verified | 2026-11-10 | |
| 07_Medium_Power_Section_H_Bridge (7/13) | Winch H-bridge (four PJQ4548P-AU MOSFETs), two UCC21551BDWKR gate drivers and gate networks, INA296A2IDGKR current sense with 5 mΩ low-side shunt, DC-link entry | TI UCC21551 datasheet (SLUSEW9D), Panjit PJQ4548P-AU datasheet, TI INA296 datasheet, `ThermalBudget.md` §4.2.2, `CurrentSenseCalculations.md`, `DCLinkCalculations.md` | ✅ Verified | 2026-11-16 | |
| 08_Medium_Power_Section_Low_Side (8/13) | Heater low-side switch (PJQ4548P-AU), 2EDN7533FXTMA1 gate driver, INA296A2IDGKR current sense with 5 mΩ low-side shunt (ground-referenced output), DC-link entry | Panjit PJQ4548P-AU and SS1060FL-AU datasheets, TI INA296 datasheet, `CurrentSenseCalculations.md`, `DCLinkCalculations.md` §4.3, `ThermalBudget.md` | ✅ Verified | 2026-11-16 | |
| 09_Medium_Power_Section_Stepper_Drivers (9/13) | DRV8434SPWPR stepper driver (one of six identical copies), charge pump, VM and DVDD capacitors, VREF divider, shared SPI / nFAULT / nSLEEP, motor connector | TI DRV8434S datasheet (SLOSE70), `DCLinkCalculations.md` §4.4, `SignalList.md`, `ElectricalRequirements.md` §3.5 | ✅ Verified | 2026-11-16 | |
| 10_High_Power_Section (10/13) | HPS H-bridge (four ISC011N04NM7V MOSFETs), two UCC21551BDWKR gate drivers with bootstrapped high-side supply and gate networks, INA296A2IDGKR current sense with 1 mΩ low-side shunt, DC-link capacitors; both H-bridges (A and B) checked, bridge B is a copy of bridge A | TI UCC21551 datasheet (SLUSEW9D), Infineon ISC011N04NM7V and Panjit SS1060FL-AU datasheets, `ThermalBudget.md` §4.3, §4.4, `CurrentSenseCalculations.md`, `DCLinkCalculations.md` §4.1 | 🟡 In review | 2026-11-16 | Recalculate the thermal budget (HPS and Winch) once the CCU8 dead time is set: body-diode conduction during dead time is not included yet |
| 11_Temperature_Measurements (11/13) | Four TMP235A4DBZR temperature sensors (HPS 1, HPS 2, MPS 1, MPS 2) with 100 Ω / 680 pF output filters | TI TMP235 datasheet (SBOS857E), `SignalList.md`, `ElectricalRequirements.md` | ✅ Verified | 2026-11-16 | |
| 12_Encoder_Inputs (12/13) | ISL32273EFVZ quad RS-422 receiver, two 6-pin encoder connectors (A/B per encoder, +5 V supply), 120 Ω terminations, fail-safe bias footprints (DNP), VCC bypass | Renesas ISL32273E datasheet (FN7529), Renesas TB509, `ElectricalRequirements.md` §6.4, `SignalList.md` | ✅ Verified | 2026-11-10 | |
| 13_Medium_Power_Section_Servomotors (13/13) | Six RC servo connectors (3-pin, +6 V, 330 Ω series resistor on each signal line), six limit switch connectors (2-pin), +6 V bulk capacitor | `SignalList.md`, `FunctionalRequirements.md` §4, `ElectricalRequirements.md` §6.3, §6.5 | 🟡 In review | 2026-11-16 | Pending: improve the drawings (captions, readability) |

## 4. Open Findings

| ID | Sheet | Finding | Severity |
|----|-------|---------|----------|
| BOM | All sheets | Update the BOM at the end to match the final schematic: the selected NTC thermistors (motor channels and heater channel), the +6 V servo fuse, the new VBAT capacitor, and any other part added or renumbered since the last BOM export | Low |
| F13-3 | 13_Medium_Power_Section_Servomotors | The six limit switch connectors now have a 10 kΩ pull-up and the LS_1 to LS_6 nets. Still open: the contact type (normally closed is fail-safe for a broken cable) | Low |
