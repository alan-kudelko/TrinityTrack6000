# **HardwareControlBoard** – **Power Electronics & Motion Control**

**Infineon XMC4000 Family**: Dedicated real-time controller responsible for deterministic control of motors, servos and other actuators, as well as supervision of power electronics.

The board serves as the low-level hardware control layer of the system, executing all time-critical control tasks independently from the MainControlBoard.

- The firmware is developed using a bare-metal architecture with hardware-assisted peripherals (DMA, CCU4/CCU8, interrupts).
- Designed to interface directly with motors, servos, power electronics and feedback sensors
- Provides a hardware abstraction layer between the MainControlBoard and the physical actuators

All tasks are allocated using **fully static memory allocation**, with **guard zones** implemented via custom **linker scripts**. The firmware is designed for deterministic execution and predictable timing under all operating conditions.

The controller includes dedicated **safety and diagnostic mechanisms**, including:

- Motor and actuator diagnostics
- Current and temperature monitoring
- Hardware fault detection
- External watchdog supervision
- Safe enable / disable handling

The project follows **MISRA C:2025** guidelines to ensure code safety, maintainability and portability.

> 🔄 **Status:** Requirements definition and architecture design  
> 🔧 **Goal:** Develop a dedicated real-time controller for power electronics and motion control, while exploring deterministic embedded software architecture, safety mechanisms and industrial motor control techniques.

---

## ✳️ Planned Technologies & Tools

- **MCU:** Infineon XMC4500
- **Architecture:** Bare-metal
- **Memory Management:** Fully static memory allocation
- **Interfaces:** SPI, I²C, UART
- **Motion Control:** DC motors, servos, future stepper motor support
- **Feedback:** Incremental encoders, current sensing, temperature monitoring
- **Development Tools:** VS Code, CMake, Ninja, arm-gcc toolchain, DAVE IDE
- **Software Libraries:** Infineon low-level drivers
- **Diagnostics & Monitoring:** Runtime diagnostics, watchdog supervision, fault handling

## Development Strategy

- **Architecture Phase:**  
  Define hardware architecture, communication interfaces and safety mechanisms.

- **Firmware Bring-up:**  
  Validate peripherals, communication interfaces and RTOS infrastructure using an Infineon evaluation board.

- **Production Phase:**  
  Implement the final firmware on a dedicated **HardwareControlBoard**, responsible for deterministic motion control and power electronics supervision.

## Key Features

- Deterministic bare-metal execution
- Modular hardware abstraction
- Separation of high-level coordination and low-level control
- Scalable architecture for multiple actuators
- Hybrid approach: RTOS with hardware-assisted peripherals (DMA, timers, interrupts)

### 🔌 Hardware & Electronics

- **Infineon XMC4500 MCU**
- H-bridge interface
- DC motor control
- Servo control
- Incremental encoder interface
- Current sensing
- Temperature monitoring
- External watchdog
- Cortex-M Debug & Trace connector
- Diagnostic LEDs
- Hardware fault inputs
- ENABLE / KILL interface
- SPI communication with MainControlBoard

### 🧠 System Architecture & Concepts

- Bare-metal firmware
- Hardware-driven architecture
- Static memory allocation only
- Event-driven processing using interrupts
- Hardware-assisted peripherals (DMA, CCU4, CCU8, VADC)
- Dedicated safety layer

### 🧰 Development Tools & Libraries

- **C**
- Native Infineon low-level drivers
- **VS Code, CMake, Ninja**
- **Doxygen**
- **DAVE IDE / XMC Pinout Tool**

### 📚 Documentation

The firmware and hardware design are documented using **Doxygen** and dedicated architecture documentation, including system diagrams, flow diagrams and PCB revision logs.

---

## 🧠 Design Goals

- 💾 Use 100% static memory allocation (no malloc, no heap)
- ⚡ Utilize hardware peripherals whenever possible to minimize CPU overhead.
- 🎯 Achieve deterministic execution for all time-critical control tasks.
- 🛡️ Implement hardware-oriented safety mechanisms and fault handling
- 🔁 Provide reliable communication with the MainControlBoard
- 🧱 Build a modular control platform for motors, servos and future actuators
- 📚 Explore industrial real-time control techniques and embedded safety concepts
- 🔧 Simplify debugging through comprehensive diagnostics and trace support
- 🌐 Create a reusable motion-control platform for future embedded projects

---

## Technical Table of Contents

