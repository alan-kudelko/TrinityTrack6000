# Functional Requirements

## 1. Purpose

This document defines the functional requirements of the `HardwareControlBoard`.

Its purpose is to specify the intended functionality, operational responsibilities, and external interfaces of the board independently of the final hardware implementation.

The requirements defined in this document serve as the primary input for the electrical architecture, hardware design, firmware development, PCB implementation, and system verification activities.

Unless explicitly stated otherwise, the requirements described herein are implementation-independent and remain valid regardless of the selected electronic components or future hardware revisions.

## 2. Scope

This document defines the functional requirements of the `HardwareControlBoard`.

It specifies the responsibilities of the board, the functionality it shall provide, and its external interfaces within the TrinityTrack6000 platform.

Implementation details, electrical architecture, and hardware realization are specified in separate design documents.

## 3. System Overview

The `HardwareControlBoard` is responsible for deterministic control of platform actuators and acquisition of real-time feedback signals.

It executes all time-critical control tasks independently from the `MainControlBoard` while continuously monitoring the electrical and thermal state of the controlled subsystems.

The functional requirements defined in this document serve as the basis for the electrical architecture, hardware design, firmware implementation, and system verification.

## 4. Functional Requirements

The design shall maximize reuse of proven hardware building blocks across all functional subsystems. Where functional requirements are equivalent, identical hardware architectures shall be preferred. Dedicated implementations shall only be introduced when justified by significantly different functional, electrical, thermal, or safety requirements.

The `HardwareControlBoard` shall provide the following functional capabilities:

| Function | Quantity | Description |
|----------|---------:|-------------|
| Brushed DC Motor Control | 2 | Independent closed-loop control of left and right drive motors |
| Winch Motor Control | 1 | Closed-loop control of the winch motor |
| Stepper Motor Control | 6 | STEP/DIR control for external stepper motor drivers |
| RC Servo Control | 6 | Direct PWM control for RC servos using a shared connector architecture |
| Heater Control | 1 | PWM control of the glycerin vaporizer heater |
| Glycerin Pump Control | 1 | PWM control of the smoke generator pump |
| Cooling Fan Control | 2 | PWM control of enclosure cooling fans |
| Current Measurement | Multiple | Current acquisition for all power stages |
| Temperature Measurement | Multiple | Monitoring of H-bridges, motors, heater, and other temperature-critical components |
| Encoder Acquisition | 2 | Quadrature encoder feedback for drive motors |
| Fault Monitoring | Multiple | Detection of hardware faults and driver error signals |
| Level Monitoring | 1 | Glycerin reservoir level detection |
| SPI Communication | 1 | Communication with the `MainControlBoard` |
| Debug Interface | 1 | SWD, SWO, ETM, and UART debugging support |
| External Lighting Control | Multiple | Control of external LED lighting and status indicators |
| Laser Control | 1 | Control of the onboard aiming laser module |

#### 4.1 Low Power Section (LPS)

The Low Power Section (LPS) provides switching and PWM control for auxiliary low-power loads that do not require dedicated power stages or closed-loop control.

The section is intended for general-purpose power outputs used to control peripheral devices and auxiliary equipment throughout the platform.

Typical applications include:

- External lighting
- Indicator lamps
- Laser module
- Auxiliary switched power outputs
- Glycerin pump
- Cooling fans
- Future low-power peripherals

#### 4.2 Medium Power Section (MPS)

The Medium Power Section (MPS) provides dedicated control of medium-power actuators that require application-specific power stages, dedicated control interfaces, or additional monitoring capabilities.

Typical applications include:

- Winch DC motor with current monitoring
- Glycerin vaporizer heater with PWM power control
- External STEP/DIR stepper motor drivers
- RC servo outputs

#### 4.3 High Power Section (HPS)

The High Power Section (HPS) is dedicated to the propulsion system and provides deterministic control of the platform's high-current brushed DC drive motors.

Unlike the Low Power and Medium Power sections, the HPS is designed for high-current motor control with continuous feedback acquisition, closed-loop operation, and integrated protection mechanisms.

Typical functionality includes:

- Independent control of the left and right propulsion motors
- Closed-loop motor current control
- Closed-loop motor velocity control
- Incremental quadrature encoder feedback
- Real-time motor current measurement
- H-bridge temperature monitoring
- Gate driver fault monitoring
- Hardware fault detection and protection

#### 4.4 Analog Voltage Reference

The `HardwareControlBoard` shall provide a dedicated precision voltage reference for all onboard analog measurement circuits.

The reference shall be used exclusively for analog signal acquisition and conditioning to ensure consistent measurement accuracy across all current and temperature sensing channels.

The analog voltage reference shall remain electrically independent from the digital supply rails.

#### 4.5 Configurability

The `HardwareControlBoard` shall support hardware configuration through assembly options and PCB solder bridges where appropriate.

The configurable hardware shall allow adaptation of the board to different application requirements without requiring PCB redesign.

Typical configurable features include:

- Selection between stepper motor and RC servo outputs
- Selection of actuator supply voltage where applicable
- Optional population of application-specific hardware
- Selection of signal routing where supported by the hardware design

#### 4.6 Functional Safety

The `HardwareControlBoard` shall provide mechanisms allowing all controllable power outputs to be placed into a predefined safe state under fault conditions or upon external request.