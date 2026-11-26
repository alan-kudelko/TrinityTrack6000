# Signal List

## 1. Purpose

This document records the list of external signals brought to the XMC4500 MCU on `HardwareControlBoard`, with the net names used on the schematic, compiled from the final MCU sheet.

## 2. Scope

This document covers only the signal inventory: net name, originating or destination block, direction, and the known constraints of each signal (peripheral type, synchronization, pull-ups, series resistors). It does not cover pin assignment, which is done in the schematic and the DAVE project. This document is a working record, not a set of normative requirements.

**Notes:**
- Net names are as drawn on the schematic. The numbering of the LPS, STEP, CS, DIR, SERVO and LS channels may change after the routing; the signal functions do not.
- The 22 Ω series resistors mentioned below are on the MCU sheet, next to the MCU pin.

## 3. Signal List (91 signals)

| ID | Signal Name | Source / Destination Block | Direction (to/from MCU) | Notes |
|---|---|---|---|---|
| 1 | U2_PGOOD | Gate-drive supply (LM5164 buck) | To MCU (input) | Open-drain power-good output, 10 kΩ pull-up to +3.3 V. Safety-critical: if the gate supply sags, PWM generation must stop. ERU event input. |
| 2 | POWER_ENABLE | MainControlBoard | To MCU (input) | Global power enable, active high. Gated in hardware directly to every power stage except the RC servos (the servo rail is not switched by this board). The MCU only reads the state. Plain digital input. |
| 3 | XMC_RDY | MainControlBoard | From MCU (output) | Ready flag to the MainControlBoard. Open-drain, active low, 4.7 kΩ pull-up to +3.3 V. |
| 4 | XMC_RST | MainControlBoard, debug connector | To MCU (input) | MCU reset (PORST), 10 kΩ pull-up to +3.3 V. The MainControlBoard can hard-reset the MCU through it. |
| 5 | LPS_1_PWM | LPS gate driver (2EDN7533), output 1 | From MCU (output) | PWM up to 40 kHz. Single CCU4/CCU8 slice output. Every LPS driver input has a 10 kΩ pull-down and no series resistor. |
| 6 | LPS_2_PWM | LPS gate driver, output 2 | From MCU (output) | PWM up to 40 kHz, as LPS_1_PWM. |
| 7 | LPS_3_PWM | LPS gate driver, output 3 | From MCU (output) | PWM up to 40 kHz, as LPS_1_PWM. |
| 8 | LPS_4_PWM | LPS gate driver, output 4 | From MCU (output) | PWM up to 40 kHz, as LPS_1_PWM. |
| 9 | LPS_5 | LPS gate driver, output 5 | From MCU (output) | Digital on/off, plain GPIO. |
| 10 | LPS_6 | LPS gate driver, output 6 | From MCU (output) | Digital on/off, plain GPIO. |
| 11 | LPS_7 | LPS gate driver, output 7 | From MCU (output) | Digital on/off, plain GPIO. |
| 12 | LPS_8 | LPS gate driver, output 8 | From MCU (output) | Digital on/off, plain GPIO. |
| 13 | IN_WINCH_RIGHT_HIGH | Winch H-bridge gate driver (UCC21551), leg 2 high-side | From MCU (output) | H-bridge leg PWM, 40 kHz. All four Winch signals come from one CCU8 slice with two complementary channels; dead time is inserted by the CCU8 only (the driver DT pin is not populated). |
| 14 | IN_WINCH_RIGHT_LOW | Winch H-bridge gate driver, leg 2 low-side | From MCU (output) | Complementary to IN_WINCH_RIGHT_HIGH. |
| 15 | IN_WINCH_LEFT_HIGH | Winch H-bridge gate driver, leg 1 high-side | From MCU (output) | H-bridge leg PWM, 40 kHz, same slice as the other Winch signals. |
| 16 | IN_WINCH_LEFT_LOW | Winch H-bridge gate driver, leg 1 low-side | From MCU (output) | Complementary to IN_WINCH_LEFT_HIGH. |
| 17 | WINCH_CURRENT | Winch current-sense amplifier (INA296A2) | To MCU (analog input) | Bidirectional, 100 mV/A (20 V/V, 5 mΩ), centred on 1.65 V. EMI-sensitive. ADC conversion triggered by the Winch CCU8 slice and sampled in a clean window of the switching cycle (see `CurrentSenseCalculations.md`). |
| 18 | IN_HEATER | Heater low-side gate driver (2EDN7533) | From MCU (output) | Heater PWM, single low-side switch, up to 40 kHz, CCU8 output. 22 Ω series resistor. |
| 19 | HEATER_CURRENT | Heater current-sense amplifier (INA296A2) | To MCU (analog input) | Unidirectional, ground-referenced, 100 mV/A (0.5–1.0 V at 5–10 A). EMI-sensitive. ADC conversion triggered by the Heater CCU8 slice. |
| 20 | SPI_MPS_SCK | Stepper drivers (DRV8434S), shared bus | From MCU (output) | SPI master clock, 1 MHz maximum. 22 Ω series resistor. Configuration and diagnostics only, not time-critical. |
| 21 | SPI_MPS_MOSI | Stepper drivers, shared bus | From MCU (output) | SPI data out to all six drivers, 1 MHz maximum. 22 Ω series resistor. |
| 22 | SPI_MPS_MISO | Stepper drivers, shared bus | To MCU (input) | SPI data in from all six drivers. No series resistor at the MCU pin; each driver has a 22 Ω series resistor at its SDO output instead. |
| 23 | MPS_CS_1 | Stepper driver 1 | From MCU (output) | Chip select, software-controlled GPIO. |
| 24 | MPS_CS_2 | Stepper driver 2 | From MCU (output) | Chip select, software-controlled GPIO. |
| 25 | MPS_CS_3 | Stepper driver 3 | From MCU (output) | Chip select, software-controlled GPIO. |
| 26 | MPS_CS_4 | Stepper driver 4 | From MCU (output) | Chip select, software-controlled GPIO. |
| 27 | MPS_CS_5 | Stepper driver 5 | From MCU (output) | Chip select, software-controlled GPIO. |
| 28 | MPS_CS_6 | Stepper driver 6 | From MCU (output) | Chip select, software-controlled GPIO. |
| 29 | DRV_FAULT | Stepper drivers, shared | To MCU (input) | Open-drain fault output shared by all six drivers, one common 10 kΩ pull-up to +3.3 V. The faulting driver is identified by an SPI status read. ERU event input for a fast hardware response. |
| 30 | DRV_SLEEP | Stepper drivers, shared (nSLEEP) | From MCU (output) | Active-high enable. The device has an internal pull-down (about 200 kΩ), so an undriven line means sleep. |
| 31 | MPS_DIR_1 | Stepper driver 1 (DIR) | From MCU (output) | Direction, plain GPIO. The device pin has an internal pull-down. |
| 32 | MPS_DIR_2 | Stepper driver 2 (DIR) | From MCU (output) | As MPS_DIR_1. |
| 33 | MPS_DIR_3 | Stepper driver 3 (DIR) | From MCU (output) | As MPS_DIR_1. |
| 34 | MPS_DIR_4 | Stepper driver 4 (DIR) | From MCU (output) | As MPS_DIR_1. |
| 35 | MPS_DIR_5 | Stepper driver 5 (DIR) | From MCU (output) | As MPS_DIR_1. |
| 36 | MPS_DIR_6 | Stepper driver 6 (DIR) | From MCU (output) | As MPS_DIR_1. |
| 37 | MPS_STEP_1 | Stepper driver 1 (STEP) | From MCU (output) | Step pulse train, application target 40 kHz (the driver accepts up to 500 kHz). Each STEP signal has its own independent CCU4 slice. |
| 38 | MPS_STEP_2 | Stepper driver 2 (STEP) | From MCU (output) | As MPS_STEP_1. |
| 39 | MPS_STEP_3 | Stepper driver 3 (STEP) | From MCU (output) | As MPS_STEP_1. |
| 40 | MPS_STEP_4 | Stepper driver 4 (STEP) | From MCU (output) | As MPS_STEP_1. |
| 41 | MPS_STEP_5 | Stepper driver 5 (STEP) | From MCU (output) | As MPS_STEP_1. |
| 42 | MPS_STEP_6 | Stepper driver 6 (STEP) | From MCU (output) | As MPS_STEP_1. |
| 43 | IN_HPSA_LEFT_HIGH | HPS bridge A gate driver (UCC21551), leg 1 high-side | From MCU (output) | H-bridge leg PWM, 40 kHz. All four HPSA signals come from one CCU8 slice with two complementary channels; dead time is inserted by the CCU8 only (the driver DT pin is not populated). |
| 44 | IN_HPSA_LEFT_LOW | HPS bridge A gate driver, leg 1 low-side | From MCU (output) | Complementary to IN_HPSA_LEFT_HIGH. |
| 45 | IN_HPSA_RIGHT_HIGH | HPS bridge A gate driver, leg 2 high-side | From MCU (output) | H-bridge leg PWM, 40 kHz, same slice as the other HPSA signals. |
| 46 | IN_HPSA_RIGHT_LOW | HPS bridge A gate driver, leg 2 low-side | From MCU (output) | Complementary to IN_HPSA_RIGHT_HIGH. |
| 47 | HPSA_CURRENT | HPS bridge A current-sense amplifier (INA296A2) | To MCU (analog input) | Bidirectional, 20 mV/A (20 V/V, 1 mΩ), centred on 1.65 V. EMI-sensitive. ADC conversion triggered by the HPSA CCU8 slice and sampled in a clean window of the switching cycle. |
| 48 | IN_HPSB_LEFT_HIGH | HPS bridge B gate driver (UCC21551), leg 1 high-side | From MCU (output) | H-bridge leg PWM, 40 kHz. Same arrangement as bridge A, on its own CCU8 slice. |
| 49 | IN_HPSB_LEFT_LOW | HPS bridge B gate driver, leg 1 low-side | From MCU (output) | Complementary to IN_HPSB_LEFT_HIGH. |
| 50 | IN_HPSB_RIGHT_HIGH | HPS bridge B gate driver, leg 2 high-side | From MCU (output) | H-bridge leg PWM, 40 kHz, same slice as the other HPSB signals. |
| 51 | IN_HPSB_RIGHT_LOW | HPS bridge B gate driver, leg 2 low-side | From MCU (output) | Complementary to IN_HPSB_RIGHT_HIGH. |
| 52 | HPSB_CURRENT | HPS bridge B current-sense amplifier (INA296A2) | To MCU (analog input) | As HPSA_CURRENT, triggered by the HPSB CCU8 slice. Converted in a different VADC group than HPSA_CURRENT. |
| 53 | HPSA_TEMP_IC | Temperature sensor (TMP235A4), HPS A | To MCU (analog input) | 10 mV/°C, 500 mV at 0 °C, 100 Ω and 680 pF output filter. Slow signal, no CCU synchronization; firmware averages 4–16 samples. |
| 54 | HPSB_TEMP_IC | Temperature sensor (TMP235A4), HPS B | To MCU (analog input) | As HPSA_TEMP_IC. |
| 55 | MPS1_TEMP_IC | Temperature sensor (TMP235A4), MPS 1 | To MCU (analog input) | As HPSA_TEMP_IC. |
| 56 | MPS2_TEMP_IC | Temperature sensor (TMP235A4), MPS 2 | To MCU (analog input) | As HPSA_TEMP_IC. |
| 57 | HPSA_MOTOR_TEMP_NTC | NTC divider, HPS motor A (casing) | To MCU (analog input) | NTC 10 kΩ, B 3950 K, 1.5 kΩ pull-up to +3.3 V, 1 kΩ series resistor, no filter capacitor. Slow signal; needs a long ADC sample time. |
| 58 | HPSB_MOTOR_TEMP_NTC | NTC divider, HPS motor B (casing) | To MCU (analog input) | As HPSA_MOTOR_TEMP_NTC. |
| 59 | HEATER_TEMP_NTC | NTC divider, heater | To MCU (analog input) | Glass NTC 100 kΩ, B 3950 K, up to 300 °C, 3.3 kΩ pull-up, 1 kΩ series resistor, no filter capacitor. A short or over-temperature reading must disable the heater. |
| 60 | ENC_1_OUTA | Encoder 1 receiver (ISL32273E, RS-422) | To MCU (input) | Quadrature channel A. POSIF input; A and B go to the same POSIF unit. A 22 Ω series resistor is fitted in each of the four ENC_x_OUT lines. |
| 61 | ENC_1_OUTB | Encoder 1 receiver | To MCU (input) | Quadrature channel B. |
| 62 | ENC_2_OUTA | Encoder 2 receiver | To MCU (input) | Quadrature channel A, on the second POSIF unit. |
| 63 | ENC_2_OUTB | Encoder 2 receiver | To MCU (input) | Quadrature channel B. |
| 64 | LS_1 | Limit switch, channel 1 | To MCU (input) | 10 kΩ pull-up to +3.3 V. ERU event input, so the stop does not depend on polling. |
| 65 | LS_2 | Limit switch, channel 2 | To MCU (input) | As LS_1. |
| 66 | LS_3 | Limit switch, channel 3 | To MCU (input) | As LS_1. |
| 67 | LS_4 | Limit switch, channel 4 | To MCU (input) | As LS_1. |
| 68 | LS_5 | Limit switch, channel 5 | To MCU (input) | As LS_1. |
| 69 | LS_6 | Limit switch, channel 6 | To MCU (input) | As LS_1. |
| 70 | MPS_SERVO_1 | RC servo, channel 1 | From MCU (output) | Servo PWM, 50 Hz. Output of a CCU8 slice with two channels sharing one period. 330 Ω series resistor on the signal line at the connector. |
| 71 | MPS_SERVO_2 | RC servo, channel 2 | From MCU (output) | As MPS_SERVO_1. |
| 72 | MPS_SERVO_3 | RC servo, channel 3 | From MCU (output) | As MPS_SERVO_1. |
| 73 | MPS_SERVO_4 | RC servo, channel 4 | From MCU (output) | As MPS_SERVO_1. |
| 74 | MPS_SERVO_5 | RC servo, channel 5 | From MCU (output) | As MPS_SERVO_1. |
| 75 | MPS_SERVO_6 | RC servo, channel 6 | From MCU (output) | As MPS_SERVO_1. |
| 76 | SPI_MOSI | MainControlBoard | To MCU (input) | Main SPI bus, the MCU is the slave, target clock 20–40 MHz. Time-critical real-time command and data link. 22 Ω series resistor. The XMC4500 slave maximum clock is not confirmed (the electrical datasheet is not in the project). |
| 77 | SPI_MISO | MainControlBoard | From MCU (output) | Main SPI bus data out of the MCU. 22 Ω series resistor. |
| 78 | SPI_SCK | MainControlBoard | To MCU (input) | Main SPI bus clock, 20–40 MHz. 22 Ω series resistor. |
| 79 | XMC_CS | MainControlBoard | To MCU (input) | Chip select of the main SPI bus, hardware slave-select input of the same USIC channel (at 20–40 MHz it cannot be handled in software). 4.7 kΩ pull-up to +3.3 V, no series resistor. |
| 80 | UART_TX | Debug UART header | From MCU (output) | Debug UART. Not time-critical. |
| 81 | UART_RX | Debug UART header | To MCU (input) | Debug UART. Not time-critical. |
| 82 | TMS | Debug connector (20-pin Cortex Debug + ETM) | To MCU (input) | SWD/JTAG, 22 Ω series resistor. Dedicated pin. |
| 83 | TCK | Debug connector | To MCU (input) | SWD/JTAG clock, 22 Ω series resistor. Dedicated pin. |
| 84 | TDO | Debug connector | From MCU (output) | JTAG data out / SWO, 22 Ω series resistor. Alternate function of a port pin. |
| 85 | TDI | Debug connector | To MCU (input) | JTAG data in, 22 Ω series resistor. Alternate function of a port pin. |
| 86 | TRACECLK | Debug connector | From MCU (output) | ETM trace clock, 22 Ω series resistor. Alternate function of a port pin. |
| 87 | TRACEDATA0 | Debug connector | From MCU (output) | ETM trace data, 22 Ω series resistor. |
| 88 | TRACEDATA1 | Debug connector | From MCU (output) | ETM trace data, 22 Ω series resistor. |
| 89 | TRACEDATA2 | Debug connector | From MCU (output) | ETM trace data, 22 Ω series resistor. |
| 90 | TRACEDATA3 | Debug connector | From MCU (output) | ETM trace data, 22 Ω series resistor. |
| 91 | GNDDetect | Debug connector | To MCU (input) | Pulled low by the debug probe when plugged in. Uses the internal pull-up of the MCU pin. |

## 4. Resource Summary

| Resource | Signals |
|---|---|
| ERU event inputs (8 channels) | DRV_FAULT, U2_PGOOD, LS_1 to LS_6 |
| CCU8, complementary pairs with dead time | Winch, HPSA, HPSB (one slice each, two channels) |
| CCU8, single output | IN_HEATER |
| CCU8 twin-channel slices (shared period) | MPS_SERVO_1 to MPS_SERVO_6 |
| CCU4 single slices | MPS_STEP_1 to MPS_STEP_6 (each on its own slice), LPS_2_PWM, LPS_4_PWM |
| CCU8, one slice with two outputs | LPS_1_PWM and LPS_3_PWM (shared period, independent duty) |
| POSIF | ENC_1 (POSIF0), ENC_2 (POSIF1) |
| VADC, synchronized to CCU8 | WINCH_CURRENT, HEATER_CURRENT, HPSA_CURRENT, HPSB_CURRENT |
| VADC, slow | four `*_TEMP_IC`, three `*_TEMP_NTC` |
| USIC | stepper SPI master, MainControlBoard SPI slave, debug UART |
