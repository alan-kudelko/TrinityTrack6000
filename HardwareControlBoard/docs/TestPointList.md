# Test Point List

## 1. Purpose

This document lists the test points of the `HardwareControlBoard` by designator, with the placement rule for each one in the layout, and records the decisions taken on test point access.

It implements `ElectricalRequirements.md` §8.9: dedicated test points for critical analog measurement nodes and critical digital control signals, accessible without modifying the assembled board.

## 2. Designators

Test points present on the schematic. Updated each time a test point is added or renumbered.

| Designator | Net | Sheet | Placement |
|------------|-----|-------|-----------|
| TP1 | +12V (LM5164 output, before the gate supply solder bridge) | 03_Gate_Driver_Supply | After the LM5164 output capacitors, on the load side, not next to the converter. The ripple that matters is the one during gate switching, not the converter ripple. |
| TP2 | +3.3V (LDO output) | 04_Logic_Power | After the LDO output capacitor, on the load side. |
| TP3 | VREF_1.65V (REF3016 output) | 05_Precision_Voltage_Reference | After the reference output capacitor, on the load side. |
| TP4 | Winch gate, left leg high side (Gate9) | 07_Medium_Power_Section_H_Bridge | Short stub off the gate net, on the MOSFET side of the gate resistors, not in series in the gate loop. |
| TP5 | Winch gate, left leg low side (Gate10) | 07_Medium_Power_Section_H_Bridge | As TP4. |
| TP6 | Winch gate, right leg high side (Gate11) | 07_Medium_Power_Section_H_Bridge | As TP4. |
| TP7 | Winch gate, right leg low side (Gate12) | 07_Medium_Power_Section_H_Bridge | As TP4. |
| TP8 | WINCH_CURRENT (INA296 output) | 07_Medium_Power_Section_H_Bridge | Near the MCU end of the net, so it shows the signal the ADC sees, not at the amplifier output. |
| TP9 | HEATER_CURRENT (INA296 output) | 08_Medium_Power_Section_Low_Side | As TP8. |
| TP10 | HPS bridge A gate, left leg high side (Gate1) | 10_High_Power_Section | As TP4. |
| TP11 | HPS bridge A gate, left leg low side (Gate2) | 10_High_Power_Section | As TP4. |
| TP12 | HPS bridge A gate, right leg high side (Gate3) | 10_High_Power_Section | As TP4. |
| TP13 | HPS bridge A gate, right leg low side (Gate4) | 10_High_Power_Section | As TP4. |
| TP14 | HPSA_CURRENT (INA296 output) | 10_High_Power_Section | As TP8. |
| TP15 | HPS bridge B gate, left leg high side (Gate5) | 10_High_Power_Section | As TP4. |
| TP16 | HPS bridge B gate, left leg low side (Gate6) | 10_High_Power_Section | As TP4. |
| TP17 | HPS bridge B gate, right leg high side (Gate7) | 10_High_Power_Section | As TP4. |
| TP18 | HPS bridge B gate, right leg low side (Gate8) | 10_High_Power_Section | As TP4. |
| TP19 | HPSB_CURRENT (INA296 output) | 10_High_Power_Section | As TP8. |
| TP20 | HPSA_TEMP_IC | 11_Temperature_Measurements | As TP8. |
| TP21 | HPSB_TEMP_IC | 11_Temperature_Measurements | As TP8. |
| TP22 | HPSA_MOTOR_TEMP_NTC | 11_Temperature_Measurements | As TP8. |
| TP23 | MPS1_TEMP_IC | 11_Temperature_Measurements | As TP8. |
| TP24 | HPSB_MOTOR_TEMP_NTC | 11_Temperature_Measurements | As TP8. |
| TP25 | MPS2_TEMP_IC | 11_Temperature_Measurements | As TP8. |
| TP26 | HEATER_TEMP_NTC | 11_Temperature_Measurements | As TP8. |
| TP27 | ENC_1_OUTA (MCU side of the series resistor) | 01_XMC_Core | After the series resistor, on the MCU side. |
| TP28 | ENC_1_OUTB (MCU side of the series resistor) | 01_XMC_Core | After the series resistor, on the MCU side. |
| TP29 | ENC_2_OUTB (MCU side of the series resistor) | 01_XMC_Core | After the series resistor, on the MCU side. |
| TP30 | ENC_2_OUTA (MCU side of the series resistor) | 01_XMC_Core | After the series resistor, on the MCU side. |
| TP31 | SPI_MPS_SCK (stepper driver SPI clock, bus side of the series resistor) | 01_XMC_Core | After the series resistor, on the bus side. |
| TP32 | SPI_MPS_MOSI (stepper driver SPI data out, bus side of the series resistor) | 01_XMC_Core | After the series resistor, on the bus side. |
| TP33 | SPI_MPS_MISO (stepper driver SPI data in, at the MCU pin, no series resistor) | 01_XMC_Core | At the MCU pin end of the net. |
| TP34 | SPI_MISO (main SPI data from the MCU, bus side of the series resistor) | 01_XMC_Core | After the series resistor, on the bus side. |
| TP35 | SPI_MOSI (main SPI data to the MCU, MCU side of the 0 Ω resistor) | 01_XMC_Core | At the MCU pin end of the net, as a short stub. |
| TP36 | SPI_SCK (main SPI clock, MCU side of the 0 Ω resistor) | 01_XMC_Core | At the MCU pin end of the net, as a short stub. |
| TP37 | XMC_CS (main SPI chip select, at the MCU pin and the pull-up) | 01_XMC_Core | At the MCU pin end of the net, as a short stub. |
| TP38 | +3.3VA (ADC supply and reference, at the VAREF pin) | 01_XMC_Core | At the VAREF pin of the MCU, after the ADC supply filter. |
| TP39 | POWER_ENABLE (global enable, at the MCU pin) | 01_XMC_Core | At the MCU pin end of the net. |
| TP40 | GND (logic and analog, at the MCU ground pin) | 01_XMC_Core | Close to the MCU. |
| TP41 | GND (at the LM5164 ground pin, extra ground for short probe return) | 03_Gate_Driver_Supply | Close to the LM5164 and the SW snubber footprint. |
| TP42 | GND (at the DRV8434S exposed pad, extra ground for short probe return) | 09_Medium_Power_Section_Stepper_Drivers | Close to the stepper drivers (the sheet shows one of six identical copies; this test point is fitted once). |
| TP43 | GND (HPS bridge A gate drivers, extra ground for short probe return) | 10_High_Power_Section | Close to the bridge A gate drivers and the Gate_Supply local bypass capacitors. Also the return for the gate and supply ripple measurements there. |
| TP44 | GND (HPS bridge B gate drivers, extra ground for short probe return) | 10_High_Power_Section | Close to the bridge B gate drivers and the Gate_Supply local bypass capacitors. Also the return for the gate and supply ripple measurements there. |

