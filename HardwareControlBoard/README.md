# HardwareControlBoard – Power Electronics & Real-Time Motion Control

Power electronics and real-time motion control board developed for the `TrinityTrack6000` embedded robotics platform.

The board integrates high-power motor drivers, medium- and low-power outputs, stepper motor interfaces, encoder inputs, current sensing, and onboard power conversion into a single embedded control platform.

## Overview

The `HardwareControlBoard` is the dedicated power electronics and motion control board of the **TrinityTrack6000** platform.

It is responsible for driving the vehicle propulsion motors, winch motor, auxiliary loads, and external actuators while providing current measurement, encoder interfaces, safety monitoring, and communication with the system controller.

The board integrates multiple power stages, dedicated motor gate drivers, stepper motor interfaces, analog measurement circuitry, and onboard power conversion into a single hardware platform designed for embedded robotics applications.

The `HardwareControlBoard` is controlled by an **Infineon XMC4500** microcontroller, which performs all real-time control tasks including PWM generation, current acquisition, encoder processing, and safety supervision. High-level commands are received from the MainControlBoard, allowing the `HardwareControlBoard` to operate as a dedicated real-time motion control subsystem.

Detailed electrical, functional, and thermal information is provided in the accompanying project documentation.

## Key Features

- Infineon XMC4500 real-time controller
- Two high-power H-bridges for propulsion motors
- One medium-power H-bridge for winch motor
- Heater and auxiliary power outputs
- Six external stepper motor interfaces
- Eight low-power outputs
- Differential encoder interfaces (RS-422)
- Current sensing for all power stages
- Onboard power conversion
- Hardware safety supervision and diagnostics

## Design Philosophy

Although originally developed for the `TrinityTrack6000` platform, the `HardwareControlBoard` was designed with modularity and hardware reuse as primary goals.

The board is organized into independent functional blocks, allowing individual subsystems to be reused or adapted for other embedded applications. Generic power stages, configurable interfaces, and standardized signal routing make the design suitable for a wide range of motion-control and industrial automation systems.

The hardware architecture also supports adaptation to different supply configurations through component selection and power-stage configuration, enabling the same platform to be reused across multiple system variants.

## Technical Overview

This section provides a high-level overview of the `HardwareControlBoard` architecture, describing the major functional subsystems and their roles within the overall design. It serves as an introduction to the board's hardware organization before presenting the detailed design documentation.

### 1. System Architecture

The `HardwareControlBoard` is organized around a modular architecture that separates power electronics, signal acquisition, communication interfaces, and safety functions into independent functional blocks.

At the core of the system is an **Infineon XMC4500** microcontroller responsible for all real-time control tasks, including PWM generation, encoder processing, current acquisition, fault monitoring, and communication with the `MainControlBoard`.

The board is divided into dedicated hardware subsystems, including High Power, Medium Power, and Low Power stages, analog measurement circuitry, power conversion, communication interfaces, and safety logic. This separation simplifies development, improves maintainability, and enables individual subsystems to be reused across different hardware configurations.

The following block diagram illustrates the overall organization of the `HardwareControlBoard`.

![HardwareControlBoard System Architecture](/HardwareControlBoard/Media/HardwareControlBoard_BlockDiagram.drawio.png)

### 2. Power Architecture

The `HardwareControlBoard` is built around a modular power architecture that separates loads according to their power requirements and electrical characteristics. This approach simplifies system integration, improves scalability, and allows individual power stages to be reused in different applications.

The board is divided into three primary power domains:

- **High Power Stage (HPS)** – dedicated H-bridges for the propulsion motors, designed to deliver high continuous current with integrated current sensing and hardware protection.
- **Medium Power Stage (MPS)** – power outputs for medium-current loads such as the winch motor and glycerin heater, supporting PWM control and load monitoring.
- **Low Power Stage (LPS)** – multiple low-side switching channels intended for auxiliary loads, external peripherals, and general-purpose outputs.

Each power stage operates as an independent functional module with dedicated gate drivers, protection circuitry, and analog feedback, while sharing a common control interface with the XMC4500 microcontroller.

