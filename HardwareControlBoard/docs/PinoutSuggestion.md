# Pinout Suggestion

## 1. Purpose

This document proposes an initial pin assignment for all 89 signals in `docs/SignalList.md` (Key excluded — resolved as not populated) against the XMC4500's actual peripheral multiplexing, sourced from the Port I/O Functions table (Reference Manual §25.10.1, `Infineon_XMC4500_Datasheet_Pins.pdf`) read this session.

This is a **starting point for DAVE IDE, not a final assignment.** Confidence varies by signal group — see the flags in each table. Cross-check every entry in DAVE before committing to schematic pin numbers.

## 2. Scope

This document covers pin candidates only. It does not cover schematic net naming, board routing, or firmware pin-configuration code. Component designators and signal definitions are in `docs/SignalList.md`; that document remains the source of truth for *what* each signal is — this one is only a proposal for *where*.

## 3. Resource Budget Summary

| Resource | Capacity | Demand | Margin |
|---|---|---|---|
| CCU4 slices (single PWM) | 16 (CCU40-43 × 4 slices) | 16 (LPS 4 + MPS_STEP 6 + MPS servo 6) | **0 — saturated** |
| CCU8 slices (complementary pair) | 8 (CCU80/81 × 4 slices) | 7 (3 H-bridges × 2 legs = 6, + Heater = 1) | 1 spare |
| POSIF units | 2 | 2 (2 encoders) | 0 — exact fit |
| ERU channels | 8 (ERU0/1 × 4 channels) | 8 (DRV_FAULT, U2_PGOOD, MPS_LS1-6) | 0 — exact fit. POWER_ENABLE downgraded to plain polled `DIGITAL_IO` (hardware-gated independent of XMC); U2_PGOOD reclassified onto ERU in its place — see `SignalList.md` |
| VADC groups | 4 (G0-G3) | 4 sync-critical channels, one per group | 0 spare groups, but 11/26 analog pins used overall |
| USIC channels | 6 (U0C0/1, U1C0/1, U2C0/1) | 3 (stepper SPI, MainControlBoard SPI, UART) | comfortable |

**Bottom line (unchanged from the earlier discussion): CCU4 and ERU are fully saturated. There is no spare capacity for the U7/U8 LPS GPIO→PWM upgrade contingency, or for any additional fault/limit input discovered later.**

## 4. Confirmed DAVE Assignments (pinout order)

These are pins actually assigned in DAVE, not speculative candidates. Roughly grouped by section in the order confirmed rather than strict port/pin order. Growing as assignments are confirmed.

