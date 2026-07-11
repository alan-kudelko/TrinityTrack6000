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

> 🔄 **Status:** Requirements definition
> 🔧 **Goal:** Develop a dedicated real-time controller for power electronics and motion control, while exploring deterministic embedded software architecture, safety mechanisms and industrial motor control techniques.

---

## Preview

![Module block diagram1](/HardwareControlBoard/Media/HardwareControlBoard_BlockDiagram.drawio.png)



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

- ⚡ Utilize hardware peripherals whenever possible to minimize CPU overhead.
- 🎯 Achieve deterministic execution for all time-critical control tasks.
- 🛡️ Implement hardware-oriented safety mechanisms and fault handling
- 🧱 Build a modular control platform for motors, servos and future actuators
- 📚 Explore industrial real-time control techniques and embedded safety concepts
- 🌐 Create a reusable motion-control platform for future embedded projects

---

## Technical Table of Contents

1. [Module Structure](#1--module-structure)
2. [System Architecture](#-2-system-architecture)
   - [2.1 XMC4500 (Real-Time Controller)](#21-xmc4500-real-time-controller)
   - [2.2 Module Block Diagram](#22-module-block-diagram)
3. [Functional Requirements](#-3-functional-requirements)
   - [3.1 Low Power Section (LPS)](#31-low-power-section-lps)
   - [3.2 Medium Power Section (MPS)](#32-medium-power-section-mps)
   - [3.3 High Power Section (HPS)](#33-high-power-section-hps)
4. [Electrical Requirements](#-4-electrical-requirements)
   - [4.1 Common Electrical Requirements](#41-common-electrical-requirements)
   - [4.2 Analog Voltage Reference (AVR)](#42-analog-voltage-reference-avr)
   - [4.3 Low Power Section (LPS)](#43-low-power-section-lps)
   - [4.4 Medium Power Section (MPS)](#44-medium-power-section-mps)
       - [4.4.1 Heater Section](#441-heater-section)
       - [4.4.2 Winch Motor](#442-winch-motor)
       - [4.4.3 Stepper / Servo](#443-stepper--servo)
           - [4.4.3.1 Stepper Motors](#4431-stepper-motors)
           - [4.4.3.2 RC Servos](#4432-rc-servos)
   - [4.5 High Power Section (HPS)](#45-high-power-section-hps)
5. [Design Calculations](#-5-design-calculations)
   - [5.1 Analog Front-End](#51-analog-front-end)

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

High-level block diagram showing the internal architecture of the `HardwareControlBoard`, including the XMC4500, power stages, current sensing circuits, stepper motor interfaces, communication links, and signal flow between the analog, digital, and power domains.

![Module block diagram1](/HardwareControlBoard/Media/HardwareControlBoard_BlockDiagram.drawio.png)

---

### 📋 3. Functional Requirements

This chapter defines the electrical and functional requirements that guided the hardware architecture and component selection.

All subsequent design decisions, including power electronics, analog front-end, protection mechanisms, and PCB layout, are based on the requirements listed below.

The HardwareControlBoard is responsible for controlling all power-related actuators and acquiring real-time feedback from the platform.

**Notes:**
- The design shall maximize reuse of proven hardware building blocks across all power sections.
- Where electrical requirements are equivalent, identical circuit topologies and components shall be used. Dedicated implementations shall only be introduced when required by significantly different operating conditions or functional requirements.

The module shall provide the following functionality:

| Function | Quantity | Description |
|----------|---------:|-------------|
| Brushed DC Motor Control | 2 | Independent closed-loop control of left and right drive motors |
| Winch Motor Control | 1 | Closed-loop control of the winch motor |
| Stepper Motor Control | 6 | STEP/DIR control for external stepper motor drivers |
| Heater Control | 1 | PWM control of the glycerin vaporizer heater |
| Glycerin Pump Control | 1 | PWM control of the smoke generator pump |
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

#### 3.1 Low Power Section (LPS)

The Low Power Section provides switching and PWM control for auxiliary low-current loads that do not require dedicated power stages or closed-loop current control.

Typical applications include:

- External LED lighting
- Status indicator LEDs
- Laser module
- Logic-level control outputs
- Auxiliary digital outputs
- Future low-current peripherals
- Glycerin pump
- Chassis fans

#### 3.2 Medium Power Section (MPS)

The Medium Power Section provides dedicated control of medium-power actuators requiring application-specific power stages, current monitoring, or closed-loop control.

Typical applications include:

- Winch DC motor with current feedback
- Glycerin vaporizer heater with closed-loop power/current control
- Stepper motor interfaces (external STEP/DIR drivers) or Servo motor control

#### 3.3 High Power Section (HPS)

The High Power Section is dedicated to the propulsion system and provides deterministic closed-loop control of the platform's high-current brushed DC drive motors.

Unlike the Low and Medium Power sections, the propulsion system implements cascaded current and velocity control loops to achieve precise torque regulation, fast dynamic response, and reliable vehicle motion under varying load conditions.

Typical functionality includes:

- Closed-loop current control
- Closed-loop velocity control
- Cascaded Current → Velocity control architecture
- Incremental quadrature encoder feedback
- Real-time motor current measurement
- H-bridge temperature monitoring
- Gate driver temperature and fault monitoring
- Hardware fault detection and protection

---

### ⚡ 4. Electrical Requirements

This chapter defines the electrical requirements that guide the implementation of the `HardwareControlBoard`.

The requirements specified in this section define the electrical characteristics, measurement architecture, protection mechanisms, analog front-end, power stages, and PCB design principles used throughout the project.

Whenever practical, hardware building blocks shall be standardized and reused across the design to simplify validation, manufacturing, firmware development, and future scalability.

Exceptions are permitted only when required by specific electrical, thermal, safety, or performance constraints.

#### 4.1 Common Electrical Requirements

The following electrical requirements apply to all hardware blocks of the `HardwareControlBoard` unless explicitly stated otherwise.

- A global hardware **POWER_ENABLE** signal shall be provided by the `MainControlBoard` and used to enable or disable all power stages.
- All power stages shall implement a dedicated hardware enable input controlled by the global **POWER_ENABLE** signal.
- Power stages shall default to a safe disabled state after power-up or reset until explicitly enabled.
- Precision analog measurements shall use a common voltage reference to ensure consistent ADC scaling and measurement accuracy.
- All analog measurement circuits shall reference the same ADC reference voltage unless specific application requirements dictate otherwise.
- Dedicated implementations are permitted only when justified by electrical, thermal, safety, or performance requirements.

#### 4.2 Analog Voltage Reference (AVR)

The `HardwareControlBoard` shall use a common precision voltage reference for all analog measurements to ensure consistent scaling, repeatability, and measurement accuracy across the entire system.

**Requirements**

- A single precision voltage reference shall be used for the XMC4500 ADC reference input.
- The same voltage reference shall be used by all analog signal conditioning circuits.
- All current sensing circuits shall use a common analog reference to maintain measurement consistency.
- Analog offset voltages required for bidirectional current measurement shall be derived from the common voltage reference.
- The analog reference distribution shall minimize noise coupling from high-current switching circuits.
- The analog reference shall remain independent from the digital 3.3 V supply accuracy.

#### 4.3 Low Power Section (LPS)

- 12 V nominal supply
- Low-side switching topology
- PWM capability up to 40 kHz
- Common MOSFET architecture
- Standardized gate driver architecture for PWM-controlled outputs
- Standardized logic buffer architecture for non-PWM digital outputs
- Individual software control for each output
- Designed for loads up to 1 A continuous
- Modular and reusable hardware building blocks

#### 4.4 Medium Power Section (MPS)

**Common Requirements**

- 12 V nominal supply
- PWM operation up to 40 kHz (application dependent)
- Current measurement for diagnostics and protection
- Thermal monitoring where applicable
- Modular and reusable hardware building blocks

##### 4.4.1 Heater Section

- Low-side MOSFET topology
- Closed-loop current control
- Heater current measurement
- Heater temperature measurement
- PWM power regulation
- Overtemperature protection
- Designed for loads up to 5 A continuous
  
##### 4.4.2 Winch Motor

- Full H-bridge topology
- Closed-loop current control
- Motor current measurement
- Motor temperature monitoring
- H-bridge temperature monitoring
- Hardware fault detection
- Designed for loads up to 5 A continuous

#### 4.4.3 Stepper / Servo

The `HardwareControlBoard` shall support both stepper motors and RC servos through a common connector interface. The final actuator type may be selected during assembly without requiring PCB redesign.

The PCB is designed primarily around external STEP/DIR stepper motor drivers. When RC servos are used instead, the external driver circuitry is bypassed using configurable solder bridges.

**Common Requirements**

- Support for both stepper motors and RC servos
- Common connector interface for both actuator types
- Assembly-time selection using PCB solder bridges
- Modular hardware architecture allowing either implementation without PCB redesign
- Configurable actuator supply voltage
- Support for either 6 V or 12 V actuator supply using PCB solder bridges

##### 4.4.3.1 Stepper Motors

- 12 V nominal supply
- Dedicated stepper motor drivers
- STEP/DIR control interface
- ENABLE signal for each motor driver
- Microstepping supported by dedicated driver hardware
- Current regulation performed internally by the selected stepper driver
- Hardware fault monitoring where supported by the selected driver
- Designed for loads up to 2 A continuous per phase

##### 4.4.3.2 RC Servos

- Direct PWM control generated by the XMC4500
- Stepper motor drivers are not populated
- Compatible with both 6 V and 12 V servos
- Servo supply voltage selectable using PCB solder bridges
- Common connector footprint shared with the stepper motor implementation

#### 4.5 High Power Section (HPS)

The High Power Section is considered the primary engineering challenge of the `HardwareControlBoard`.
Unlike the remaining sections, it combines high-current power electronics, precision analog measurements, closed-loop control algorithms, thermal management, hardware protection mechanisms, and high-speed PCB layout techniques within a single subsystem.

As a result, this section serves as the primary research and development area of the project.

**Electrical Requirements**

- 12 V nominal supply
- Continuous operating current up to 20 A per motor
- Rated current up to 30 A per motor
- Peak current measurement range up to ±50 A
- Discrete full H-bridge topology for each motor channel
- PWM operation up to 40 kHz
- Dedicated half-bridge gate driver for each H-bridge
- Dead-time generation shall be performed by the XMC4500 CCU8 peripheral
- Low-side current sensing using shunt resistors
- Differential current measurement using Kelvin sensing
- Real-time current measurement for control, diagnostics and protection
- Temperature monitoring of the power stage
- Hardware fault monitoring

#### 4.6 Analog Front-End (AFE)

The `HardwareControlBoard` shall utilize a standardized analog front-end architecture for all measurement channels whenever practical.

The primary objective is to maximize hardware reuse, simplify PCB layout, reduce firmware complexity, and minimize calibration effort.

**Requirements**

- Common precision voltage reference shared across all analog measurement circuits
- Standardized current sense amplifier family
- Fixed-gain current measurement architecture preferred
- Measurement range defined primarily by the selected shunt resistor
- Common analog offset for bidirectional current measurement
- Kelvin sensing for all high-current measurements
- Dedicated analog filtering where required
- Modular analog building blocks reused across all power sections whenever possible
- Exceptions shall only be introduced where electrical requirements cannot be satisfied by the standardized architecture

---

### 📐 5. Design Calculations

#### 5.1 Analog Front-End

The `HardwareControlBoard` utilizes the full input range of the XMC4500 Analog-to-Digital Converter to maximize measurement resolution and minimize quantization error.

All analog signal conditioning circuits shall be designed around the following assumptions:

- ADC input voltage range: **0 V to 3.3 V**
- Reference voltage: **3.3 V**
- Mid-scale reference voltage: **1.65 V**
- Bipolar current measurements shall be centered around **1.65 V**
- Positive current shall increase the ADC input voltage
- Negative current shall decrease the ADC input voltage

To maximize the effective utilization of the ADC input range, the ideal current sense amplifier gain would be **30 V/V**, resulting in an output voltage swing of approximately **0.15 V to 3.15 V** for a ±50 A measurement range for driver motors.

However, a commercially available fixed-gain amplifier with **30 V/V** gain could not be identified among the evaluated device families.

Therefore, a gain of **25 V/V** was selected as the best commercially available compromise.

| Device | Current Range | Shunt | Gain | ADC Voltage Range | Current / LSB | Shunt Power Loss @ Imax |
|--------|--------------:|------:|-----:|------------------:|--------------:|------------------------:|
| Left Drive Motor | -50 A to 50 A | 1 mΩ | 25 V/V | 0.40 V – 2.90 V | 24.41 mA | 2.50 W |
| Right Drive Motor | -50 A to 50 A | 1 mΩ | 25 V/V | 0.40 V – 2.90 V | 24.41 mA | 2.50 W |
| Winch Motor | -10 A to 10 A | 5 mΩ | 25 V/V | 0.40 V – 2.90 V | 4.88 mA | 0.50 W |
| Heater | 0 A to 10 A | 5 mΩ | 25 V/V | 1.65 V – 2.90 V | 2.44 mA | 0.50 W |

The table above summarizes the standardized current measurement architecture adopted throughout the HardwareControlBoard. All current sensing channels share a common amplifier gain, while the measurement range is adjusted solely by the selected shunt resistor.

---

### 6. Component Selection

**WIP**

### 7. PCB Design Requirements

The `HardwareControlBoard` is designed as a mixed-signal, high-current control system. PCB requirements were established before the layout phase to ensure reliable operation under realistic operating conditions while maintaining reasonable manufacturing complexity.

The following sections define the thermal and electrical design constraints used during PCB development.

#### 7.1 Thermal Requirements

The PCB shall satisfy the following thermal requirements during continuous operation.

| Parameter | Requirement |
|----------|-------------|
| PCB material | FR-4, 4-layer, 1 oz copper |
| PCB thickness | 1.6 mm |
| Maximum ambient temperature | 40°C |
| Recommended average PCB temperature | ≤ 70°C |
| Maximum local PCB temperature (power stage) | ≤ 90°C |
| Maximum MOSFET junction temperature | ≤ 125°C |
| Maximum average PCB thermal density | TBD (calculated during thermal budget analysis) |

These limits are intended to provide adequate thermal margin while maximizing long-term reliability of the power electronics.

Two PCB dimensions are considered during the design process:

| PCB Size | Board Area | Total Heat Exchange Area* |
|----------|-----------:|--------------------------:|
| 100 × 150 mm | 150 cm² | 300 cm² |
| 100 × 200 mm | 200 cm² | 400 cm² |

#### 7.2 Thermal Budget

Thermal analysis is performed on every major heat-generating subsystem.

The total thermal budget consists of:

- MOSFET conduction losses
- MOSFET switching losses
- Current shunt losses
- Gate driver losses
- Stepper driver losses
- Auxiliary power losses

Each subsystem is evaluated independently in order to identify local hot spots rather than relying only on average PCB temperature.




#### 7.3 Current Budget

The current budget defines electrical loading of the `HardwareControlBoard` under multiple operating scenarios.

Three operating conditions are considered during the design process.

| Scenario | Total Current |
|----------|--------------:|
| Theoretical Maximum | 166 A |
| Engineering Design Case | 102 A |
| Typical Heavy Operation | 86–92 A |

Theoretical Maximum represents simultaneous peak current of all subsystems and is not considered a continuous operating condition.

The Engineering Design Case represents the expected worst realistic operating condition and is used as the primary reference during PCB layout and thermal analysis.

Current requirements are continuously refined using:

- mathematical motor models,
- drivetrain analysis,
- experimental current measurements,
- closed-loop motor control assumptions.

---


