# TrinityTrack6000

**TrinityTrack6000** is a physically built, modular, multi-MCU tracked robotic platform.

The system is built around a modular PCB architecture, consisting of a stackable processing backplane and dedicated external control and power modules. Each module is responsible for a single functional domain such as system coordination, motion control, power management, sensing, or high-performance data processing.

A central STM32-based `MainControlBoard` supervises overall system operation, while dedicated MCU and FPGA modules perform local real-time processing and subsystem control.

The project combines mechanical design, custom electronics, embedded software, and power electronics, focusing on distributed system architecture, real-time operation, and scalable hardware integration.

## 📦 System Modules

The system is built as a set of dedicated PCB modules, each responsible for a specific function within the platform.
A subset of modules forms a **stackable processing backplane**, connected via a shared SPI bus and a common 5V power rail distributed through a high-density board-to-board connector.

Power electronics and battery-related modules are connected externally through dedicated communication and power interfaces.

### 🔌 Stack Modules

These modules are physically stacked and provide the system's processing and control backplane.

* **MainControlBoard** – System supervision, communication and telemetry
* **VideoAudioBoard** – FPGA-based audio/video processing *(planned)*
* **FireControlBoard** – Weapon subsystem control and ballistic computation *(planned)*
* **RadarBoard** – Radar signal processing and radar subsystem control *(planned)*

### 🔧 External Modules

These modules are connected outside of the processing stack and provide power conversion, battery management, and real-time actuator control.

* **HardwareControlBoard** – Generic real-time actuator controller, power electronics and motion control
* **PowerConversionBoard** – Power regulation and distribution
* **BMSBoard** – Battery management system

Each module is developed, tested, and documented independently, while integrating into the system through clearly defined electrical and communication interfaces.

---

## 🧪 Project Scope

The project serves as a practical platform for exploring and implementing:

* Distributed embedded systems and multi-MCU architectures
* Real-time embedded control systems
* Inter-module communication
* System synchronization, diagnostics, and failsafe mechanisms
* FPGA-based audio/video processing
* Radar signal processing and object detection
* Power electronics and motor control
* Modular hardware and scalable PCB system architecture

Due to its modular architecture, the repository is organized into dedicated hardware modules.

Each module contains:

* Hardware design files (schematics and PCB)
* Firmware
* Module-specific documentation

Every module includes its own `README.md` describing its purpose, architecture, and implementation details.

This organization enables independent development, testing, and future scalability.

---

## 🔧 Status

- 🔄 **MainControlBoard** hardware validation and firmware development
- 🔄 **HardwareControlBoard** architecture and hardware design
- 🔄 System architecture refinement and module integration

---

## 🖼️ Hardware Overview

### 🚜 Tank Platform

![Tank Chassis](Media/Tank_Body_1.jpg)
![Tank Chassis](Media/Tank_Body_2.jpg)

---

### 🧠 MainControlBoard

![PCB View1](/MainControlBoard/Media/PCB_View1.jpg)
![PCB View2](/MainControlBoard/Media/PCB_View2.jpg)
![PCB View3](/MainControlBoard/Media/PCB_View3.jpg)
![PCB View4](/MainControlBoard/Media/PCB_View4.jpg)
![PCB View5](/MainControlBoard/Media/PCB_View5.jpg)

![Main control board top](/MainControlBoard/Media/PCB_Top.png)

![Main control board bottom](/MainControlBoard/Media/PCB_Bottom.png)

---

### 🔧 HardwareControlBoard

*PCB preview and assembly*

---

### ⚡ PowerConversionBoard

*PCB preview and assembly*

---

### 🎥 VideoAudioBoard (FPGA)

*PCB preview and assembly*

---

### 🎯 FireControlBoard *(planned)*

*PCB preview and assembly*

---

### 📡 RadarBoard *(planned)*

*PCB preview and assembly*

---

### 🔋 BMSBoard

*PCB preview and assembly*

## 🧠 System Architecture

The diagram below presents a high-level overview of the system architecture, including communication interfaces, control paths, and power distribution between all modules.

It illustrates how individual boards are interconnected, the role of each subsystem, and the overall data and energy flow within the platform.

![System Architecture Diagram](/Media/System_Architecture_Diagram.png)


> ⚠️ Note: This diagram represents a high-level abstraction of the system and does not include low-level implementation details such as signal timing, electrical characteristics, or protocol configurations.

---

## 📁 Repository Structure

The repository is organized into dedicated hardware modules and supporting resources.

* [`MainControlBoard/`](MainControlBoard/) – System supervision, communication, telemetry, and coordination of all subsystem modules
* [`HardwareControlBoard/`](HardwareControlBoard/) – Real-time motion control, actuator control, and power electronics
* [`PowerConversionBoard/`](PowerConversionBoard/) – System power conversion, regulation, and power distribution
* [`BMSBoard/`](BMSBoard/) – Battery monitoring, protection, and state estimation
* [`FireControlBoard/`](FireControlBoard/) – Weapon subsystem control and ballistic computation *(planned)*
* [`RadarBoard/`](RadarBoard/) – Radar subsystem control and signal processing *(planned)*
* [`VideoAudioBoard/`](VideoAudioBoard/) – FPGA-based audio and video processing *(planned)*
* [`MechanicalDesign/`](MechanicalDesign/) – Mechanical CAD models and assemblies
* [`Datasheets/`](Datasheets/) – Component datasheets and reference documentation
* [`docs/`](docs/) – Project documentation and generated files
* [`Media/`](Media/) – Images, renders, and architecture diagrams
* [`tools/`](tools/) – Development and utility scripts

Each hardware module contains its own hardware design, firmware, documentation, and development resources.

## 🤝 Acknowledgments

PCB manufacturing for this project was sponsored by **PCBWay**, enabling rapid prototyping and validation of the hardware design.

<img src="Media/PCBWay-Logo.png" width="200"/>
