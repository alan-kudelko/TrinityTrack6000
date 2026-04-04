# **ControlBoard** – **STM32G4 + XMC4200**  

**STM32G4**: Main system controller responsible for coordination, telemetry handling and communication (radio via nRF24L01+)

**XMC4200**: Dedicated real-time controller for motor and servo control, including encoder feedback and monitoring (current, temperature)

The system is built on a custom dual-MCU PCB, where responsibilities are clearly separated between high-level logic (STM32) and time-critical control tasks (Infineon).

- **STM32** leverages **ThreadX RTOS** for system supervision, communication, telemetry processing and coordination of all subsystems  
- **Infineon** uses **Micrium µC/OS RTOS** for deterministic motor and servo control with precise timing requirements

All tasks on both MCUs are allocated using **fully static memory allocation**, with **guard zones** implemented via custom **linker scripts**. Task stacks and guard regions are arranged contiguously in memory, enabling runtime monitoring and deterministic stack overflow detection.

The system includes **robust diagnostics and fault-handling mechanisms**, such as:
- RAM and CPU usage monitoring  
- Task stack overflow detection  
- Persistent error logging in FRAM  

The project follows **MISRA C:2025** guidelines to ensure code safety, maintainability and portability.

![PCB Top](/MainControlBoard/Media/PCB_Top.png)

**4-layer PCB with RF (SMA), GNSS and dual-MCU architecture**

> ✅ Status: Currently in active development and prototyping  
> 🔧 Goal: Serve as an educational platform for learning **STM32**, **Infineon MCUs**, and real-time systems **ThreadX** and **Micrium µC/OS**, while exploring multi-MCU coordination and embedded system design  

---

## ✳️ Planned Technologies & Tools

- **MCUs**: STM32G4 (high-level coordination, RTOS), Infineon XMC4200 (motor & servo control, real-time monitoring)
- **RTOS**: ThreadX on STM32, Micrium µC/OS on XMC4200
- **Memory Management**: Guard zones, static memory allocation, custom linker scripts
- **Interfaces**: UART, SPI, and I²C for peripheral communication
- **Communication Modules**: nRF24L01 for radio control
- **Development Tools**: VS Code, CMake, Ninja, arm-gcc toolchain, STM32CubeIDE, DAVE IDE
- **Software Libraries**: STM32 HAL/LL drivers, Infineon low-level drivers
- **Diagnostics & Monitoring**: RAM and CPU usage tracking, EEPROM fault logging

## Development Strategy

- **Prototyping Phase:**  
  Early validation is performed on a breadboard, focusing on interfaces, signal integrity, and integration of planned peripherals.  
  - **STM32G4** is programmed using an STM32 Nucleo board (ST-LINK)  
  - **XMC4200** is programmed using a modified **KIT_XMC45_RELAX_LITE_V1** (on-board debugger)

- **Production Phase:**  
  The final system is implemented on a custom PCB featuring:
  - **STM32G4** as the main system controller  
  - **XMC4200** as the real-time control unit

## Key Features

- Modular software architecture
- Integration of multiple sensors and communication interfaces
- Scalable design for both prototyping and production
- Hybrid approach: HAL for fast development, LL/bare-metal for final product optimization
  
### 🔌 Hardware & Electronics
- **Nucleo-64 STM32L4RGT6 board** for rapid prototyping and STM32 development
- **Infineon KIT_XMC45_RELAX_LITE_V1 evaluation board** for rapid prototyping and learning the XMC MCU family
- Custom PCB hosting:
  - **STM32G473RET6 MCU** – Handles high-level coordination, telemetry and system management
  - **Infineon XMC4200F64K256BAXQSA1 MCU** – Responsible for real-time motor and servo control with feedback monitoring
  - **nRF24L01+** wireless transceiver
  - **L76L-M33** GNSS (GPS) module
  - **FM25L16B-GTR** FRAM memory for persistent data logging and configuration storage
  - **ADXL345** accelerometer for motion/orientation sensing
  - **STWD100** watchdog supervisor for system reliability
  - **TLV76133** 3.3V LDO regulator
  - **MQ-6** LPG / flammable gas sensor
  - **MQ-7** carbon monoxide gas sensor
  - **Buzzer** for diagnostics and system feedback
  - **Cortex-M debug connectors (modified 10-pin)** for programming and debugging

### 🧠 System Architecture & Concepts
- Real-time system based on **ThreadX** (STM32) and **Micrium µC/OS** (Infineon)
- Static memory allocation only — no malloc, no heap fragmentation
- Custom linker scripts with dedicated memory sections for task stacks and **guard zones**
- Preemptive multitasking with structured task separation
- Guard zone-based stack overflow detection for all tasks
- EEPROM-based error logging and system state preservation
- Fault recovery logic with planned automatic restart on critical errors

### 🧰 Development Tools & Libraries
- **C / C++** for firmware development
- Native MCU libraries with low-level register access
- **ThreadX** for STM32, **Micrium µC/OS** for Infineon
- **VS Code, CMake, Ninja** for building and managing the project
- **Doxygen** for automatic code documentation
- **XMC Pinout Tool** for selecting pins on XMC4200 MCU

### 📚 Documentation

The source code is fully documented using **Doxygen**, which generates up-to-date, browsable documentation from the annotated source files. The generated docs can be found in the `docs` directory and are updated as part of the build process.

---

## 🧠 Design Goals

- 💾 Use 100% static memory allocation (no malloc, no heap)
- 🧰 Track system stability via runtime task/memory debug tools, including guard zones and stack overflow detection
- 🔁 Ensure fault safety: robust recovery after failure or communication loss using EEPROM fault logging
- 🧪 Serve as a practical educational platform for learning **STM32** and **Infineon MCUs**, as well as real-time operating systems **ThreadX** and **Micrium µC/OS**
- 📚 Deepen understanding of multitasking, resource sharing, fail-safe system design, and embedded system best practices
- 🧩 Implement low-level memory management techniques for optimized and reliable resource control
- 🧱 Integrate tightly with custom hardware (PCB, sensors, actuators)
- 🌐 Explore principles of distributed embedded systems and multi-MCU communication commonly used in commercial applications
  
---

## 🗺️ Development Progress

### Build System & Tooling
- ✅ Migration to VS Code + CMake + Ninja (arm-gcc toolchain)
- ✅ Support for Debug / Release builds
- ✅ Verified `.elf` and `.map` outputs for flashing and debugging

### STM32G4
- ✅ Custom linker script with extended memory sections (RAM2, CCSRAM, diagnostics)
- 🔄 System initialization and startup sequence
- 🔄 Runtime diagnostics (RAM usage, stack monitoring via UART)

### XMC4200
- ✅ Migration to CMake-based build system (DAVE-generated sources)
- 🔄 SPI communication with STM32 (in progress)