## 3. Decisions Taken

- Test points on all four gate nets of the Winch and of both HPS bridges, for dead-time tuning and because the gate trace lengths may differ slightly between the gates. No dedicated gate test points on the other MOSFETs (Heater, LPS): there the gate voltage is measured when needed by soldering a thin wire to the MOSFET-side pad of the gate resistor. For the high-side MOSFETs the measurement reference is the switch node (the source), not board ground, so use a differential probe or connect the probe ground to the switch node.
- No test point on the LM5164 SW node. The SW waveform (peak, undershoot, ring frequency) is probed at the pads of the SW snubber footprint, using the GND-side pad as the probe ground. Place that footprint right at the SW pin and the inductor, and keep a ground test point (TP41) close to the buck.
- No test points on the Low Power Section sheet. Its outputs are reachable at the load connectors, the gate nets use the wire method, and the local supply is covered by the Gate_Supply test point.
- No test points on the servomotor sheet. The servo and limit switch connectors are through-hole, so the signals are accessible at the connector pins.
- No test points on the PWM input lines (LPS, Winch, HPS, Heater) and on the STEP and DIR lines. The series resistors in these lines give probe access, and the gate test points cover the dead time and synchronisation of the H-bridges.
- No test points on XMC_RST, U2_PGOOD, DRV_FAULT and XMC_RDY: slow digital signals, probed at the resistors or the debug connector. POWER_ENABLE, being the global gating signal, has one test point at the MCU pin.
- The ring terminals of the Winch, HPS and Heater outputs are the test access for the output (switch node) voltage and for the probe ground of the high-side gate measurements. The pads are large, and a test cable can be screwed down through the terminal hole. No dedicated test points on the switch nodes.
- No test points on VDD_IN, +5V and +6V. They are accessible at the through-hole connectors and the large components (fuses, capacitors, DC-link capacitors). No test point on the MCU core supply (VDDC).
- No dedicated test points on the power-section and gate-driver grounds. Grounds are accessible at the through-hole components and the Redcube busbar connectors. A few additional ground test points are added where a short probe return path is needed, and listed in the designator table as they are added.
- No hook (loop) test points: there is no room for them. Every test point (including the grounds) is a round pad with a plated hole, so a probe tip sits in the hole and does not slide off. One footprint for all of them, no component and no BOM line. Size: 1.2 mm round pad with a via in the centre (1.0 mm via diameter, 0.6 mm hole, 0.2 mm annular ring), same footprint for every test point including the grounds, solder mask opening over the pad, hole not tented and not filled.
