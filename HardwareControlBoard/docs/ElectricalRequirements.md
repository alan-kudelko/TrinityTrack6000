# Electrical Requirements

## 1. Purpose

This document defines the electrical requirements of the `HardwareControlBoard`.

Its purpose is to establish the electrical characteristics, interfaces, protection mechanisms, and design constraints that shall be satisfied to ensure reliable operation of the board.

The requirements defined in this document serve as the basis for schematic design, component selection, electrical verification, PCB implementation, and future hardware revisions.

## 2. Scope

This document specifies the electrical requirements applicable to the `HardwareControlBoard` within the TrinityTrack6000 platform.

It covers:

- Power architecture and power distribution
- Electrical interfaces
- Analog measurement requirements
- Protection mechanisms
- PCB electrical design requirements

Mechanical design, functional behavior, firmware implementation, software architecture, and PCB layout implementation are specified in separate design documents.

## 3. Electrical Design Philosophy

#### 3.1 BOM-Dependent Supply Voltage

The input voltage capability of the `HardwareControlBoard` shall be determined by the populated Bill of Materials (BOM).

Whenever the BOM is modified, the supported operating voltage range shall be re-evaluated based on the electrical ratings of all populated components.

#### 3.2 Scalable Electrical Architecture

The electrical architecture shall support future hardware variants through component population without requiring major schematic redesign.

#### 3.3 Component Footprint Strategy

Component footprints shall be selected to support electrically compatible devices with higher voltage or current ratings where practical.

Whenever compatible packages are available, future BOM upgrades shall not require PCB redesign.

#### 3.4 Component Standardization

Common electrical functions shall reuse identical or functionally equivalent components wherever practical.

Standardization shall be preferred to reduce design complexity, simplify procurement, improve maintainability, and minimize the number of unique components within the Bill of Materials (BOM).

#### 3.5 Component Derating

Electrical components shall be selected with appropriate voltage, current, power, and thermal margins to ensure reliable long-term operation.

Continuous operation at absolute maximum ratings shall not be considered acceptable.

## 4. Power Distribution

#### 4.1 Supply Voltage

The `HardwareControlBoard` shall support input supply voltages commonly used in industrial and mobile electrical systems through appropriate Bill of Materials (BOM) selection.

The electrical architecture shall allow implementation of hardware variants supporting nominal input voltages from 12 V up to 48 V, provided that all populated components satisfy the required electrical ratings.

#### 4.2 Power Rails

The `HardwareControlBoard` shall generate dedicated supply rails for the onboard functional subsystems.

Separate power rails shall be provided for digital logic, analog circuitry, actuator interfaces, and other application-specific loads where required.

Power rails shall be designed to minimize mutual interference between functional subsystems.

The standard hardware configuration provides:

- 5 V power rail for onboard digital logic and voltage regulation
- 3.3 V power rail generated locally from the 5 V rail for digital and analog circuitry
- 6 V power rail dedicated to RC servo outputs
- 12 V power rail generated locally for gate driver supply

#### 4.3 Power Domains

The electrical system shall be divided into dedicated power domains according to functional requirements.

Each power domain shall be independently decoupled and designed to minimize electrical interference between high-current switching circuits, analog measurement circuits, and digital control logic.

#### 4.4 High Current Distribution

High-current power shall be distributed using dedicated copper bus bars wherever practical.

The use of PCB copper traces as the primary high-current conduction path shall be minimized to reduce resistive losses, voltage drop, and thermal loading of the PCB.

High-current connectors shall be mechanically attached to the corresponding bus bars to minimize current flow through the PCB laminate.

The PCB shall primarily provide electrical interconnection between bus bars and local circuitry rather than serving as the primary high-current conduction path.

#### 4.5 PCB Current Distribution

PCB copper shall primarily provide electrical interconnection between functional blocks and local power distribution.

Where high-current conduction through the PCB is unavoidable, the current path shall be designed to minimize resistance, temperature rise, and current density.

#### 4.6 Grounding Strategy

The grounding system shall provide low-impedance current return paths for both power and signal circuits.

High-current return paths shall be arranged to minimize loop area and reduce conducted and radiated electromagnetic interference (EMI).

Dedicated analog and digital circuitry shall be arranged to minimize coupling from high-current switching circuits.

Power bus bars carrying supply and return currents shall be positioned in close proximity wherever practical to minimize magnetic field generation and current loop area.

#### 4.7 High-Speed Switching

High-current switching loops shall be designed to minimize parasitic inductance, current loop area, and voltage transients.

Component placement and interconnections shall minimize switching current paths wherever practical.

## 5. Analog Measurement Subsystem

#### 5.1 Analog Voltage Reference

The `HardwareControlBoard` shall include a dedicated precision analog voltage reference intended exclusively for onboard analog measurement circuits.

