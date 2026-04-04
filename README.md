# TrinityTrack6000

**TrinityTrack6000** is a physically built, modular, multi-MCU tracked robotic platform.

The system is based on a stackable PCB architecture, where each module is responsible for a specific function such as control, power management, sensing, or data processing. A central STM32-based controller coordinates the system, while additional MCUs and FPGA modules handle specialized real-time tasks.

The project combines mechanical design with custom electronics and embedded software, focusing on distributed control, real-time operation, and scalable hardware integration.

## 📦 System Modules

The system is built as a set of dedicated PCB modules, each responsible for a specific function within the platform.
A subset of modules forms a **stackable backplane**, connected via a shared SPI bus and a common 5V power rail distributed through a high-density board-to-board connector.
Other modules are connected externally depending on their function and power requirements.

### 🔌 Stack Modules (SPI Backplane)

These modules are physically stacked and share:
* SPI communication bus
* Dedicated control lines (CS, READY, RESET)
* Common 5V power rail
* **MainControlBoard** – STM32 + XMC4200 (system coordinator and real-time control)
* **VideoAudioBoard** – FPGA-based processing *(planned)*
* **FireControlBoard** – Ballistic computation *(planned)*
* **RadarBoard** – Radar processing module *(planned)*

### 🔧 External Modules

These modules are connected outside of the stack and may use separate power paths or interfaces:
* **PowerConversionBoard** – Power regulation and distribution
* **HardwareControlBoard** – Motor/servo interface and power-stage control
* **BMSBoard** – Battery management system

Each module is developed independently and integrates into the system through clearly defined electrical and communication interfaces.

---

## 🧪 Project Scope

The project serves as a practical platform for exploring and implementing:

* Distributed embedded systems and **multi-MCU coordination**
* Real-time system design using **RTOS (ThreadX / bare-metal approaches)**
* Inter-module communication over a **shared SPI backplane**
* System-level synchronization, fault detection, and **failsafe mechanisms**
* FPGA-based **real-time audio/video processing and compression**
* Radar signal processing and **object detection techniques**
* Power electronics, including **switch-mode power supplies and motor control (H-bridge)**
* Modular hardware design and **scalable PCB-based system architecture**

Due to the modular nature of the system, the repository is organized around individual PCB modules.

Each module is contained in a dedicated directory and includes:

* hardware design files (schematics and PCB)
* firmware for the corresponding MCU(s)
* local documentation and development notes

Every module has its own `README.md`, describing its functionality, design decisions, and implementation details.

This structure allows independent development, easier navigation, and scalable integration of new modules into the system.

---

## 🔧 Status

- 🔄 Ongoing development of **MainControlBoard (STM32 + XMC4200)**
- 🔄 Firmware development, hardware bring-up, and system integration in progress

---


## 🛠 Tech Stack

- **MCUs:** STM32G4, XMC4200, NXP / Renesas (planned), FPGA  
- **RTOS:** ThreadX  
- **Communication:** SPI, UART, I2C, wireless link  
- **Build system:** CMake + arm-gcc

---

## 🖼️ Hardware Overview

### 🚜 Tank Platform

![Tank Chassis](Media/Tank_Body_1.jpg)
![Tank Chassis](Media/Tank_Body_2.jpg)

---

### 🧠 MainControlBoard (STM32 + XMC4200)

*PCB preview and assembly*

![Main control board top](/MainControlBoard/Media/PCB_Top.png)

![Main control board bottom](/MainControlBoard/Media/PCB_Bottom.png)

---

### ⚡ PowerConversionBoard

*PCB preview and assembly*

---

### 🔧 HardwareControlBoard

*PCB preview and assembly*

---

### 🎥 VideoAudioBoard (FPGA)

*PCB preview and assembly*

---

### 🎯 FireControlBoard *(planned)*

---

### 📡 RadarBoard *(planned)*

---

### 🔋 BMSBoard

*PCB preview and assembly*

## 📁 Repository Structure

The repository is organized around modular hardware components and supporting resources.

* `MainControlBoard/` – main control system (STM32 + XMC4200)
* `HardwareControlBoard/` – motor and actuator interface
* `PowerConversionBoard/` – power regulation and distribution
* `RadarModuleBoard/` – radar module *(planned)*
* `AVProcessingBoard/` – FPGA-based audio/video processing *(planned)*
* `FireControlBoard/` – fire control system *(planned)*
* `MechanicalDesign/` – chassis and mechanical components
* `Datasheets/` – documentation for used components
* `docs/` – technical documentation and generated files (e.g. Doxygen)
* `Media/` – images, diagrams, and visual materials
* `tools/` – development tools and helper scripts

Each module contains its own firmware, hardware design files, and documentation.

## 🤝 Acknowledgments

PCB manufacturing for this project was sponsored by **PCBWay**, enabling rapid prototyping and validation of the hardware design.
