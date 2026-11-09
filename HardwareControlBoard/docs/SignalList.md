# Signal List

## 1. Purpose

This document records the working list of external signals that must be brought to the XMC4500 MCU on `HardwareControlBoard`, compiled section by section from the board's schematics before pin assignment.

## 2. Scope

This document covers only the signal inventory — name, originating/destination block, direction, and any known assignment constraint (e.g. ADC-capable, timer/CCU-capable, EMI-sensitive). It does not cover actual pin assignment. Pin assignment will be performed separately using DAVE IDE and the XMC4500 datasheet, and recorded afterward in a separate pinout document listing only the pins actually used (not all 144). This document is a working record, not a set of normative requirements.

**Note:** several entries currently reference a block/IC type only (e.g. "HPS bridge A driver (UCC21551)") without an exact schematic designator. Designators are to be filled in once the full signal list is complete.

## 3. Signal List

| ID | Signal Name | Source / Destination Block | Direction (to/from MCU) | Notes |
|---|---|---|---|---|
| 1 | U2_PGOOD | Gate_Supply (U2, LM5164 buck) | To MCU (input) | Open-drain output signaling gate-drive supply voltage is in regulation. Pulled up to +3.3V via R6=10k on the board. Reclassified 2026-09-03: safety-critical, arguably more urgent than DRV_FAULT — if the gate supply sags, the XMC will keep generating PWM into a bad gate drive before DRV_FAULT itself would trip. Worth routing through the ERU as a hardware event input, same rationale as POWER_ENABLE/DRV_FAULT/MPS_LSx — needs an ERU0/ERU1-capable pin. Fits the 8-channel ERU budget exactly now that POWER_ENABLE was downgraded to plain polled `DIGITAL_IO` (confirmed hardware-gated independent of XMC) — the set is DRV_FAULT, U2_PGOOD, MPS_LS1-6. |
| 2 | U5_INA | LPS driver U5 (2EDN7533FXTMA1) | From MCU (output) | LPS PWM channel (up to 40kHz per `ThermalBudget.md` §4.2) — needs a CCU4/CCU8 timer-capable pin. |
| 3 | U5_INB | LPS driver U5 (2EDN7533FXTMA1) | From MCU (output) | LPS PWM channel — needs a CCU4/CCU8 timer-capable pin. |
| 4 | U6_INA | LPS driver U6 (2EDN7533FXTMA1) | From MCU (output) | LPS PWM channel — needs a CCU4/CCU8 timer-capable pin. |
| 5 | U6_INB | LPS driver U6 (2EDN7533FXTMA1) | From MCU (output) | LPS PWM channel — needs a CCU4/CCU8 timer-capable pin. |
| 6 | U7_INA | LPS driver U7 (2EDN7533FXTMA1) | From MCU (output) | LPS digital ON/OFF channel — plain GPIO for now, may upgrade to PWM-capable pin later if pin budget allows. |
| 7 | U7_INB | LPS driver U7 (2EDN7533FXTMA1) | From MCU (output) | LPS digital ON/OFF channel — plain GPIO for now, may upgrade to PWM-capable pin later if pin budget allows. |
| 8 | U8_INA | LPS driver U8 (2EDN7533FXTMA1) | From MCU (output) | LPS digital ON/OFF channel — plain GPIO for now, may upgrade to PWM-capable pin later if pin budget allows. |
| 9 | U8_INB | LPS driver U8 (2EDN7533FXTMA1) | From MCU (output) | LPS digital ON/OFF channel — plain GPIO for now, may upgrade to PWM-capable pin later if pin budget allows. |
| 10 | IN_WINCH_RIGHT_HIGH | Winch H-bridge driver U9 (UCC21551), leg 1 high-side | From MCU (output) | H-bridge leg PWM, driven at 40kHz. Must share the same CCU module/timer instance as the other 3 Winch signals below for synchronized switching and dead-time insertion — CCU8 is the natural fit (built-in complementary-pair dead-time generation for motor H-bridge control). |
| 11 | IN_WINCH_RIGHT_LOW | Winch H-bridge driver U9 (UCC21551), leg 1 low-side | From MCU (output) | H-bridge leg PWM (INLO), driven at 40kHz. Same CCU module as IN_WINCH_RIGHT_HIGH. |
| 12 | IN_WINCH_LEFT_HIGH | Winch H-bridge driver U10 (UCC21551), leg 2 high-side | From MCU (output) | H-bridge leg PWM (INHI), driven at 40kHz. Same CCU module as the other three Winch signals. |
| 13 | IN_WINCH_LEFT_LOW | Winch H-bridge driver U10 (UCC21551), leg 2 low-side | From MCU (output) | H-bridge leg PWM (INLO), driven at 40kHz. Same CCU module as the other three Winch signals. |
| 14 | VADC_H3_OUT | U11 (INA190 current-sense amp, Winch) | To MCU (analog input) | Winch current-sense analog output. Needs a VADC-capable pin (analog Group 0/1 channel). EMI-sensitive — one of the signals flagged earlier as needing distance from Gate_Supply, since noise here directly corrupts current measurement accuracy. ADC conversion should be triggered by the same CCU module driving the Winch PWM (CCU-triggered VADC conversion), sampled in a clean window of the switching cycle rather than free-running, to avoid capturing switching-transient noise. |
| 15 | U13_INA | Heater/MPS low-side driver U13 (2EDN7533FXTMA1) | From MCU (output) | Heater PWM channel — single low-side switch, up to 40kHz per `ThermalBudget.md` §4.2.1. Needs a CCU4/CCU8 timer-capable pin. |
| 16 | VADC_L_OUT | U12 (INA190 current-sense amp, Heater/low-side MPS) | To MCU (analog input) | Heater current-sense analog output. Needs a VADC-capable pin. EMI-sensitive, same rationale as VADC_H3_OUT. ADC conversion should be triggered by the same CCU module driving U13_INA's PWM, sampled in a clean window of the switching cycle. |
| 17 | SPI_MPS_SCK | Stepper drivers U14-U19 (DRV8434S, shared bus) | From MCU (output) | SPI clock, target ~10MHz. Not time-sensitive — configuration/diagnostics only, not real-time control. Needs a USIC (SPI-Master) capable pin. |
| 18 | SPI_MPS_MOSI | Stepper drivers U14-U19 (DRV8434S, shared bus) | From MCU (output) | SPI data out, shared bus to all 6 drivers. Not time-sensitive. Needs a USIC (SPI-Master) capable pin, same channel as SPI_MPS_SCK/MISO. |
| 19 | SPI_MPS_MISO | Stepper drivers U14-U19 (DRV8434S, shared bus) | To MCU (input) | SPI data in, shared bus from all 6 drivers. Not time-sensitive. Needs a USIC (SPI-Master) capable pin, same channel as SPI_MPS_SCK/MOSI. |
| 20 | MPS1_CS | Stepper driver U14 (DRV8434S) | From MCU (output) | Chip select for the shared SPI bus, software (GPIO) controlled — plain digital output, no USIC/timer requirement. |
| 21 | MPS2_CS | Stepper driver U15 (DRV8434S) | From MCU (output) | Chip select for the shared SPI bus, software (GPIO) controlled — plain digital output, no USIC/timer requirement. |
| 22 | MPS3_CS | Stepper driver U16 (DRV8434S) | From MCU (output) | Chip select for the shared SPI bus, software (GPIO) controlled — plain digital output, no USIC/timer requirement. |
| 23 | MPS4_CS | Stepper driver U17 (DRV8434S) | From MCU (output) | Chip select for the shared SPI bus, software (GPIO) controlled — plain digital output, no USIC/timer requirement. |
| 24 | MPS5_CS | Stepper driver U18 (DRV8434S) | From MCU (output) | Chip select for the shared SPI bus, software (GPIO) controlled — plain digital output, no USIC/timer requirement. |
| 25 | MPS6_CS | Stepper driver U19 (DRV8434S) | From MCU (output) | Chip select for the shared SPI bus, software (GPIO) controlled — plain digital output, no USIC/timer requirement. |
| 26 | DRV_FAULT | Stepper drivers U14-U19 (DRV8434S), shared | To MCU (input) | Open-drain fault output, shared across all 6 drivers, pulled up to +3.3V. Since it's shared, faulting driver identity must be resolved via SPI status read (per-driver), not from this pin alone. Should be routed through XMC4500's ERU (Event Request Unit) as a hardware event input rather than a plain polled GPIO, for fast response (e.g. emergency PWM shutdown) without software polling latency — needs an ERU0/ERU1-capable pin. |
| 27 | DRV_SLEEP | Stepper drivers U14-U19 (DRV8434S), shared (nSLEEP) | From MCU (output) | Active-high enable, internal ~200kΩ pulldown on the device (defaults to sleep/safe state if undriven, per §7.3.10). |
| 28 | MPS_DIR1 | Stepper driver U14 (DRV8434S DIR pin) | From MCU (output) | Direction control, plain GPIO — internal pulldown on the device pin. |
| 29 | MPS_DIR2 | Stepper driver U15 (DRV8434S DIR pin) | From MCU (output) | Direction control, plain GPIO — internal pulldown on the device pin. |
| 30 | MPS_DIR3 | Stepper driver U16 (DRV8434S DIR pin) | From MCU (output) | Direction control, plain GPIO — internal pulldown on the device pin. |
| 31 | MPS_DIR4 | Stepper driver U17 (DRV8434S DIR pin) | From MCU (output) | Direction control, plain GPIO — internal pulldown on the device pin. |
| 32 | MPS_DIR5 | Stepper driver U18 (DRV8434S DIR pin) | From MCU (output) | Direction control, plain GPIO — internal pulldown on the device pin. |
| 33 | MPS_DIR6 | Stepper driver U19 (DRV8434S DIR pin) | From MCU (output) | Direction control, plain GPIO — internal pulldown on the device pin. |
| 34 | MPS_STEP1 | Stepper driver U14 (DRV8434S STEP pin) | From MCU (output) | PWM step pulse train, application target 40kHz (datasheet supports up to 500kHz — tWH/tWL min 970ns each — so this is a design margin choice, not a part limit). Needs a CCU4/CCU8 timer-capable pin. |
| 35 | MPS_STEP2 | Stepper driver U15 (DRV8434S STEP pin) | From MCU (output) | PWM step pulse train, application target 40kHz (datasheet supports up to 500kHz). Needs a CCU4/CCU8 timer-capable pin. |
| 36 | MPS_STEP3 | Stepper driver U16 (DRV8434S STEP pin) | From MCU (output) | PWM step pulse train, application target 40kHz (datasheet supports up to 500kHz). Needs a CCU4/CCU8 timer-capable pin. |
| 37 | MPS_STEP4 | Stepper driver U17 (DRV8434S STEP pin) | From MCU (output) | PWM step pulse train, application target 40kHz (datasheet supports up to 500kHz). Needs a CCU4/CCU8 timer-capable pin. |
| 38 | MPS_STEP5 | Stepper driver U18 (DRV8434S STEP pin) | From MCU (output) | PWM step pulse train, application target 40kHz (datasheet supports up to 500kHz). Needs a CCU4/CCU8 timer-capable pin. |
| 39 | MPS_STEP6 | Stepper driver U19 (DRV8434S STEP pin) | From MCU (output) | PWM step pulse train, application target 40kHz (datasheet supports up to 500kHz). Needs a CCU4/CCU8 timer-capable pin. |
| 40 | VADC_H1_OUT | HPS H-bridge current-sense amp (INA190A1QDDFRQ1), channel 1 | To MCU (analog input) | HPS current-sense analog output, bidirectional (per `CurrentSenseCalculations.md` §3.2). Needs a VADC-capable pin. EMI-sensitive, same rationale as VADC_H3_OUT/VADC_L_OUT. ADC conversion must be CCU-triggered, synchronized to the same CCU module driving the HPSA PWM signals below, sampled in a clean window of the switching cycle. |
| 41 | IN_HPSA_LEFT_HIGH | HPS bridge A driver (UCC21551), leg 1 high-side | From MCU (output) | H-bridge leg PWM (INHI), driven at 40kHz. Must share the same CCU module/timer instance as the other 3 HPSA signals for synchronized switching and dead-time insertion. |
| 42 | IN_HPSA_LEFT_LOW | HPS bridge A driver (UCC21551), leg 1 low-side | From MCU (output) | H-bridge leg PWM (INLO), driven at 40kHz. Same CCU module as the other three HPSA signals. |
| 43 | IN_HPSA_RIGHT_HIGH | HPS bridge A driver (UCC21551), leg 2 high-side | From MCU (output) | H-bridge leg PWM (INHI), driven at 40kHz. Same CCU module as the other three HPSA signals. |
| 44 | IN_HPSA_RIGHT_LOW | HPS bridge A driver (UCC21551), leg 2 low-side | From MCU (output) | H-bridge leg PWM (INLO), driven at 40kHz. Same CCU module as the other three HPSA signals. |
| 45 | VADC_H2_OUT | HPS H-bridge current-sense amp (INA190A1QDDFRQ1), channel 2 | To MCU (analog input) | HPS current-sense analog output, bidirectional (per `CurrentSenseCalculations.md` §3.2). Needs a VADC-capable pin. EMI-sensitive, same rationale as VADC_H1_OUT. ADC conversion must be CCU-triggered, synchronized to the same CCU module driving the HPSB PWM signals below, sampled in a clean window of the switching cycle. |
| 46 | IN_HPSB_LEFT_HIGH | HPS bridge B driver (UCC21551), leg 1 high-side | From MCU (output) | H-bridge leg PWM (INHI), driven at 40kHz. Must share the same CCU module/timer instance as the other 3 HPSB signals for synchronized switching and dead-time insertion. |
| 47 | IN_HPSB_LEFT_LOW | HPS bridge B driver (UCC21551), leg 1 low-side | From MCU (output) | H-bridge leg PWM (INLO), driven at 40kHz. Same CCU module as the other three HPSB signals. |
| 48 | IN_HPSB_RIGHT_HIGH | HPS bridge B driver (UCC21551), leg 2 high-side | From MCU (output) | H-bridge leg PWM (INHI), driven at 40kHz. Same CCU module as the other three HPSB signals. |
| 49 | IN_HPSB_RIGHT_LOW | HPS bridge B driver (UCC21551), leg 2 low-side | From MCU (output) | H-bridge leg PWM (INLO), driven at 40kHz. Same CCU module as the other three HPSB signals. |
| 50 | VADC_HPS1_TEMP_OUT | U26 (TMP235A4DBZR) | To MCU (analog input) | Temperature sense analog output. Needs a VADC-capable pin. Slow-changing signal — unlike the current-sense channels, does not need CCU-triggered/synchronized sampling. |
| 51 | VADC_HPS2_TEMP_OUT | U27 (TMP235A4DBZR) | To MCU (analog input) | Temperature sense analog output. Needs a VADC-capable pin. Slow-changing signal — no CCU-sync requirement. |
| 52 | VADC_MPS1_TEMP_OUT | U28 (TMP235A4DBZR) | To MCU (analog input) | Temperature sense analog output. Needs a VADC-capable pin. Slow-changing signal — no CCU-sync requirement. |
| 53 | VADC_MPS2_TEMP_OUT | U29 (TMP235A4DBZR) | To MCU (analog input) | Temperature sense analog output. Needs a VADC-capable pin. Slow-changing signal — no CCU-sync requirement. |
| 54 | ENC_1_OUTA | U30 (ISL32273EFVZ, quad channel) | To MCU (input) | Quadrature encoder 1, channel A. Should route to a POSIF/CCU4-capable pin pair for hardware quadrature decoding — pair with ENC_1_OUTB on the same POSIF/CCU4 slice, not arbitrary GPIO. |
| 55 | ENC_1_OUTB | U30 (ISL32273EFVZ, quad channel) | To MCU (input) | Quadrature encoder 1, channel B. Same POSIF/CCU4 slice as ENC_1_OUTA. |
| 56 | ENC_2_OUTA | U30 (ISL32273EFVZ, quad channel) | To MCU (input) | Quadrature encoder 2, channel A. Should route to a POSIF/CCU4-capable pin pair — pair with ENC_2_OUTB on the same POSIF/CCU4 slice. |
| 57 | ENC_2_OUTB | U30 (ISL32273EFVZ, quad channel) | To MCU (input) | Quadrature encoder 2, channel B. Same POSIF/CCU4 slice as ENC_2_OUTA. |
| 58 | MPS_LS1 | Limit switch, MPS channel 1 (stepper/servo) | To MCU (input) | Open-drain, 10k pullup. Should route through XMC4500's ERU as a hardware event input for fast/reliable trip response, not polled GPIO — needs an ERU0/ERU1-capable pin. |
| 59 | MPS_LS2 | Limit switch, MPS channel 2 (stepper/servo) | To MCU (input) | Open-drain, 10k pullup. ERU event input — needs an ERU0/ERU1-capable pin. |
| 60 | MPS_LS3 | Limit switch, MPS channel 3 (stepper/servo) | To MCU (input) | Open-drain, 10k pullup. ERU event input — needs an ERU0/ERU1-capable pin. |
| 61 | MPS_LS4 | Limit switch, MPS channel 4 (stepper/servo) | To MCU (input) | Open-drain, 10k pullup. ERU event input — needs an ERU0/ERU1-capable pin. |
| 62 | MPS_LS5 | Limit switch, MPS channel 5 (stepper/servo) | To MCU (input) | Open-drain, 10k pullup. ERU event input — needs an ERU0/ERU1-capable pin. |
| 63 | MPS_LS6 | Limit switch, MPS channel 6 (stepper/servo) | To MCU (input) | Open-drain, 10k pullup. ERU event input — needs an ERU0/ERU1-capable pin. |
| 64 | MPS_A | RC servo, MPS channel 1 | From MCU (output) | Servo PWM, 50-100Hz. Needs a CCU4/CCU8 timer-capable pin. |
| 65 | MPS_B | RC servo, MPS channel 2 | From MCU (output) | Servo PWM, 50-100Hz. Needs a CCU4/CCU8 timer-capable pin. |
| 66 | MPS_C | RC servo, MPS channel 3 | From MCU (output) | Servo PWM, 50-100Hz. Needs a CCU4/CCU8 timer-capable pin. |
| 67 | MPS_D | RC servo, MPS channel 4 | From MCU (output) | Servo PWM, 50-100Hz. Needs a CCU4/CCU8 timer-capable pin. |
| 68 | MPS_E | RC servo, MPS channel 5 | From MCU (output) | Servo PWM, 50-100Hz. Needs a CCU4/CCU8 timer-capable pin. |
| 69 | MPS_F | RC servo, MPS channel 6 | From MCU (output) | Servo PWM, 50-100Hz. Needs a CCU4/CCU8 timer-capable pin. |
| 70 | VADC_P_OUT | Heater NTC temperature sensor | To MCU (analog input) | Analog output of an NTC divider network. Needs a VADC-capable pin. Slow-changing signal — no CCU-sync requirement. |
| 71 | VADC_A_OUT | HPS DC motor NTC temperature sensor, motor A | To MCU (analog input) | Analog output of an NTC divider network. Needs a VADC-capable pin. Slow-changing signal — no CCU-sync requirement. |
| 72 | VADC_B_OUT | HPS DC motor NTC temperature sensor, motor B | To MCU (analog input) | Analog output of an NTC divider network. Needs a VADC-capable pin. Slow-changing signal — no CCU-sync requirement. |
| 73 | SPI_MOSI | MainControlBoard | To MCU (input) | Main SPI comms bus to MainControlBoard, XMC4500 operating as SPI slave. Target clock 20-40MHz. Needs a USIC (SPI-Slave) capable pin. Time-sensitive — real-time command/data link, unlike the stepper SPI bus. |
| 74 | SPI_MISO | MainControlBoard | From MCU (output) | Main SPI comms bus, XMC4500 as SPI slave. Same USIC channel as SPI_MOSI/SCK/XMC_CS. |
| 75 | SPI_SCK | MainControlBoard | To MCU (input) | Main SPI comms bus clock, 20-40MHz, XMC4500 as SPI slave. Needs verification against XMC4500's SPI-slave max clock spec once the full datasheet electrical characteristics are available — could not confirm from the reference manual excerpt read this session. |
| 76 | XMC_CS | MainControlBoard | To MCU (input) | Chip select for the XMC4500 as SPI slave, 4.7k pullup. Same USIC channel as SPI_MOSI/MISO/SCK. Must be a hardware-compatible CS pin (native USIC SELx input), not software/GPIO-bit-banged — at 20-40MHz, software CS handling would be far too slow. |
| 77 | UART_TX | Debug UART interface | From MCU (output) | Debug UART. Needs a USIC (UART-capable) pin. Not time-critical — low priority for pin resource contention. |
| 78 | UART_RX | Debug UART interface | To MCU (input) | Debug UART. Same USIC channel as UART_TX. Not time-critical. |
| 79 | TMS | J_DEBUG (FTSH-110-01-L-DV-K-TR) | To MCU (input) | SWD/JTAG debug. Fixed-function dedicated pin, not GPIO-mux'd. Connector pinout confirmed against the XMC4500 Debug and Trace System chapter §27.6.2 (Figure 27-4, standard 20-pin Cortex Debug+ETM connector) — schematic matches exactly. |
| 80 | TCK | J_DEBUG (FTSH-110-01-L-DV-K-TR) | To MCU (input) | SWD/JTAG debug clock. Fixed-function dedicated pin. |
| 81 | TDO | J_DEBUG (FTSH-110-01-L-DV-K-TR) | From MCU (output) | JTAG data out / SWO. GPIO alternate function (`DB.TDO/TRACESWO`), not a dedicated silicon pin — competes for a regular port pin, unlike TMS/TCK. Assigned P2.1 in `PinoutSuggestion.md`. |
| 82 | TDI | J_DEBUG (FTSH-110-01-L-DV-K-TR) | To MCU (input) | JTAG data in. GPIO alternate function (`DB.TDI`), competes for a regular port pin. Assigned P0.7 in `PinoutSuggestion.md`. |
| 83 | TRACECLK | J_DEBUG (FTSH-110-01-L-DV-K-TR) | From MCU (output) | ETM trace clock. GPIO alternate function (`DB.ETM_TRACECLK`), competes for a regular port pin. Assigned P2.14 in `PinoutSuggestion.md`. |
| 84 | TRACEDATA0 | J_DEBUG (FTSH-110-01-L-DV-K-TR) | From MCU (output) | ETM trace data bit 0. GPIO alternate function. Assigned P2.13 in `PinoutSuggestion.md`. |
| 85 | TRACEDATA1 | J_DEBUG (FTSH-110-01-L-DV-K-TR) | From MCU (output) | ETM trace data bit 1. GPIO alternate function. Assigned P2.12 in `PinoutSuggestion.md`. |
| 86 | TRACEDATA2 | J_DEBUG (FTSH-110-01-L-DV-K-TR) | From MCU (output) | ETM trace data bit 2. GPIO alternate function. Assigned P2.11 in `PinoutSuggestion.md`. |
| 87 | TRACEDATA3 | J_DEBUG (FTSH-110-01-L-DV-K-TR) | From MCU (output) | ETM trace data bit 3. GPIO alternate function. Assigned P2.10 in `PinoutSuggestion.md`. |
| 88 | GNDDetect | J_DEBUG (FTSH-110-01-L-DV-K-TR) | To MCU (input) | Detects whether a programmer/debug probe is present — driven low by the tool when plugged in. Per §27.6.2: requires a weak pull-up on the PCB. |
| 89 | Key | J_DEBUG (FTSH-110-01-L-DV-K-TR) | N/A | Resolved: per §27.6.2, this is a purely mechanical keying position (Infineon explicitly recommends NOT populating this pin on the connector) — not an electrical signal, no MCU connection needed. |
| 90 | POWER_ENABLE | MainControlBoard | To MCU (input) | Global POWER_ENABLE — primary hardware permission for power delivery, per `README.md` §5 Safety Features. Sourced from the MainControlBoard's STM32 supervisor, which deasserts it on wireless link loss or on a communication timeout between itself and the XMC4500. Confirmed 2026-09-04: gated in hardware directly to every power stage (HPS, Winch, Heater, LPS, steppers) except RC servos — servos are excluded because they're pure PWM outputs with no local HardwareControlBoard power stage to gate (externally powered from the 6V servo rail, not switched by this board). XMC's own connection to this signal is for status/notification only, not enforcement — the hardware cutoff does not depend on XMC noticing or reacting, which matters specifically because XMC being unresponsive is one of the scenarios this signal exists to catch. Downgraded from the earlier ERU-routing plan to plain polled `DIGITAL_IO` — no ERU channel needed. Note: STM32 can also hard-reset XMC directly in an emergency via a separate signal, XMC_RST. |