| # | Pin | Signal | Peripheral | Instance | Notes |
|---|---|---|---|---|---|
| 1 | P6.1 | HPSA LEFT HIGH | CCU8 | GLOBAL_CCU8_0 | Direct out — complementary pair with P6.0 |
| 2 | P6.0 | HPSA LEFT LOW | CCU8 | GLOBAL_CCU8_0 | Inverted out — complementary pair with P6.1 |
| 3 | P5.1 | HPSA RIGHT HIGH | CCU8 | GLOBAL_CCU8_0 | Direct out — complementary pair with P5.0 |
| 4 | P5.0 | HPSA RIGHT LOW | CCU8 | GLOBAL_CCU8_0 | Inverted out — complementary pair with P5.1 |
| 5 | P1.15 | HPSB LEFT HIGH | CCU8 | GLOBAL_CCU8_0 | Direct out — complementary pair with P1.12 |
| 6 | P1.12 | HPSB LEFT LOW | CCU8 | GLOBAL_CCU8_0 | Inverted out — complementary pair with P1.15 |
| 7 | P5.7 | HPSB RIGHT HIGH | CCU8 | GLOBAL_CCU8_0 | Direct out — complementary pair with P5.6 |
| 8 | P5.6 | HPSB RIGHT LOW | CCU8 | GLOBAL_CCU8_0 | Inverted out — complementary pair with P5.7 |
| 9 | P3.14 | SPI_MPS_MISO | USIC | U1C1 | Stepper SPI master. Hardware CS omitted — 6 slaves, per-slave CS via GPIO instead |
| 10 | P3.15 | SPI_MPS_MOSI | USIC | U1C1 | Stepper SPI master |
| 11 | P0.13 | SPI_MPS_SCK | USIC | U1C1 | Stepper SPI master |
| 12 | P4.0 | SPI_MOSI | USIC | U2C1 | MainControlBoard SPI slave |
| 13 | P3.5 | SPI_MISO | USIC | U2C1 | MainControlBoard SPI slave |
| 14 | P4.2 | SPI_SCK | USIC | U2C1 | MainControlBoard SPI slave |
| 15 | P4.1 | XMC_CS | USIC | U2C1 | MainControlBoard SPI slave. Hardware SELx input |
| 16 | P2.14 | TRACECLK | ETM | — | Debug/Trace connector |
| 17 | P2.13 | TRACEDATA0 | ETM | — | Debug/Trace connector |
| 18 | P2.12 | TRACEDATA1 | ETM | — | Debug/Trace connector |
| 19 | P2.11 | TRACEDATA2 | ETM | — | Debug/Trace connector |
| 20 | P2.10 | TRACEDATA3 | ETM | — | Debug/Trace connector |
| 21 | P15.8 | HEATER_CURRENT | VADC | GLOBAL_ADC_0 (ADC_MEASUREMENT_HEATER) | An earlier draft had this pin as HPSB current sense — reassigned here. Group/channel not yet confirmed from DAVE |
| 22 | P14.8 | WINCH_CURRENT | VADC | GLOBAL_ADC_0 (ADC_MEASUREMENT_WINCH) | An earlier draft had Winch current sense at P14.0 — reassigned here. Group/channel not yet confirmed from DAVE |
| 23 | P14.0 | ADC_MEASUREMENT_HPSA | VADC | GLOBAL_ADC_0 (ADC_MEASUREMENT_HPSA) | Sync-critical current sense |
| 24 | P14.1 | ANALOG_HPS_2_TEMP_OUT | VADC | GLOBAL_ADC_0 (ADC_MEASUREMENT_0) | Non-sync-critical, basic ADC_MEASUREMENT. Channel_A-G letter not yet given |
| 25 | P14.4 | ADC_MEASUREMENT_HPSB | VADC | GLOBAL_ADC_0 (ADC_MEASUREMENT_HPSB) | Sync-critical current sense |
| 26 | P14.9 | ANALOG_MPS_2_TEMP_OUT | VADC | GLOBAL_ADC_0 (ADC_MEASUREMENT_0) | Non-sync-critical, basic ADC_MEASUREMENT. Channel_A-G letter not yet given |
| 27 | P15.12 | ANALOG_HPS_1_TEMP_OUT | VADC | GLOBAL_ADC_0 (ADC_MEASUREMENT_0) | Non-sync-critical, basic ADC_MEASUREMENT. Channel_A-G letter not yet given |
| 28 | P14.7 | POSIF_HPSA_PHASE_A | POSIF | POSIF0 (inferred) | Confirmed accepted in DAVE — P14.x doing double duty as a digital POSIF input despite being nominally analog-only |
| 29 | P14.6 | POSIF_HPSA_PHASE_B | POSIF | POSIF0 (inferred) | Confirmed accepted in DAVE — same double-duty pin note as above |
| 30 | P2.5 | POSIF_HPSB_PHASE_A | POSIF | POSIF1 (inferred) | |
| 31 | P2.4 | POSIF_HPSB_PHASE_B | POSIF | POSIF1 (inferred) | |

## 5. Pending / Not Yet Decided

Pin-to-signal mapping not final — tracked as two separate pools instead of paired rows, since which PWM signal lands on which pin/section hasn't been decided yet. 20 pins reserved via DAVE vs. 22 signals still needing one — 2 short, consistent with the P5.11 and P5.9 collisions that prompted this restructure.

### 5a. Available pins (reserved in DAVE, not yet bound to a signal)