The analog voltage reference shall provide a common measurement baseline for all onboard analog signal conditioning and acquisition circuits.

The analog voltage reference shall remain electrically independent from the digital supply rails wherever practical.

#### 5.2 ADC Requirements

Analog measurement channels shall provide sufficient accuracy, resolution, and sampling capability for closed-loop control, diagnostics, and system monitoring.

ADC reference voltages and input ranges shall be selected to maximize the usable measurement range while maintaining appropriate measurement margins.

Analog measurements shall remain repeatable over the intended operating conditions.

#### 5.3 Current Measurement

The `HardwareControlBoard` shall provide current measurement for all power stages requiring closed-loop control, power regulation, or diagnostic monitoring.

Current sensing circuits shall be designed to provide stable measurements over the complete operating current range.

Where practical, common current measurement architectures shall be reused across multiple functional subsystems.

#### 5.4 Temperature Measurement

Temperature monitoring shall be provided for power electronics where required to ensure safe operation.

Temperature sensors shall be positioned to accurately represent the thermal condition of the monitored subsystem.

Measured temperatures shall be available for diagnostics and protection mechanisms.

#### 5.5 Signal Conditioning

Analog signal conditioning circuits shall preserve measurement accuracy while providing adequate noise immunity.

Signal conditioning shall be designed to minimize the influence of switching noise generated by high-current power stages.

Where practical, identical signal conditioning architectures shall be reused across equivalent measurement channels.

## 6. Digital Interfaces

#### 6.1 MainControlBoard Interface

The `HardwareControlBoard` shall communicate with the `MainControlBoard` through a dedicated SPI interface.

The communication interface shall provide deterministic and reliable data exchange suitable for real-time control applications.

The interface shall maintain reliable operation in the presence of electrical disturbances generated by high-current switching circuits.

#### 6.2 Stepper Motor Interfaces

The `HardwareControlBoard` shall provide dedicated STEP, DIR, and ENABLE control signals for each supported stepper motor channel.

The electrical interface shall be compatible with external STEP/DIR stepper motor drivers.

Signal integrity shall be maintained to ensure reliable operation at the maximum supported pulse frequency.

The interface shall support independent control of each motor channel.

The control interface shall be independent of the internal implementation of the external motor driver.

#### 6.3 RC Servo Interfaces

The `HardwareControlBoard` shall provide dedicated PWM control signals for each supported RC servo channel.

The electrical interface shall be compatible with standard RC servo control interfaces.

Signal integrity shall be maintained to ensure reliable servo operation under all supported operating conditions.

#### 6.4 Incremental Encoder Interfaces

The `HardwareControlBoard` shall provide dedicated electrical interfaces for incremental rotary encoders.

The encoder interfaces shall support reliable position and speed feedback for the controlled drive systems.

The electrical interface shall be compatible with differential encoder signaling.

Signal integrity shall be maintained to ensure reliable operation in electrically noisy environments.

Each encoder interface shall operate independently of the remaining encoder channels.

The standard hardware configuration shall support quadrature encoder signals.

#### 6.5 Home Sensor Interfaces

The `HardwareControlBoard` shall provide dedicated interfaces for home position sensors associated with supported stepper motor axes.

The interfaces shall provide reliable detection of the reference position under all supported operating conditions.

Each home sensor interface shall operate independently of the remaining channels.

#### 6.6 Debug Interfaces

The `HardwareControlBoard` shall provide hardware interfaces for firmware programming, debugging, and system verification.

The debug interface shall support Serial Wire Debug (SWD).

The debug interface shall provide full ARM CoreSight trace capability to enable real-time firmware analysis and advanced debugging.

The debug interface shall remain electrically isolated from high-current power paths wherever practical.

## 7. Safety Architecture

#### 7.1 Controller Supervision

The `HardwareControlBoard` shall monitor communication with the external system controller.

Upon detection of a communication timeout, the `HardwareControlBoard` shall transition all controlled outputs to a predefined safe state by stopping the generation of control signals.

The external system controller shall monitor communication with the `HardwareControlBoard`.

Upon detection of a communication timeout, the external system controller shall be capable of disabling all power stages through an independent hardware safety mechanism.

#### 7.2 Safe Output Control

The `HardwareControlBoard` shall provide an independent hardware mechanism capable of disabling all gate drivers.

The hardware safety mechanism shall operate independently of the real-time motor control firmware.

Activation of the hardware safety mechanism shall transition all controlled power stages into a predefined safe state.

The hardware safety mechanism shall allow the external system controller to disable all gate drivers independently of the real-time control functions executed by the `HardwareControlBoard`.

#### 7.3 Device Protection

The `HardwareControlBoard` shall utilize protection mechanisms integrated within supported devices where available.