The overall power architecture is designed to support future hardware revisions through component selection and configuration changes without requiring significant modifications to the system architecture.

### 3. Communication Interfaces

The `HardwareControlBoard` provides multiple communication interfaces for interaction with the MainControlBoard, external sensors, actuators, and peripheral devices. The communication architecture combines high-speed digital interfaces with dedicated motion-control and industrial signaling standards to support reliable real-time operation.

The primary system interface connects the `HardwareControlBoard` to the `MainControlBoard`, through which high-level control commands, operating modes, and diagnostic information are exchanged. The **XMC4500** executes all real-time control algorithms locally while remaining synchronized with the central system controller.

Dedicated communication interfaces are also provided for external devices, including differential RS-422 encoder inputs for robust position and speed feedback, STEP/DIR outputs for external stepper motor drivers, PWM outputs for servo control, and general-purpose digital I/O for auxiliary peripherals.

The communication architecture is designed to minimize latency, improve noise immunity in electrically demanding environments, and provide a standardized interface for future system expansion.


                    MainControlBoard
                           │
                   Communication Bus
                           │
                    XMC4500 Controller
        ┌──────────────┼───────────────┐
        │              │               │
    RS-422         STEP/DIR         PWM/GPIO
    Encoders       Stepper Drivers  Servos & I/O


### 4. Analog Measurement

The `HardwareControlBoard` incorporates a dedicated analog measurement subsystem responsible for monitoring the electrical and thermal state of the power electronics.

Current measurement is implemented for the High Power Stage (HPS) and Medium Power Stage (MPS), enabling high-bandwidth closed-loop control based on cascaded PID algorithms. The acquired current feedback is also used for load supervision, fault detection, and overcurrent protection.

Temperature monitoring is used to supervise critical system components, including the PCB, propulsion motors, and the glycerin heater. These measurements enable thermal protection, derating, and continuous monitoring of the operating conditions.

All analog signals are conditioned by dedicated analog front-end circuitry and sampled by the XMC4500 integrated ADC, where they are processed by the real-time control and safety software.

### 5. Safety Features

The `HardwareControlBoard` implements a multi-layer safety architecture combining hardware protection mechanisms with real-time software supervision to ensure safe operation of the power electronics under both normal and fault conditions.

Power stages are controlled through a dedicated hardware enable architecture. A global POWER_ENABLE signal provided by the `MainControlBoard` serves as the primary hardware permission for power delivery, while each individual power stage implements its own local enable control. Following power-up or reset, all outputs remain disabled until explicitly enabled by the control software.

The controller continuously supervises current and temperature measurements from the High Power Stage (HPS) and Medium Power Stage (MPS), as well as the temperatures of the propulsion motors, PCB, and glycerin heater. These measurements are used for overload protection, thermal supervision, fault detection, and safe shutdown when operating limits are exceeded.

In addition to analog supervision, the `HardwareControlBoard` monitors hardware fault inputs and communication with the `MainControlBoard`. Communication loss or detection of a critical fault causes the controller to transition all power stages into a predefined safe state, preventing unintended actuator operation.

### 6. Power Conversion

The `HardwareControlBoard` includes a dedicated buck converter that generates the supply rail for the gate driver circuitry from the main input voltage.

By providing a stable and independent gate-driver supply, the converter ensures reliable MOSFET switching across the supported operating voltage range while isolating the gate-drive circuitry from variations on the main power input.

The power conversion stage is optimized for high efficiency, low noise, and reliable operation under the dynamic load conditions generated by the switching power stages.

### 7. External Interfaces

The `HardwareControlBoard` provides dedicated external interfaces for power distribution, actuator control, feedback acquisition, communication, programming, and debugging. The interfaces are designed to support reliable operation in electrically demanding environments while simplifying system integration and future expansion.

Power interfaces provide connections for the main supply and high-current loads, while dedicated signal connectors support incremental encoders, external stepper motor drivers, RC servos, temperature sensors, and auxiliary peripherals.

Communication with the `MainControlBoard` is performed through a dedicated system interface, complemented by programming and debug connectors for firmware development, diagnostics, and hardware validation.