| # | Pin | Peripheral | Instance | Notes |
|---|---|---|---|---|
| 1 | P0.4 | CCU8 | GLOBAL_CCU8_0 | |
| 2 | P0.9 | CCU8 | GLOBAL_CCU8_0 | |
| 3 | P0.10 | CCU8 | GLOBAL_CCU8_1 | |
| 4 | P1.1 | CCU4 | GLOBAL_CCU4_0 | Shares module with POSIF0 |
| 5 | P2.7 | CCU8 | GLOBAL_CCU8_1 | |
| 6 | P2.8 | CCU8 | GLOBAL_CCU8_1 | |
| 7 | P2.9 | CCU8 | GLOBAL_CCU8_1 | |
| 8 | P3.0 | CCU4 | GLOBAL_CCU4_3 | |
| 9 | P3.3 | CCU4 | GLOBAL_CCU4_3 | |
| 10 | P3.4 | CCU4 | GLOBAL_CCU4_3 | |
| 11 | P3.10 | CCU4 | GLOBAL_CCU4_1 | Shares module with POSIF1 |
| 12 | P3.13 | CCU4 | GLOBAL_CCU4_3 | |
| 13 | P4.3 | CCU4 | GLOBAL_CCU4_2 | |
| 14 | P4.4 | CCU4 | GLOBAL_CCU4_2 | |
| 15 | P5.3 | CCU8 | GLOBAL_CCU8_0 | |
| 16 | P5.8 | CCU8 | GLOBAL_CCU8_1 | |
| 17 | P5.9 | CCU8 | GLOBAL_CCU8_1 | |
| 18 | P5.11 | CCU8 | GLOBAL_CCU8_1 | |
| 19 | P6.4 | CCU4 | GLOBAL_CCU4_2 | |
| 20 | P6.5 | CCU4 | GLOBAL_CCU4_2 | |

### 5b. PWM signals left to assign

| Signal | Pins needed | Peripheral | Notes |
|---|---|---|---|
| WINCH | 4 (2 complementary pairs) | CCU8 | GLOBAL_CCU8_1. Leg/side (which pin = which side/polarity) not yet decided |
| HEATER PWM | 1 | CCU8 | Single-ended |
| MPS_A | 1 | CCU8 | Single-ended |
| MPS_B | 1 | CCU8 | Single-ended |
| MPS_C | 1 | CCU8 | Single-ended |
| MPS_D | 1 | CCU8 | Single-ended |
| MPS_E | 1 | CCU4 | Single-ended |
| MPS_F | 1 | CCU4 | Single-ended |
| MPS_STEP1-6 | 6 | CCU4 | Single-ended, one pin each |
| LPS1-3 | 3 | CCU8 | Single-ended, one pin each |
| LPS4-5 | 2 | CCU4 | Single-ended — share slices with POSIF0/POSIF1 |

## 6. Pending — VADC Temperature Channels

Same split as §5 — reserved pins and remaining signal names tracked separately since the pin-to-signal mapping isn't final. Part of the 7-channel basic `ADC_MEASUREMENT_0` block (§4 rows 24/26/27 are the other 3 of the 7, already named and pinned).

### 6a. Available pins (reserved in DAVE, not yet bound to a signal)

| # | Pin | Peripheral | Instance | Notes |
|---|---|---|---|---|
| 1 | P14.12 | VADC | GLOBAL_ADC_0 (ADC_MEASUREMENT_0) | |
| 2 | P14.13 | VADC | GLOBAL_ADC_0 (ADC_MEASUREMENT_0) | |
| 3 | P14.14 | VADC | GLOBAL_ADC_0 (ADC_MEASUREMENT_0) | |
| 4 | P14.15 | VADC | GLOBAL_ADC_0 (ADC_MEASUREMENT_0) | |

### 6b. Temperature/NTC signals left to assign

| Signal | Pins needed | Peripheral | Notes |
|---|---|---|---|
| MPS_1_TEMP | 1 | VADC | Non-sync-critical, basic ADC_MEASUREMENT |
| Heater NTC | 1 | VADC | Non-sync-critical, basic ADC_MEASUREMENT — name TBC (an earlier draft called it VADC_P_OUT) |
| Motor A NTC | 1 | VADC | Non-sync-critical, basic ADC_MEASUREMENT — name TBC (an earlier draft called it VADC_A_OUT) |
| Motor B NTC | 1 | VADC | Non-sync-critical, basic ADC_MEASUREMENT — name TBC (an earlier draft called it VADC_B_OUT) |