Fault indications generated by supported devices shall be made available to the control firmware whenever practical.

#### 7.4 System Energy Protection

Protection against catastrophic electrical faults shall be provided by higher-level system protection mechanisms.

The `HardwareControlBoard` shall operate as part of the overall system safety architecture together with external protection devices where applicable.

The safety architecture shall support coordination with external power conversion and battery management subsystems responsible for electrical fault protection.

## 8. PCB Design Requirements

#### 8.1 Component Placement

Components shall be placed to minimize interconnect length between functionally related circuits.

Power conversion components shall be located to minimize high-current path lengths.

Analog circuitry shall be physically separated from high-current switching circuits wherever practical.

Components shall be placed to facilitate thermal management, assembly, inspection, and servicing.

Components shall be arranged to simplify PCB routing while maintaining electrical performance.

#### 8.2 Power Distribution Layout

High-current power paths shall be designed to minimize electrical resistance, parasitic inductance, and voltage drop.

High-current distribution shall utilize dedicated conductors, such as copper bus bars, wherever practical.

PCB copper shall primarily provide electrical interconnection between power distribution elements and onboard circuitry.

Power distribution shall be arranged to minimize current loop area and electromagnetic emissions.

Power and return current paths shall be routed in close physical proximity wherever practical.

High-current distribution shall be separated from sensitive analog and digital circuitry to minimize electrical interference.

#### 8.3 Grounding

The PCB shall provide low-impedance ground paths for all functional subsystems.

Ground return paths shall be designed to minimize current loop area and electromagnetic interference.

High-current return paths shall be physically separated from sensitive analog circuitry wherever practical.

Grounding shall be implemented to minimize coupling between high-current switching circuits and low-level analog signals.

Ground planes shall provide continuous return paths for high-speed digital signals wherever practical.

The grounding strategy shall support reliable operation of analog measurements, digital communication interfaces, and power electronics.

#### 8.4 Decoupling

Each integrated circuit shall be provided with local high-frequency decoupling capacitors located as close as practical to the corresponding power supply pins.

Power distribution networks shall provide adequate local energy storage to support transient current demands.

Decoupling capacitors shall be connected using low-impedance power and ground connections.

The PCB layout shall minimize parasitic inductance within decoupling current loops.

Separate decoupling shall be provided for independent power domains where required.

Power supply filtering shall be implemented where necessary to maintain stable operation of analog, digital, and power electronic subsystems.

#### 8.5 High-Current Routing

High-current conductors shall be designed to minimize electrical resistance, parasitic inductance, and voltage drop.

High-current routing shall be implemented using conductors appropriate for the expected current levels.

Current bottlenecks shall be avoided throughout all high-current paths.

Current transitions between different conductor types shall be designed to minimize localized current density.

High-current routing shall be designed to minimize thermal hotspots.

Current carrying capacity shall be verified for all high-current conductors under the intended operating conditions.

#### 8.6 Digital Signal Routing

Digital signal routing shall maintain reliable communication under all supported operating conditions.

Digital signals shall be routed to minimize coupling with high-current switching circuits.

Critical digital interfaces shall be provided with continuous return current paths wherever practical.

Signal routing shall minimize unnecessary trace length and discontinuities.

Where practical, debug and communication interfaces shall be routed to facilitate reliable programming, debugging, and system verification.

#### 8.7 Analog Signal Routing

Analog signal paths shall be routed to minimize susceptibility to electrical noise.

Analog signals shall be routed with continuous, low-impedance ground reference paths wherever practical.

Analog signal routing shall maintain adequate separation from high-current and high-speed switching circuits.

Signal conditioning and filtering shall be provided where required to ensure reliable analog measurements.

Analog signal routing shall minimize parasitic coupling, impedance discontinuities, and unnecessary trace length.

The PCB layout shall support the accuracy and repeatability of all analog measurement subsystems.

#### 8.8 Thermal Management

Heat-generating components shall be arranged to promote uniform thermal distribution across the PCB wherever practical.

High-power components shall be physically separated where practical to reduce localized thermal accumulation.

The PCB shall be designed to maximize the effective use of its available area for heat dissipation.

Thermal conduction through PCB copper shall be utilized to distribute heat away from high-power components.

Component placement shall facilitate adequate cooling under the intended operating conditions.

#### 8.9 Testability

The PCB shall provide dedicated test points for critical signals required during manufacturing, validation, and debugging.

Test points shall be provided for critical analog measurement nodes where practical.

Test points shall be provided for critical digital control signals where practical.

Test points shall be accessible without requiring modification of the PCB assembly.

Signal routing shall facilitate electrical verification of onboard circuits that are not directly accessible through external connectors.

The PCB layout shall support efficient fault diagnosis during development and production testing.