📓 Detailed development notes: [docs/dev_log](docs/dev_log/dev_log.md)

---

## Technical Table of Contents

1. [ControlBoard Module Structure & File Overview](#1--controlboard-module-structure--file-overview)
2. [Design considerations](#2-design-considerations)
3. [MCU's pinouts](#3-mcus-pinouts)
   - [3.1 STM32G473RET6 Pinout (LQFP-64)](#31-stm32g473ret6-pinout-lqfp-64)
   - [3.2 XMC4200F64K256BAXQSA1 Pinout (TQFP-64)](#32-XMC4200F64K256BAXQSA1-pinout-tqfp-64)
4. [System's architecture](#4-systems-architecture)
5. [STM32G473RET6](#5-stm32g473ret6)
   - [5.7 💾 Memory Layout](#57--memory-layout)
     - [5.7.1 RAM Map](#571-ram-map)
     - [5.7.2 Custom RAM segments](#572-custom-ram-segments)
     - [5.7.3 Free Memory Calculation](#573-free-memory-calculation)
     - [5.7.4 RAM Usage Overview](#574-ram-usage-overview)
   - [5.8 MCU Diagnostics](#58-mcu-diagnostics)
     - [5.8.1 ThreadX Tasks Diagnostics](#581-threadx-tasks-diagnostics)
     - [5.8.2 RAM Usage Diagnostics](#582-ram-usage-diagnostics)
6. [XMC4200](#6-xmc4200)
7. [Electrical Schematic](#7--electrical-schematic)
   - [7.1 Voltage regulation for logic part](#71-voltage-regulation-for-logic-part)
   - [7.2 Gas Sensors](#72-gas-sensors)
   - [7.3 NRF24L01P 2.4GHz Transceiver](#73-nrf24l01p-24ghz-transceiver)
   - [7.4 L76L-M33 GPS](#74-l76l-m33-gps)
   - [7.5 STWD100NYWY3F external watchdog timer](#75-stwd100nywy3f-external-watchdog-timer)
   - [7.6 FM25L16B-GTR FRAM](#76-stwd100nywy3f-external-watchdog-timer)
   - [7.7 ADXL-345 Accelerometer](#77-adxl-345-accelerometer)
   - [7.8 Buzzer](#78-buzzer)
   - [7.9 STM32G473RET6](#79-stm32g473ret6)
   - [7.10 XMC4200F64K256BAXQSA1](#710-xmc4200f64k256baxqsa1)
   - [7.11 Programming and Debug Interfaces](#711-programming-and-debug-interfaces)
   - [7.12 Hardware Control Connector](#712-hardware-control-connector)
   - [7.13 PCB Stack SPI Interface](#713-pcb-stack-spi-interface)
8. [PCB](#8--pcb)
   - [8.1 PCB Preview](#81-pcb-preview)
   - [8.2 Assembly Notes](#82-assembly-notes)
   - [8.3 BOM (Bill of Materials)](#83-bom-bill-of-materials)
---

## ⚙️ Technical Overview 

### 1. 📦 Module Structure

    MainControlBoard/
    ├── STM32/ # Main MCU firmware (STM32G4, prototyping and production)
    ├── XMC4200/ # Peripheral MCU firmware
    ├── AVR_NRF_Tester/ # Simple test tool for radio communication
    ├── docs/ # Technical documentation (Doxygen, notes)
    ├── Media/ # Schematics, PCB previews, diagrams
    └── README.md # Module documentation

---

### 2. Design considerations

#### 2.1 MCU Responsibilities / Scope of Control

Due to the complexity of the system, it is essential to clearly define the responsibilities and scope of each MCU during the theoretical design stage to ensure proper modularization, communication efficiency, and maintainability. During the design phase, it is difficult to predict all possible scenarios or critical connections, which is why the design process follows an iterative improvement loop.

##### 2.1.1 STM32's Responsibilities

The STM32 serves as the main system controller and high-level supervisor – effectively the central "brain" of the project. It coordinates communication between all MCUs, monitors their responsiveness, and can reset individual modules in case of a timeout or fault condition.

Inter-MCU communication is implemented using a dedicated high-speed SPI interface with separate CS lines and reset control signals, allowing robust fault recovery and module reinitialization.

In addition, the STM32 handles all non-power-electronics-related peripherals, wireless communication, telemetry processing, and system-level diagnostics. It also manages persistent data storage using an onboard FRAM device.

*List of responsibilities:*
- System-level supervision and synchronization
- Inter-MCU communication (SPI-based)
- Fault detection and module reset handling
- Wireless communication via nRF24L01+ (2.4 GHz)
- GNSS data acquisition from L76L-M33 module
- Gas sensing:
  - MQ-6 (LPG / flammable gases)
  - MQ-7 (carbon monoxide)
- Motion sensing via ADXL345 accelerometer
- Data logging and persistent storage using FM25L16B-GTR FRAM
- Buzzer control for diagnostics and system feedback
- Control logic for turret systems (arming, disarming, firing)

---

##### 2.1.2 XMC4200 Responsibilities

The Infineon XMC4200 MCU is responsible for all real-time hardware control tasks, especially those related to power electronics and precise timing. It acts as a dedicated low-level controller for motors, actuators, and high-power subsystems.

This MCU generates control signals, processes feedback from sensors and encoders, and ensures safe operation of all power-demanding components.

*List of responsibilities:*
- Generation of PWM signals for DC motors, servos and actuators
- Encoder signal processing for position and speed feedback
- Acquisition of temperature and current measurements via external ADCs (I²C bus, HardwareControlBoard)
- Control of the smoke generation system (heater and glycerin pump)
- Monitoring of current limits and implementation of basic fault protection mechanisms
- Real-time control loops for precise positioning (e.g. radar servo systems)

---

### 3. MCU's pinouts

#### 3.1 STM32G473RET6 Pinout (LQFP-64)

![STM32 Pinout](/MainControlBoard/Media/STM32_Pinout.png)

**Notes:**
- Pins labeleded as **NC_x** are currently not used. They are provided as futureproofing of the project.

#### 3.2 XMC4200F64K256BAXQSA1 Pinout (TQFP-64)


---

### 4. Control board logic diagram

![Control board logic diagram](/MainControlBoard/Media/PCB_Architecture.png)

---

### 5. STM32G473RET6

#### 5.1 Clock configuration

![STM32G473 Clock Configuration](/MainControlBoard/Media/STM32G473RET6_ClockConfiguration.png)

#### 5.2 System initialization

The system initialization process on the STM32G4 microcontroller is one of the most critical stages in the TrinityTrack6000 startup sequence.
As the main MCU in a distributed, multi-controller architecture, it is responsible for establishing a stable runtime environment and verifying the integrity of all essential hardware components before launching the operating system.

#### 5.3 Normal start

#### 5.4 Fault start

#### 5.5 Normal operation

#### 5.6 Task Overview

| Task ID | Task Name                  | Description                                                                                                               | Priority | Stack Size | Free Stack |
|---------|----------------------------|---------------------------------------------------------------------------------------------------------------------------|----------|------------|------------|
| 00      | `taskErrorHandler`         | Handles critical faults such as stack overflows and guard zone corruption, and logs errors to EEPROM                      |    3     |    256     |     50     |
| 01      | `taskSerialDiagnostics`    |                                                                                                                           |    1     |    x       |     x      |

#### 5.7 ThreadX Architecture

#### 5.8 💾 Memory Layout

##### 5.8.1 RAM Map

This section provides a detailed description of the memory sections and their roles within the project. It covers the organization of all RAM banks (RAM1, RAM2, and CCSRAM), custom linker-defined sections, and their purpose in task management, memory diagnostics, and system operation.

![STM32G473_RAM_MAP](/MainControlBoard/Media/STM32G473_RAM_MAP.png)

###### 5.8.1.1 RAM1
- `__DATA_start__` is a custom linker symbol representing the starting address of the `.data` section in RAM1 on STM32G473CET6
- `__DATA_end__` is a custom linker symbol representing the ending address of the `.data` section in RAM1 on STM32G473CET6
- `__BSS_start__` is a custom linker symbol representing the starting address of the `.bss` section in RAM1 on STM32G473CET6
- `__BSS_end__` is a custom linker symbol representing the ending address of the `.bss` section in RAM1 on STM32G473CET6
- `__TDAT_start__` is a custom linker symbol representing the starting address of the `.tdat` section in RAM1 on STM32G473CET6
- `__TDAT_end__` is a custom linker symbol representing the ending address of the `.tdat` section in RAM1 on STM32G473CET6
- `ramDiagnosticsRAM1_lastHeapEnd` is a C variable defined by me to represent the current end of the heap. Its value is calculated at runtime (see Notes below).
- `ramDiagnosticsRAM1_lastMSP` is a C variable defined by me to capture the last value of the main stack pointer before the RTOS scheduler starts (see Notes below).
- `__RAM1_end__` is a custom linker symbol representing the last address of RAM1 on STM32G473CET6

*RAM1 notes:*
- The `.tdat` section is a custom linker-defined memory segment used to store non-critical or rarely used Task Control Blocks (TCBs), their stacks, and associated guard zones. By placing these stacks contiguously within `.tdat`, the system ensures controlled allocation and simplifies stack overflow detection for non-critical tasks.
- The symbols `__TDAT_start__` and `__TDAT_end__` were predefined in the linker script, along with a custom `.tdat` section. This section is used to store Task Control Blocks (TCBs), task stacks, and corresponding guard zones. The `.tdat` section ensures that stacks and their guard zones are placed contiguously in memory, enabling reliable stack overflow monitoring.
- The `ramDiagnosticsRAM1_lastHeapEnd` variable is computed as:
  
      ramDiagnosticsRAM1_lastHeapEnd = (__sbrk_heap_end != NULL) ? __sbrk_heap_end : (void*)&__TDAT_end__;
  
  Here, `__sbrk_heap_end` is a pointer managed by the STM32's custom implementation of `_sbrk`, indicating the current top of the heap. If no memory has been allocated yet, it remains `NULL`.
- The `ramDiagnosticsRAM1_lastMSP` variable captures the value of the main stack pointer (`MSP`) before the RTOS scheduler starts. After the scheduler starts, `MSP` is switched to the currently running task's stack pointer, which would make direct free memory calculations invalid if used afterward.

###### 5.8.1.2 RAM2
- `__RAM_DIAGNOSTICS_start__` is a custom linker symbol representing the starting address of the `.ramDiagnostics` section in RAM2
- `__RAM_DIAGNOSTICS_end__` is a custom linker symbol representing the ending address of the `.ramDiagnostics` section in RAM2
- `__SYS_DIAGNOSTICS_start__` is a custom linker symbol representing the starting address of the `.sysDiag` section in RAM2
- `__SYS_DIAGNOSTICS_end__` is a custom linker symbol representing the ending address of the `.sysDiag` section in RAM2
- `__RAM2_end` is a custom linker symbol representing the ending address of the RAM2 on STM32G473CET6

*RAM2 notes:*
- The `.ramDiagnostics` section stores variables related to memory usage and diagnostics of the RAM state. These variables are not time-critical, allowing them to reside in RAM2, which has slower access compared to other RAM regions.
- The `.sysDiag` section is used for storing global error flags and other system diagnostics that are also not time-critical.
- Access to RAM2 is slower compared to other RAM regions, making it suitable for non-time-critical data storage.

###### 5.8.1.3 CCSRAM
- `__CRIT_start__` is a custom linker symbol representing the starting address of the `.crit` section in CCSRAM
- `__CRIT_end__` is a custom linker symbol representing the ending address of the `.crit` section in CCSRAM
- `__CCSRAM_end__` is a custom linker symbol representing the ending address of the CCSRAM on STM32G473CET6

*CCSRAM notes:*
- The `.crit` section (located in CCSRAM) stores TCBs, stacks, guard zones, and buffers for critical or frequently executed tasks, ensuring maximum performance and predictability.
- CCSRAM is tightly coupled with the CPU core, providing the fastest access compared to other RAM regions on STM32G473, making it ideal for time-critical data and task management.

##### 5.8.2 Custom RAM Segments

The default linker script generated by STM32CubeIDE only includes sections for the RAM1 bank and the minimal sections required for basic programs to run on the STM32.  

To fully utilize the available memory and support advanced features such as runtime memory diagnostics and critical task management, the linker script was modified. Additional RAM banks (RAM2 and CCSRAM) and custom memory sections were added, including `.tdat`, `.crit`, `.ramDiagnostics`, and `.sysDiag`, allowing fine-grained control over task stacks, guard zones, and system diagnostics.

The naming convention was intentionally unified across all regions to improve **code readability and maintainability**.

These symbols representing the **start and ending addresses of each RAM bank**.  

    __RAM1_start__ = ORIGIN(RAM);
    __RAM1_end__ = ORIGIN(RAM) + LENGTH(RAM);

    __RAM2_start__ = ORIGIN(RAM2);
    __RAM2_end__ = ORIGIN(RAM2) + LENGTH(RAM2);

    __CCSRAM_start__ = ORIGIN(CCSRAM);
	__CCSRAM_end__ = ORIGIN(CCSRAM) + LENGTH(CCSRAM);

In addition to the custom sections, the variables in the default `.data` and `.bss` sections have been **renamed and unified** with consistent naming conventions.  
This unification improves **code readability**, simplifies memory diagnostics, and ensures a coherent approach when calculating used and free memory across RAM1, RAM2, and CCSRAM.

	__DATA_start__ = .
    __DATA_end__ = .

 	__BSS_start__ = .
    __BSS_end__ = .

The `.tdat` section, located in **RAM1**, contains variables such as **Task Control Blocks (TCBs)**, task **stacks**, and corresponding **guard zones** for non-critical or rarely executed tasks.  

By placing these elements in `.tdat`, the system ensures that stack memory for less critical tasks is grouped together, enabling efficient memory usage and reliable **stack overflow detection**, while keeping time-critical memory regions free for high-priority tasks.

    .tdat :
    {
	. = ALIGN(4);
	PROVIDE (__TDAT_start__ = . );
	
	KEEP(*(.tdat.guardZone0));	
	KEEP(*(.tdat.errorHandlerStack));	
	
	KEEP(*(.tdat))
	KEEP(*(.tdat*))
	PROVIDE (__TDAT_end__ = . );
    } >RAM
 

The `.ramDiagnostics` section is defined in **RAM2** and groups diagnostic variables by their data type (`uint8_t`, `uint16_t`, `uint32_t`).  
This organization ensures proper alignment while minimizing unused padding ("fill") between variables.  

The reason for this optimization is that the final size of the project is not yet known, so reducing RAM waste is important to keep the system scalable and efficient as the codebase grows.

    .ramDiagnostics : 
    {
    PROVIDE ( __RAM_DIAGNOSTICS_START__ = . );
    . = ALIGN(4);
    *(.ramDiagnostics.uint8_t)
    . = ALIGN(2);
    *(.ramDiagnostics.uint16_t)
    . = ALIGN(4);
    *(.ramDiagnostics.uint32_t)
    PROVIDE ( __RAM_DIAGNOSTICS_END__ = . );
    } >RAM2

The `.sysDiag` section is also placed in **RAM2** and contains diagnostic variables such as **global error flags** and other system state indicators.  
Access to these variables is not time-critical, which makes RAM2 suitable for storing them.  

In fact, all of **RAM2** is dedicated to variables whose access is not timing-critical, including memory usage diagnostics (`.ramDiagnostics`) and system status indicators (`.sysDiag`).  
This separation allows the main RAM (RAM1 and CCSRAM) to be used for time-critical tasks and stacks, while still providing a centralized location for runtime diagnostics and error monitoring.

    .sysDiag :
    {
    PROVIDE ( __SYS_DIAGNOSTICS_START__ = . );
    . = ALIGN(4);
    *(sysDiag);
    PROVIDE ( __SYS_DIAGNOSTICS_END__ = . );
    } >RAM2

##### 5.8.3 Free Memory Calculation

Free memory is calculated separately for each RAM bank (RAM1, RAM2, and CCSRAM) based on linker symbols and runtime diagnostics variables.
- `ramDiagnosticsRAM1_used` calculates the memory used in RAM1, including the main stack (MSP) before the RTOS scheduler starts and the heap usage
- `ramDiagnosticsRAM2_used` calculates the memory occupied by sections in RAM2
- `ramDiagnosticsCCSRAM_used` calculates the memory occupied by the sections in CCSRAM

All used memory values are expressed in **kilobytes (kB)**:

    ramDiagnosticsRAM1_used=(((uint32_t)&__RAM1_end__ - ramDiagnosticsRAM1_lastMSP) + (ramDiagnosticsRAM1_lastHeapEnd - (uint32_t)&__RAM1_start__))/1024;
    ramDiagnosticsRAM2_used=((uint32_t)&__RAM_DIAGNOSTICS_end__ - (uint32_t)&__RAM2_start__)/1024;
 	ramDiagnosticsCCSRAM_used=((uint32_t)&__CRIT_end__ - (uint32_t)&__CCSRAM_start__)/1024;
    ramDiagnosticsGeneral_used = ramDiagnosticsRAM1_used + ramDiagnosticsRAM2_used + ramDiagnosticsCCSRAM_used;

The free memory for each RAM bank and the overall system is then calculated by subtracting the used memory from the total size of the corresponding memory region:

    ramDiagnosticsRAM1_total_size - ramDiagnosticsRAM1_used
    ramDiagnosticsRAM2_total_size - ramDiagnosticsRAM2_used
    ramDiagnosticsCCSRAM_total_size - ramDiagnosticsCCSRAM_used
    ramDiagnosticsGeneral_total_size - ramDiagnosticsGeneral_used

##### 5.8.4 RAM Usage Overview

(will be summarized after adding ThreadX and tasks to the project)

---

#### 5.9 MCU Diagnostics

Diagnostics for the STM32G473CET6 microcontroller can be performed either via the **ST-LINK** interface in STM32CubeIDE or through a **UART** interface.

The diagnostics system provides real-time insight into:
- The current state of system tasks (Task Control Blocks, stack usage, and guard zones)
- RAM usage and memory status across all memory banks
- The most recent error codes and system fault flags

This enables developers and engineers to monitor system health, detect stack overflows, and analyze runtime behavior for both critical and non-critical tasks.

##### 5.9.1 ThreadX Tasks Diagnostics

(soon)

##### 5.9.2 RAM Usage Diagnostics

RAM usage diagnostics provide a detailed view of the memory sections for each RAM bank (RAM1, RAM2, and CCSRAM).  

This diagnostics functionality is accessible via the **UART interface**, allowing real-time monitoring of:
- Memory usage per section
- Free and used memory in kilobytes (kB)
- Task stack allocation and guard zones (for RAM1 and CCSRAM)

This diagnostics functionality is accessible via the **UART interface** and consists of:
- A **main menu** summarizing all RAM banks and their overall usage
- Detailed views for each individual RAM bank, showing section-specific memory usage, free and used memory in kilobytes (kB), and task stack allocation with guard zones (for RAM1 and CCSRAM)

The diagnostics interface is structured similarly to the diagrams below, providing an intuitive visualization of memory allocation and usage.

      +-------------------------[ RAM DIAGNOSTICS ]--------------------------+
      | Bank   | Start      | End        | Size    | Usage      | Used       |
      +--------+------------+------------+---------+------------+------------+
      | RAM1   | 0x20000000 | 0x2001FFFF | 128 KB  | ########## | 80%        |
	  | RAM2   | 0x20020000 | 0x2003FFFF |  64 KB  | ####------ | 40%        |
      | CCSRAM | 0x10000000 | 0x10003FFF |  16 KB  | ##-------- | 20%        |
	  +--------+------------+------------+---------+------------+------------+
      | FREE RAM TOTAL: 250 KB                                               |
      | Commands: s(snapshot) b(bank) q(quit)                                |
	  +----------------------------------------------------------------------+

      +------------------------[ BANK RAM1 DETAILS ]-------------------------+
	  | Section | Start      | End        | Size    | Usage     |            |
      +---------+------------+------------+---------+-----------+------------+
	  | .DATA   | 0x20000000 | 0x20007FFF | 32 KB   | 32 KB     |            |
      | .BSS    | 0x20008000 | 0x2000DFFF | 24 KB   | 18 KB     |            |
      | .TDAT   | 0x20008000 | 0x2000DFFF | 24 KB   | 18 KB     |            |
	  | .HEAP   | 0x2000E000 | 0x2000FFFF | 16 KB   | 8 KB      |            |
      | .STACK  | 0x20010000 | 0x20013FFF | 16 KB   | 4 KB      |            |
	  +---------+------------+------------+---------+-----------+------------+
      | FREE RAM TOTAL: 120 KB                                               |
      | Commands: s(snapshot) b(bank) q(quit)                                |
	  +----------------------------------------------------------------------+	  

      +------------------------[ BANK RAM2 DETAILS ]-------------------------+
	  | Section | Start      | End        | Size    | Usage     |            |
      +---------+------------+------------+---------+-----------+------------+
      | .ramDia | 0x10000000 | 0x10004000 |  4 KB   |  4 KB     |            |
	  | .sysDia | 0x10000000 | 0x10004000 |  4 KB   |  4 KB     |            |
	  +---------+------------+------------+---------+-----------+------------+
      | FREE RAM TOTAL: 60 KB                                                |                                      
      | Commands: s(snapshot) b(bank) q(quit)                                |
	  +----------------------------------------------------------------------+

      +-----------------------[ BANK CCSRAM DETAILS ]------------------------+
	  | Section | Start      | End        | Size    | Usage     |            |
      +---------+------------+------------+---------+-----------+------------+
      | .crit   | 0x10000000 | 0x10004000 |  4 KB   |  4 KB     |            |
	  +---------+------------+------------+---------+-----------+------------+
      | FREE RAM TOTAL: 60 KB                                                |                                      
      | Commands: s(snapshot) b(bank) q(quit)                                |
	  +----------------------------------------------------------------------+
   

### 6. XMC4200

#### 6.1 Task Overview

#### 6.2 Micrium µC/OS Architecture

#### 6.3 System initialization

#### 6.4 Normal start

#### 6.5 Fault start

#### 6.6 Normal operation

#### 6.7 💾 Memory Layout

##### 6.7.1 RAM Map

###### 6.7.1.1 RAM1

###### 6.7.1.2 RAM2

###### 6.7.1.3 CCSRAM

---

### 7. 🔌 Electrical Schematic

#### 7.1 Voltage regulation for logic part

The MCU and all logic components operate at 3.3 V.
To ensure stable and noise-free operation of both the microcontroller and peripheral ICs, a dedicated linear voltage regulator (TLV76133DCYR) is used.
The regulator provides a clean and stable 3.3 V output with a current capability of up to 1 A, offering sufficient headroom for transient loads and ensuring reliable power delivery even under dynamic system activity.

![Voltage regulation](/MainControlBoard/Media/VoltageRegulation.png)

#### 7.2 Gas Sensors

Gas sensors from the MQ family require an initial **preheating period** of approximately **48 hours** to stabilize their sensing elements. During normal operation, they follow a **heating cycle**: 60 seconds of heating followed by 90 seconds without heating. 

To safely control the heating element, it is recommended to use a **switching device**, such as a **BJT transistor**, capable of handling the required current.

According to the datasheets, the **maximum heater power** should not exceed **950 mW for MQ-6** and **900 mW for MQ-7**.

| Parameter                              | MQ-6  | MQ-7  |
|----------------------------------------|-------|-------|
| Minimum Heater Resistance (R<sub>h</sub>) | 23 Ω  | 26 Ω  |
| Maximum Heater Power (P<sub>max</sub>)   | 950 mW | 900 mW |
| Maximum Loop Voltage (V<sub>c</sub>)     | 5 V   | 5 V   |

![MQ-X Sensors](/MainControlBoard/Media/MQ_X_Sensors_Schematic.png)

#### 7.3 NRF24L01P 2.4GHz Transceiver

The NRF24L01P is used as a 2.4 GHz wireless transceiver, interfaced with the STM32G473RET6 via the SPI1 bus.

**Key features:**
- SPI communication (SPI1) with STM32G473RET6
- Dedicated IRQ line (active LOW) for event signaling
- Hardware-controlled power cycling via P-MOSFET (high-side switch)
- External antenna connection via SMA connector

**RF design:**
- Differential RF outputs (ANT1/ANT2) matched to single-ended 50Ω
- Matching network based on reference design (L/C topology)
- Matching components may require tuning depending on PCB stackup and antenna

**Power design:**
- NRF_VC is a switched 3.3V rail used to fully power-cycle the transceiver
- Local decoupling capacitors (100nF + 4.7µF) placed close to the device
- Dedicated filtering for VDD_PA to improve RF stability

![NRF24L01P](/MainControlBoard/Media/NRF24L01P_Schematic.png)

#### 7.4 L76L-M33 GPS

The L76L-M33 is used as an onboard GNSS (GPS) receiver module, interfaced with the STM32G473RET6 via UART.

**Key features:**
- UART communication with STM32G473RET6 (TX/RX)
- PPS (Pulse Per Second) output for precise timing
- Optional reset and standby control via GPIO
- External antenna connection via SMA connector

**RF design:**
- Single-ended RF input matched to 50Ω antenna
- Matching network (PI topology) based on reference design
- Matching components (0402) allow fine tuning depending on PCB stackup and antenna characteristics

**Power design:**
- Powered from 3.3V rail with local filtering
- Decoupling capacitors (100nF + 4.7µF) placed close to the module
- Optional ESD protection on RF line (not populated by default)

**Additional notes:**
- I2C interface is not used (optional pull-ups not populated)
- UART lines include series resistors for signal integrity and voltage adaptation

![L76L-M33](/MainControlBoard/Media/GPS_Schematic.png)

#### 7.5 STWD100NYWY3F external watchdog timer

An external watchdog is used to increase system reliability and ensure recovery from unexpected failures.

**Purpose:**
- Automatic reset of the STM32 in case of:
  - Software faults (e.g. deadlock, missed tasks)
  - Hardware-related issues causing system freeze

**Control and flexibility:**
- Watchdog is periodically fed by the MCU (WATCHDOG_FEED signal)
- Dedicated enable line (WATCHDOG_EN) allows firmware control over watchdog activation
- Hardware jumper allows manual disconnection of the watchdog

**Design approach:**
- Dual control mechanism (MCU enable + jumper) provides redundancy and flexibility during development and debugging
- Ensures safe bring-up and easy fault isolation

**Notes:**
- Watchdog output is connected to the system reset line
- Series resistor used to isolate reset sources and prevent contention

![Watchdog](/MainControlBoard/Media/STWD100NYWY3F_Schematic.png)

#### 7.6 FM25L16B-GTR FRAM

The FM25L16B FRAM is used for non-volatile data storage.

**Purpose:**
- Storage of failure logs and diagnostic data
- Telemetry data logging
- Optional storage of GPS position and system state

**Communication:**
- SPI interface (connected to STM32)
- Series resistor on MISO line for signal integrity

**Configuration:**
- HOLD and Write Protection (WP) signals are not used
- Device operates in standard SPI mode

**Power:**
- Powered from +3.3V
- Local decoupling capacitor for stable operation

**Notes:**
- FRAM allows fast, low-latency writes without wear limitations typical for Flash memory

![FM25L16B](/MainControlBoard/Media/FM25L16B_Schematic.png)

#### 7.7 ADXL-345 Accelerometer

The ADXL345 is a 3-axis digital accelerometer used for motion sensing.

**Purpose:**
- Measurement of acceleration in three axes
- Determination of vehicle orientation in space

**Communication:**
- I2C interface (connected to STM32)

**Notes:**
- Used for motion analysis, tilt detection and dynamic behavior monitoring
- Interrupt pins (INT1, INT2) are not used in this design

![ADXL345](/MainControlBoard/Media/ADXL345_Schematic.png)

#### 7.8 Buzzer

The buzzer is used for basic acoustic feedback and system indication.

**Purpose:**
- System diagnostics and status indication
- Signaling events such as system state changes or errors
- Indication of radio link status and other runtime conditions

**Control:**
- Driven by STM32 using PWM signal
- Controlled via transistor (low-side switch)

**Notes:**
- Allows generation of different tones and patterns for various system events

![Buzzer](/MainControlBoard/Media/Buzzer_Schematic.png)

#### 7.9 STM32G473RET6

The STM32G473RET6 serves as the main system controller.

**Responsibilities:**
- System supervision and overall control
- Communication with external devices (SPI, UART, I2C)
- Telemetry handling and data exchange
- Radio communication via NRF24L01+
- Delegating real-time motor and servo control to XMC4200

**Peripherals and interfaces:**
- I2C2: communication with ADXL345 accelerometer and external I2C devices
- Multiple SPI interfaces for RF and external modules
- UART interfaces for GPS and debugging

**Additional functions:**
- Data acquisition from MQ-6 and MQ-7 gas sensors and other devices
- Control of a diagnostic buzzer

**Design notes:**
- NRST line supports multiple reset sources (button, watchdog, programmer)
- BOOT0 is configured as GPIO (USART3) and pulled low to ensure boot from Flash
- Pins marked as NC_x are reserved for future use and design flexibility

![STM32G473RET6](/MainControlBoard/Media/STM32G473RET6_Schematic.png)

#### 7.10 XMC4200F64K256BAXQSA1

The XMC4200 is used as a dedicated real-time hardware control MCU.

**Responsibilities:**
- Control of DC motors and servo mechanisms
- Generation of PWM signals using CCU4 and CCU8 timers
- Offloading time-critical control tasks from the main STM32 MCU

**Peripherals and interfaces:**
- SPI interface (slave) connected to STM32 (master)
- I2C interface for communication with external devices
- UART interface used for diagnostics and debugging

**Timers and I/O:**
- CCU4 / CCU8 used for PWM generation and timing-critical control
- Multiple GPIOs reserved for future expansion and control signals

**Power design:**
- Supply network implemented according to XMC4200 reference design
- Local decoupling capacitors for stable operation

**Reset:**
- Reset can be triggered by:
  - STM32 (external control)
  - Programmer/debugger
  - Manual reset (button, if present)
- Proper isolation used to prevent contention between reset sources

**Analog section:**
- Analog peripherals are not used in this design
- Analog supply pins are properly connected and decoupled as required

**Design notes:**
- Designed as a co-processor for deterministic, real-time control
- Separates high-frequency control tasks from main system logic
- Improves system stability and timing predictability

![XMC4200](/MainControlBoard/Media/XMC4200_Schematic.png)

#### 7.11 Programming and Debug Interfaces

The board provides programming and debugging interfaces for both MCUs using modified 10-pin Cortex-M debug connectors (2x5, 2.54 mm).

**STM32 (J2):**
- SWD interface (SWDIO, SWCLK)
- USART3 available for debugging and communication
- Dedicated reset line (STM32_RST)

**XMC4200 (J1):**
- JTAG interface (TMS, TCK)
- UART (USIC0_CH0) for diagnostics
- External reset line (INFINEON_RST)

**Connector pinout:**
- Based on standard Cortex-M 10-pin debug connector
- Pins KEY, GNDDetect, NC/TDI and SWO/TDO are not used in standard form
- SWO/TDO replaced with UART TX
- NC/TDI replaced with UART RX

**Design notes:**
- Combined debug and UART access in a single connector
- +3.3V and GND provided for debugger reference

![Debug](/MainControlBoard/Media/Debug_Schematic.png)

For reference, the official Cortex-M 10-pin debug connector pinout is shown below:

![CORTEX-M-DEBUG_CONN](/MainControlBoard/Media/cortex_debug10pin.png)

#### 7.12 Hardware Control Connector

This connector provides the interface between the Main Control Board and the external **HardwareControlBoard**.

The connection is implemented using a high-density board-to-board / cable interface:
- Connector: **ETMM-122-02-F-D-TH (Samtec)**
- Mating connection: multi-wire ribbon cable (Samtec compatible)

> *(Exact mating cable/connector depends on final harness selection – placeholder used for now.)*

**Signal groups:**
- **Control signals:**
  - `FIRE_GUN`, `ARM_GUN`, `KILL_SWITCH`
- **Timer outputs (motor/servo control):**
  - `CCU80_OUT_x`
  - `CCU40_IO_x`
  - `CCU41_IO_x`
- **Digital I/O:**
  - `DIGITAL_IO_x`
- **Communication buses:**
  - `I2C (SCL, SDA)`
  - `USIC I2C (U1C0)`
- **Reserved / future use:**
  - `NC_x` lines (design expansion)

**Configuration flexibility:**
Most signals are routed through **solder jumpers (SJx)**:
- Allow signal rerouting / isolation
- Enable quick reconfiguration during bring-up and testing
- Provide hardware-level debugging options

**Grounding:**
Multiple **GND pins** are distributed across the connector to:
- Provide solid reference for all signals
- Reduce noise and improve signal integrity
- Support higher current return paths (especially for motor control signals)

---

This connector acts as the primary hardware abstraction boundary between:
- High-level control logic (Main Control Board)
- Low-level actuation and power electronics (HardwareControlBoard)

![HardwareControlConnector](/MainControlBoard/Media/HardwareControlConnector.png)

![Samtec ETMM Series](/MainControlBoard/Media/samtec_etmm_series.png)

#### 7.13 PCB Stack SPI Interface

This connector provides the **inter-board communication interface** within the PCB stack.

It is used to connect the Main Control Board with additional stacked modules (e.g. FPGA, RENESANS, or NXP self made PCB's).

**Connector implementation:**
- Connector type: **2x15, 2.54mm pitch**
- Stack connection: **Samtec ESQ series (stacking headers)**
- Designed for vertical PCB stacking


**Signal groups:**

- **SPI bus (shared):**
  - `SPI3_MOSI`
  - `SPI3_MISO`
  - `SPI3_SCK`

- **Chip Select lines:**
  - `FPGA_CS`
  - `NXP_CS`
  - `RENESANS_CS`

- **Control / status signals:**
  - `FPGA_RDY`, `FPGA_RST`
  - `NXP_RDY`, `NXP_RST`
  - `RENESANS_RDY`, `RENESANS_RST`

- **Reserved lines:**
  - `NC_x` for future expansion

**Power distribution:**
- `+5V` routed through the connector for stacked modules
- Multiple **GND pins** distributed across the connector to:
  - Ensure signal integrity
  - Provide low-impedance return paths
  - Reduce crosstalk between SPI lines

**Design notes:**
- Ground pins are interleaved with signals to improve EMI performance
- SPI bus is shared, with dedicated CS lines per device
- Topology allows adding/removing stack modules without redesign

This interface enables a **modular stacked architecture**, allowing the system to scale with additional processing or peripheral boards.

![PCBStackSPI](/MainControlBoard/Media/PCB_Stack_SPI.png)

---

### 8. 🧩 PCB

The PCB is designed as a **multi-layer mixed-signal board**, integrating digital control, RF communication, and power distribution in a compact form factor.

**General characteristics:**
- 4-layer stackup (signal / GND / power (3.3V) / signal)
- Continuous ground plane for signal integrity and RF performance
- Separation of functional blocks (RF, MCU, power, sensors)

**Design approach:**
- Modular architecture:
  - Main Control Board
  - Hardware Control Board (external)
  - Stackable expansion boards (SPI stack interface)

- Signal routing optimized for:
  - Short critical paths (SPI, RF, clock)
  - Minimal via usage where possible
  - Clean return paths via GND plane

**RF considerations:**
- Controlled impedance traces (50Ω where required)
- Matching networks implemented with tuning capability (PI topology)
- Provision for external antenna via coaxial connector

**Power integrity:**
- Local decoupling for all ICs (100nF + bulk capacitors)
- Separate analog/digital domains where applicable
- Power switching for RF module (NRF24L01+)

**Debug & testability:**
- Dedicated debug connectors (modified Cortex-M 10-pin)
- Test points for critical signals (SPI, UART)
- Solder jumpers (SJx) for configuration and isolation

**Assembly & prototyping:**
- Standard SMD packages (0402/0603 where applicable)
- Design allows partial population (RF tuning, optional features)
- Silkscreen optimized for readability and debugging

The PCB is designed with a focus on **reliability, modularity, and ease of debugging**, making it suitable for both prototyping and further system expansion.

#### 8.1 PCB Preview

Top layer view:
![PCB Top](/MainControlBoard/Media/PCB_Top.png)

Bottom layer view:
![PCB Bottom](/MainControlBoard/Media/PCB_Bottom.png)

> *Images will be updated after final routing and manufacturing.*

#### 8.2 Assembly Notes

**Solder jumpers (SJ1–SJ28):**
- Located on the bottom side of the PCB
- Allow insertion of series resistors or direct short (0Ω)
- Provide flexibility for signal tuning, isolation and debugging

**Pull-up resistors (I2C and related):**
- R4, R5, R21, R22 default value: **2.2kΩ**
- If signal lines are short, values up to **4.7kΩ or higher** can be used

**SPI series resistors:**
- R15, R16, R17, R18, R19, R28, R29, R30, R31
- Default value: **22Ω**
- May be adjusted depending on signal integrity requirements

**GPS section (RF tuning and optional components):**
- C32, C34: not populated (DNP)
  - Footprints provided for RF matching/tuning if required
- R49, R50: not populated (DNP)
  - Added as optional protection / stabilization (e.g. floating pins)

**SWD / debug series resistors:**
- R10, R32, R39, R40
- Default value: **10Ω**
- May be adjusted depending on signal integrity and cable length

**NRF24L01+ section (RF components):**
The inductors used in this section are from the Murata LQG15WH series (0402 RF inductors).
Due to their frequency-dependent behavior, the nominal inductance value specified in the BOM does not directly correspond to the effective impedance at GHz frequencies.

At high frequencies (2.4–2.525 GHz), parasitic effects such as series resistance and capacitance significantly influence the component behavior. As a result, the effective inductance at the operating frequency may differ from the nominal value measured at low frequency.

Because of this, component selection must be based on manufacturer RF characteristics (Q factor, impedance vs frequency, and SRF), rather than nominal inductance alone.

In practice, slightly lower nominal inductance values are selected to compensate for frequency-dependent behavior of the components, ensuring that the effective impedance at 2.4 GHz matches the intended design.
- L1: nominal value 8.2 nH → selected value 6.8 nH  
- L3: nominal value 3.9 nH → selected value 3.7 nH

**STM32 MCU substitution:**
- U1 (STM32G473RET6)
- Due to limited availability, **STM32G474RET6** is used instead
- Fully pin-compatible and firmware-compatible with STM32G473RET6
- No hardware modifications required

**General notes:**
- Several components are intentionally left as DNP to allow flexibility during bring-up
- Final values may be adjusted after testing and signal validation

#### 8.3 BOM (Bill of Materials)

## 📦 Bill of Materials (BOM)

| Designator | Value | Tolerance | Footprint | Quantity | Mouser No | Notes |
|------------|-------|-----------|------------|----------|------------|--------|
| C1 | 1µF | 5% | C1206 | 1 | - | Power supply filtering |
| C2 | 100µF | 5% | CAP-D5.0-F2.0 | 1 | - | Bulk input capacitor |
| C3, C4 | 100nF | 5% | C1206 | 2 | - | General decoupling |
| C5, C6, C7, C9, C10, C12, C13, C15, C16, C17, C18, C23, C29, C31, C36 | 100nF | 5% | C0603 | 15 | - | Decoupling capacitors |
| C8, C14, C30, C35 | 4.7µF | 5% | C0603 | 4 | - | Bulk capacitors |
| C11, C22 | 1µF | 5% | C0603 | 2 | - | Analog supply filtering |
| C19, C20 | 22pF | 5% | C0603 | 2 | - | Crystal load capacitors |
| C21 | 10nF | 5% | C0603 | 1 | - | Analog filtering |
| C24, C32, C34 | 1.5pF | 1% | C0402 | 3 | - | RF matching network (tunable) |
| C25 | 1.0pF | 1% | C0402 | 1 | - | RF matching network (tunable) |
| C26 | 2.2nF | 1% | C0402 | 1 | - | RF matching network (tunable) |
| C27 | 4.7pF | 1% | C0402 | 1 | - | RF matching network (tunable) |
| C28 | 33nF | 5% | C0603 | 1 | - | DVDD filtering |
| C33 | 10µF | 5% | C0603 | 1 | - | Bulk capacitor |
| CN1 | ETMM_122_02_F_D_TH | - | ETMM-122-02-F-D-TH | 1 | 200-ETMM12202FDTH | External board connector (ribbon cable) |
| CON2 | MSTB2.5/2-G-5.08 | - | CONN-TH_2P-P5.08_MSTB2.5 | 1 | - | Power input connector |
| D1 | SP0402B-ULC-01ETG | - | DFN-2L_L1.0-W0.6-P0.65-BI | 1 | 576-SP0402BULC-01ETG | ESD protection (not populated by default) |
| J1, J2 | HDR-M-2.54_2x5 | - | HDR-M-2.54_2X5 | 2 | - | Programming connectors (modified Cortex-M 10-pin) |
| J3 | HDR-M-2.54_2x15 | - | HDR-M-2.54_2X15 | 1 | - | SPI stack connector between boards |
| J4 | HDR-M-2.54_1x3 | - | HDR-M-2.54_1X3 | 1 | - | Auxiliary connector |
| L1 | 8.2nH | 1% | L0402 | 1 | - | RF matching network (tunable) |
| L2 | 2.7nH | 1% | L0402 | 1 | - | RF matching network (tunable) |
| L3 | 3.9nH | 1% | L0402 | 1 | - | RF matching network (tunable) |
| LED1 | LED-0603_R | - | LED0603_RED | 1 | - | Power/status indicator |
| Q1 | IRF7380TRPBF-VB | - | SOP-8_L4.9-W3.9-P1.27-LS6.0-BL | 1 | 726-IRF7380TRPBFXTMA | MOSFET for MQ heater control |
| Q2 | BC547B | - | TO92-INLINE | 1 | - | General purpose transistor |
| Q3 | SI2365EDS-T1-GE3 | - | SOT-23-3_L2.9-W1.3-P1.90-LS2.4-BR | 1 | 78-SI2365EDS-T1-BE3 | P-channel MOSFET (RF power control) |
| R1 | 1kΩ | 5% | R1206 | 1 | - | Crystal bias resistor |
| R2, R8, R9, R11, R12, R23, R25, R26, R35, R36, R37, R51 | 10kΩ | 5% | R0603 | 12 | - | Pull-up / pull-down resistors |
| R3, R6, R7, R42 | 4.7kΩ | 5% | R0603 | 4 | - | I2C pull-ups / biasing |
| R4, R5, R21, R22, R34 | 2.2kΩ | 5% | R0603 | 5 | - | Signal conditioning |
| R10, R32, R39, R40 | 10Ω | 5% | R0603 | 4 | - | SWD signal damping |
| R13, R20, R41, R43, R49, R50 | 47kΩ | 5% | R0603 | 6 | - | Pull resistors (R49, R50 not populated by default) |
| R14 | 10kΩ | 5% | R1206 | 1 | - | Bias resistor |
| R15, R16, R17, R18, R19, R28, R29, R30, R31 | 22Ω | 5% | R0603 | 9 | - | SPI series resistors |
| R24 | 22kΩ | 5% | R0603 | 1 | - | IREF resistor |
| R27 | 1MΩ | 5% | R1206 | 1 | - | Bias resistor |
| R33, R47 | 1kΩ | 5% | R0603 | 2 | - | UART series resistors |
| R38, R44, R45, R48 | 100Ω | 5% | R0603 | 4 | - | Signal damping |
| R46 | 0Ω | 1% | R0402 | 1 | - | RF path jumper (tunable) |
| RF1, RF2 | SMA-J-P-H-RA-TH1 | - | ANT-TH_SMA-J-P-H-RA-TH1 | 2 | 200-SMAJPHRATH1 | External antenna connectors |
| SJ1–SJ28 | 0Ω (configurable) | 5% | R0603 | 28 | - | Jumpers for external signals (optional tuning / configuration) |
| SPEAKER | BUZZER | - | BUZZER-12MM-NS-KIT | 1 | - | Buzzer for diagnostics |
| SW1, SW2 | Button-6x6x8-Right-Angle | - | SW-TH_BBJ-C71833 | 2 | - | MCU reset buttons |
| U1 | STM32G473RET6 | - | LQFP-64_L10.0-W10.0-P0.50-LS12.0-BL | 1 | 511-STM32G473RET6 | Main MCU |
| U2 | L76L-M33 | - | COMM-SMD_18P-L10.1-W9.7-P1.1-TL_L76-LB | 1 | 277-L76L-M33 | GNSS (GPS) module |
| U3 | XMC4200F64K256BAXQSA1 | - | LQFP-64_L10.0-W10.0-P0.50-LS12.0-TL-EP5.7 | 1 | - | Motor control MCU |
| U4 | MQ-7 | - | SENSOR-TH_MQ-9B | 1 | - | CO gas sensor |
| U5 | MQ-6 | - | SENSOR-TH_MQ-4 | 1 | - | LPG gas sensor |
| U6 | TLV76133DCYR | - | SOT-223-4_L6.5-W3.5-P2.30-LS7.0-TR | 1 | 595-TLV76133DCYR | 3.3V LDO regulator |
| U7 | nRF24L01P-R | - | QFN-20_L4.0-W4.0-P0.50-BL-EP | 1 | 949-NRF24L01P-T | 2.4 GHz RF transceiver |
| U8 | FM25L16B-GTR | - | SOIC-8_L4.9-W3.9-P1.27-LS6.0-BL | 1 | - | SPI FRAM memory |
| U9 | STWD100NYWY3F | - | SOT-23-5_L3.0-W1.7-P0.95-LS2.8-BR | 1 | 511-STWD100NYWY3F | Watchdog supervisor |
| U10 | ADXL345BCCZ-RL7 | - | LGA-14_L5.0-W3.0-P0.80-BL | 1 | 584-ADXL345BCCZ-R7 | 3-axis accelerometer |
| X1 | XTAL-16M | - | CRYSTAL-TH_L10.8-W4.5-P4.88 | 1 | - | RF reference crystal |