The connector layout follows a modular organization, separating high-power, analog, and digital interfaces to improve signal integrity, simplify wiring, and facilitate maintenance.

## 8. Electrical Design

The electrical design of the `HardwareControlBoard` follows a modular architecture that separates power electronics, analog measurement, communication interfaces, and safety functions into dedicated subsystems. This approach simplifies hardware development, improves maintainability, and enables individual functional blocks to be reused across future hardware revisions.

Each power stage is implemented as an independent electrical subsystem with dedicated gate drivers, protection circuitry, and analog feedback. Standardized interfaces and consistent design practices are applied throughout the board to maximize reliability, simplify debugging, and facilitate future expansion.

Particular attention has been given to signal integrity, power distribution, electromagnetic compatibility (EMC), thermal performance, and hardware safety. The electrical architecture has been developed to support reliable operation in electrically demanding environments while providing the flexibility required for future platform variants.

The complete electrical design documentation is available below:

| Document | Description |
|----------|-------------|
| 📄 **Schematic** | [`PCB/HardwareControlBoard.pdf`](PCB/) |
| ⚡ **Electrical Requirements** | [`docs/ElectricalRequirements.md`](docs/ElectricalRequirements.md) |
| 📋 **Functional Requirements** | [`docs/FunctionalRequirements.md`](docs/FunctionalRequirements.md) |
| 🔥 **Thermal Analysis** | [`docs/ThermalBudget.md`](docs/ThermalBudget.md) |
| 📦 **Bill of Materials (BOM)** | [`PCB/BOM.csv`](PCB/BOM_HardwareControlBoard_2026-07-25.csv) |

### 9. PCB Design

The `HardwareControlBoard` PCB has been designed with a strong focus on power integrity, signal integrity, thermal performance, manufacturability, and long-term reliability.

Special attention has been given to the separation of high-current power stages, analog measurement circuitry, and high-speed digital interfaces. The PCB layout follows established power electronics design practices to minimize switching noise, reduce electromagnetic interference (EMI), and improve thermal dissipation.

The current hardware revision is implemented as a four-layer PCB with dedicated routing strategies for power distribution, ground return paths, and signal integrity. Thermal management is achieved through extensive copper pours, thermal via arrays, and optimized component placement.

### PCB Images

*Front View*

![HardwareControlBoard Front](Media/PCB_Front.png)

*Back View*

![HardwareControlBoard Back](Media/PCB_Back.png)

*3D Render*

![HardwareControlBoard 3D](Media/PCB_3D.png)

### PCB Design Documentation

| Document | Description |
|----------|-------------|
| 📄 **Schematic** | [`PCB/HardwareControlBoard.pdf`](PCB/) |
| ⚡ **Electrical Requirements** | [`docs/ElectricalRequirements.md`](docs/ElectricalRequirements.md) |
| 📋 **Functional Requirements** | [`docs/FunctionalRequirements.md`](docs/FunctionalRequirements.md) |
| 🔥 **Thermal Analysis** | [`docs/ThermalBudget.md`](docs/ThermalBudget.md) |
| 📦 **Bill of Materials (BOM)** | [`PCB/BOM.csv`](PCB/BOM_HardwareControlBoard_2026-07-25.csv) |
| 🛠️ **Assembly Notes** | [`docs/AssemblyNotes.md`](docs/AssemblyNotes.md) |

## 10. Software

The `HardwareControlBoard` firmware is implemented using a bare-metal architecture, allowing direct access to the hardware peripherals and deterministic real-time execution.

The firmware makes extensive use of the **XMC4500** peripheral set, including timers, PWM generation, ADCs, DMA, interrupts, and dedicated motor-control peripherals. This hardware-oriented approach minimizes software overhead while providing precise timing, low interrupt latency, and predictable system behavior required for real-time motor control.

The software architecture is organized into modular drivers and application layers to simplify maintenance, validation, and future feature expansion.

The complete software architecture and firmware documentation are available in the following document:

| Document | Description |
|----------|-------------|
| 💻 **Software Architecture** | [`docs/SoftwareArchitecture.md`](docs/SoftwareArchitecture.md) |