1. [Module Structure](#1--module-structure)
2. [System Architecture](#-2-system-architecture)
   - [2.1 XMC4500 (Real-Time Controller)](#21-xmc4500-real-time-controller)
   - [2.2 Module Block Diagram](#22-module-block-diagram)

---

## ⚙️ Technical Overview 

### 1. 📁 Module Structure

The module is organized around firmware, hardware design files, and supporting documentation.

* `XMC4500_F144/` – firmware for the Infineon XMC4500 real-time controller
* `PCB/` – electrical schematics, PCB design files, revision history, and hardware release notes
* `docs/` – technical documentation, design notes, and development logs
* `Media/` – schematics, PCB previews, diagrams, and project images
* `README.md` – module documentation

---

### 🧠 2. System Architecture

The HardwareControlBoard is built around the **Infineon XMC4500**, which serves as a dedicated real-time controller for power electronics and actuator control.

The board provides deterministic control of DC motors, stepper motors, heaters, pumps, and auxiliary outputs while continuously monitoring electrical and thermal parameters. It operates as a hardware execution layer, receiving high-level commands from the MainControlBoard and executing them with precise timing.

#### 2.1 XMC4500 (Real-Time Controller)

Acts as the dedicated real-time controller for power electronics and actuator management.

**Core responsibilities:**

- Closed-loop current and speed control for brushed DC motors
- PWM generation for H-bridges, heaters, pumps, and auxiliary outputs
- STEP/DIR generation for external stepper motor drivers
- Current acquisition using synchronized ADC measurements
- Temperature monitoring of power stages and motors
- Quadrature encoder acquisition for feedback control
- Hardware fault detection and protection handling
- Communication with the `MainControlBoard` via SPI
- Transition to a predefined safe state upon communication timeout or critical fault detection

---

#### 2.2 Module Block Diagram

High-level block diagram showing the internal architecture of the HardwareControlBoard, including the XMC4500, power stages, current sensing circuits, stepper motor interfaces, communication links, and signal flow between the analog, digital, and power domains.

### 📋 3. Design Requirements

This chapter defines the electrical and functional requirements that guided the hardware architecture and component selection.

All subsequent design decisions, including power electronics, analog front-end, protection mechanisms, and PCB layout, are based on the requirements listed below.

#### 3.1 Functional Requirements

The HardwareControlBoard is responsible for controlling all power-related actuators and acquiring real-time feedback from the platform.

The module shall provide the following functionality:

| Function | Quantity | Description |
|----------|---------:|-------------|
| Brushed DC Motor Control | 2 | Independent closed-loop control of left and right drive motors |
| Winch Motor Control | 1 | Closed-loop control of the winch motor |
| Stepper Motor Control | 6 | STEP/DIR control for external stepper motor drivers |
| Heater Control | 1 | PWM control of the glycerin vaporizer heater |
| Membrane Pump Control | 1 | PWM control of the smoke generator pump |
| Cooling Fan Control | 2 | PWM control of enclosure cooling fans |
| Current Measurement | 5 TBD | Current acquisition for all power stages |
| Temperature Measurement | TBD | Monitoring of H-bridges, motors, and heater |
| Encoder Acquisition | 2 | Quadrature encoder feedback for drive motors |
| Fault Monitoring | TBD | Detection of hardware faults and driver error signals |
| Level Monitoring | 1 | Glycerin reservoir level detection |
| SPI Communication | 1 | Communication with the MainControlBoard |
| Debug Interface | 1 | SWD, SWO, ETM and UART debugging support |
| External Lighting Control | Multiple | Control of external LED lighting and status indicators |
| Laser Control | 1 | Control of the onboard aiming laser module |

##### 3.2 Low Power Section

The Low Power section is dedicated to driving low-current loads that do not require dedicated H-bridge topologies or closed-loop current control.

All outputs in this section share a common hardware architecture based on low-side N-channel MOSFET switches and a common gate driver, providing a standardized, compact, and scalable design.

**Common Requirements**

- 12 V nominal supply
- Low-side switching topology
- Common MOSFET and gate driver architecture
- PWM capability up to 40 kHz
- Individual enable and software control
- Designed for loads up to 1 A continuous
- Overcurrent protection implemented at the system level
- Modular and reusable hardware design

##### 3.3 Medium Power Section

The Medium Power section is dedicated to driving medium-power actuators that require dedicated power stages, current monitoring, or integrated motor drivers.

Unlike the Low Power section, these loads require application-specific control strategies, including H-bridge topologies, integrated motor drivers, or closed-loop current measurement.

**Common Requirements**

- 12 V nominal supply
- Continuous load power up to 50 W
- PWM operation up to 40 kHz (application dependent)
- Current measurement for protection and diagnostics
- Hardware fault monitoring
- Thermal monitoring where applicable
- Modular and reusable hardware design

### 3.4 High Power Section

The High Power section is dedicated to the propulsion system and consists of discrete H-bridge power stages designed for high-current brushed DC motors.

Unlike the Low and Medium Power sections, these outputs implement closed-loop current and speed control, providing deterministic real-time motor control, fast dynamic response, and comprehensive hardware protection.

**Common Requirements**

- 12 V nominal supply
- Continuous output current up to 30 A per motor
- Peak measurement range up to ±50 A
- Discrete full H-bridge topology
- PWM operation up to 40 kHz
- Closed-loop current control
- Closed-loop speed control
- Quadrature encoder feedback
- Low-side current sensing using shunt resistors
- Real-time overcurrent and fault detection
- Temperature monitoring of the power stage
- Modular and reusable hardware architecture

