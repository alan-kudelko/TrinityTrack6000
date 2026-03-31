# **ControlBoard** – **STM32 + Infineon**  

**STM32**: Main logic and system coordination, communication via radio and Bluetooth
**Infineon**: Motor and servo control, current and temperature monitoring

The system runs on a custom PCB integrating **STM32** and **Infineon** microcontrollers, designed with **fully deterministic memory allocation** and task management.

- **STM32** leverages **ThreadX RTOS** for high-level coordination
- **Infineon** uses **Micrium µC/OS RTOS** for precise motor and servo control

All tasks on STM32 and Infineon MCUs are allocated with **guard zones** in RAM, using custom **linker scripts** to arrange task stacks and guard areas contiguously. This enables continuous memory monitoring and deterministic stack overflow detection, fully compatible with static memory allocation principles.  

The project adheres to **MISRA C:2025** standards, ensuring safe, maintainable, and portable code. The ControlBoard includes **robust diagnostics and fault-handling mechanisms**, such as monitoring RAM and CPU usage, task stack overflows, and EEPROM-based error logging.

> ✅ Status: Currently in active development and prototyping  
> 🔧 Goal: Serve as an educational platform for learning **STM32**, **Infineon MCUs**, and real-time systems **ThreadX** and **Micrium µC/OS**, while exploring multi-MCU coordination and embedded system design  

---

## ✳️ Planned Technologies & Tools

- **MCUs**: STM32 (high-level coordination, RTOS), Infineon XMC (motor & servo control, real-time monitoring)
- **RTOS**: ThreadX on STM32, Micrium µC/OS on Infineon
- **Memory Management**: Guard zones, static memory allocation, custom linker scripts
- **Interfaces**: UART, SPI, and I²C for peripheral communication
- **Communication Modules**: nRF24L01 for radio control, HC-05 for bluetooth control
- **Development Tools**: VS Code, CMake, Ninja, arm-gcc toolchain, STM32CubeIDE, DAVE IDE
- **Software Libraries**: STM32 HAL/LL drivers, Infineon low-level drivers
- **Diagnostics & Monitoring**: RAM and CPU usage tracking, EEPROM fault logging

## Development Strategy

- **Prototyping Phase:** Initial development and testing are conducted on a **Nucleo board with STM32L476RGT6**, using **STM32 HAL**, and on the **KIT_XMC45_RELAX_LITE_V1** Evaluation board for Infineon XMC series prototyping. This allows fast iteration and hardware abstraction during early stages.

- **Production Phase:** The final implementation will be migrated to a **custom PCB** featuring:
  - **STM32G473CET6** as the main coordinator / master MCU,
  - **Infineon XMC4200F64K256BAXQSA1** for peripheral / slave functionality.  
  Development will use **bare-metal / low-level drivers**, focusing on performance optimization, resource efficiency, and full control over hardware-level operations.


## Key Features

- Modular software architecture
- Integration of multiple sensors and communication interfaces
- Scalable design for both prototyping and production
- Hybrid approach: HAL for fast development, LL/bare-metal for final product optimization
  
### 🔌 Hardware & Electronics
- **Nucleo STM-64 STM32L4RGT6 board** for rapid prototyping and learning STM32
- **Infineon KIT_XMC45_RELAX_LITE_V1 evaluation board** for raping prototyping and learning Inineon XMC MCU series
- Custom PCB hosting:
  - **STM32G473CET6 MCU** – Handles high-level coordination tasks and system management
  - **Infineon XMC4200F64K256BAXQSA1 MCU** – Controls precise motor and servo operations with real-time current monitoring
  - **nRF24l01** wireless module
  - **FM25L16B-GTR** FRAM IC for storing data and settings for MCUs between bootups
  - **MQ-6** LPG / flammable gas sensor
  - **MQ-7** Carbon monoxide gas sensor

### 🧠 System Architecture & Concepts
- Real-time system based on **ThreadX** (STM32) and **Micrium µC/OS** (Infineon)
- Static memory allocation only — no malloc, no heap fragmentation
- Custom linker scripts with dedicated memory sections for task stacks and **guard zones**
- Preemptive multitasking with structured task separation
- Guard zone-based stack overflow detection for all tasks
- EEPROM-based error logging and system state preservation
- Fault recovery logic with planned automatic restart on critical errors

### 💬 Input / Output Interfaces
- UART serial interface for remote monitoring, control, and debugging
- I²C, SPI, UART for peripheral integration
- Wireless interface: nRF24L01 for remote control and telemetry

### 🧰 Development Tools & Libraries
- **C / C++** for firmware development
- Native MCU libraries with low-level register access
- **ThreadX** for STM32, **Micrium µC/OS** for Infineon
- **VS Code, CMake, Ninja** for building and managing the project
- **Doxygen** for automatic code documentation
- **XMC Pinout Tool** for selecting pins on XMC4200 MCU

### 📚 Documentation
The source code is fully documented using **Doxygen**, which generates up-to-date, browsable documentation from the annotated source files. The generated docs can be found in the `docs/MainControlBoard` directory and are updated as part of the build process.

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
- 🚀 Benchmark RAM bank variable placement and assembly-level optimizations to maximize performance
- ⚡ Benchmark FPU/ALU instruction interleaving using manual ASM scheduling to explore cycle-level parallelism
  
---

## 🗺️ Prototyping Roadmap  

### Common

- 🔄✅ Plan out overall logic of **ControlBoard** along with used components
- 🔄 Confirm successful programming and communication of STM32 and Infineon XMC4200 MCUs on breadboard, together with peripheral components and sensors.

### STM32

- ✅ Migrate STM32 project to VS Code, CMake, and Ninja build system with the `arm-gcc` toolchain and HAL files generated by STM32CubeIDE
- ✅ Fix the **Core blocked** issue after uploading the `.elf` file generated by CMake
- ✅ Verify `.elf` and `.map` builds with debug symbols for flashing and debugging
- ✅ Modify the CMake configuration to support selecting either **Debug** or **Release** build types
- ✅ Modify linker script to add extra symbols and sections for using alternative RAM banks (RAM2, CCMRAM) and diagnostics
- 🔄 Implement initialization functions to initialize STM32
- 🔄✅ Implement a diagnostic function to display RAM usage over UART, including `.bss`, `.data`, `.heap`, `.stack`, and other linker sections such as `.tdat`

### Infineon

- ✅ Migrate Infineon project to VS Code, CMake, and Ninja build system with the `arm-gcc` toolchain and XMC files generated by DAVE IDE


## 🗺️ Production Roadmap

- ✅ Migrate project to VS Code, CMake, and Ninja build system with the `arm-gcc` toolchain and HAL files generated by STM32CubeIDE with support of **Debug** and **Release** build types
- ✅ Verify `.elf` and `.map` builds with debug symbols for flashing and debugging

---

## Technical Table of Contents

1. [ControlBoard Module Structure & File Overview](#1--controlboard-module-structure--file-overview)
2. [Design considerations](#2-design-considerations)
3. [MCU's pinouts](#3-mcus-pinouts)
   - [3.1 STM32G473CET6 Pinout (LQFP-48)](#31-stm32g473cet6-pinout-lqfp-48)
   - [3.2 XMC4200F64K256BAXQSA1 Pinout (TQFP-64)](#32-XMC4200F64K256BAXQSA1-pinout-tqfp-64)
4. [System's architecture](#4-systems-architecture)
5. [STM32G473CET6](#5-stm32g473cet6)
   - [5.7 💾 Memory Layout](#57--memory-layout)
     - [5.7.1 RAM Map](#571-ram-map)
     - [5.7.2 Custom RAM segments](#572-custom-ram-segments)
     - [5.7.3 Free Memory Calculation](#573-free-memory-calculation)
     - [5.7.4 RAM Usage Overview](#574-ram-usage-overview)
   - [5.8 MCU Diagnostics](#58-mcu-diagnostics)
     - [5.8.1 ThreadX Tasks Diagnostics](#581-threadx-tasks-diagnostics)
     - [5.8.2 RAM Usage Diagnostics](#582-ram-usage-diagnostics)
6. [Infineon](#6-infineon)
---

## ⚙️ Technical Overview 

### 1. 📦 ControlBoard Module Structure & File Overview

    📦 TrinityTrack6000/
 	│
 	├── Datasheets/              # Documentation of used ICs and components
	├── docs/                    # Documentation generated by Doxygen for each MCU's files
	│
	├── MainControlBoard/    
	|   ├── STM32/                   # STM32 MCU's files
	|   |   ├── .metadata/           # STM32CubeIDE workspace files folder
	|   |   ├── STM32L476RGT6/       # STM32 prototyping files
 	│   │   |   ├── .settings/
   	│   │   |   ├── .vscode/         # Visual Studio Code configuration files
   	│   │   |   ├── build/           # Build output directory (generated by CMake + Ninja)
	│   │   |   ├── Core/
  	│   │   |   ├── Drivers/
   	│   │   |   ├── Include/         # Project include files
	│   │   |   ├── Init/            # Initialization includes and sources
    │   │   |   ├── Src/             # Project source files
	│   │   |   ├── Utils/           # Helper functions
 	│   │   |   ├── .cproject
  	│   │   |   ├── .mxproject
   	│   │   |   ├── .project
	│   │   |   ├── CMakeLists.txt             # CMake build system configuration for STM32L476RGT6
  	│   │   |   ├── STM32L476RGT6 Debug.launch
    │   │   |   ├── STM32L476RGT6.ioc
	│   │   |   └── STM32L476RGTX_FLASH.ld     # Linker script with custom memory sections	
	|   |   |
	|   |   ├── STM32G473CET6/       # STM32 target MCU production files
	|   |   |
	|   |   ├── Readme.md            # MainControlBoard overview and documentation
	|   |   |
	│   |   └── ... (other files)    # Yet to be added after prototyping phase
	|   |
	| 	└── Infineon/                # Infineon MCU files
	|
  	├── Media/                       # Folder containing all the drawings and schematics
    |
	├── tools/                       # Helper tools
 	│   └── arm-gnu-toolchain-14.3.rel1-mingw-w64-x86_64-arm-none-eabi/          # arm-gcc toolchain                             
    |
 	├── .gitignore            # Git ignore configuration
	├── LICENSE               # Project license file
	└── README.md             # Project overview and documentation

---

### 2. Design considerations

#### 2.1 MCU Responsibilities / Scope of Control

Due to the complexity of the system, it is essential to clearly define the responsibilities and scope of each MCU during the theoretical design stage to ensure proper modularization, communication efficiency, and maintainability. During the design phase, it is difficult to predict all possible scenarios or critical connections, which is why the design process follows an iterative improvement loop.

##### 2.1.1 STM32's Responsibilities

Role of this MCU is high level system supervision, it is basically the brain of this project. It coordinates all the MCUs together, performs their reset in case of response timeout. The multi-MCU communication will be held by a dedicated high-speed SPI interface alongside dedicated CS pins and reset pins, which will enable restart of the module in case of fault. Additionally, it handles wireless communication and reads data from all the sensors which are not related to power electronics. It also controls the system's buzzer indicating potential faults and tone generation. This MCU also communicates with an on-board FRAM module to store important data between startups such as errors.

*List of responsibilities of this MCU*
- 1. System high-level supervision and synchronization
- 2. Communication – sending and receiving data from all system MCUs
- 3. Wireless communication via NRF24L01 2.4 GHz module
- 4. Reading data from L76K GPS module
- 5. Reading data from MQ-6 LPG / flammable gas sensor
- 6. Reading data from MQ-7 carbon monoxide gas sensor
- 7. Reading data from ADXL345 digital accelerometer
- 8. Reading outside temperature and humidity from (TBD)
- 9. Reading and saving data to FM25L16B-GTR FRAM
- 10. Control of low-power electronics such as LEDs, lighting via MCP23S17 and system buzzer
- 11. Arming, disarming and firing control for the BB gun mounted in the turret

---

##### 2.1.2 Infineon's Responsibilities

In this project, the Infineon MCU will be responsible for all the hardware control including generating PWM for DC motors, servos, and the heater used in the smoke generator. Additionally, this MCU will process all the corresponding measurements of motor temperature, current, and in some cases their rotational speed via encoder signals. This MCU will control all the power-demanding components of the system.

*List of responsibilities of this MCU*
- 1. Generating control signals for DC motors and servos
- 2. Reading position or speed of DC motors or high-precision servos (used in **RadarModuleBoard** or **FireControlBoard**)
- 3. Reading temperature and current from ADCs installed on **HardwareControlBoard** via dedicated I²C bus
- 4. Controlling electrical heater and glycerin pump used in the smoke generator
- 5. Monitoring current limits and implementing basic fault protection for high-power components
- 6. Processing encoder feedback from the radar servo for precise positioning

---

### 3. MCU's pinouts

Due to the large number of pins, I decided to use colors to group them.  
Colors and hardware related to them are described below:
- 🟠 UART2 - Communication with L76K GPS
- 🔵 SPI2, GPIO - Inter-MCU communication pins and MCU's reset pins
- 🟢 I²C - Communication with ADXL345 accelerometer
- 🟣 SPI1 - Communication with FM25L16B-GTR FRAM, NRF240L01 radio module and MCP23S17 pin expander
- 🔴 ST-Link, UART1 - Debug, SWD, Programming 
- ⚪ Unused
- 🟡 GPIO - used to control various tank components such as LED's, smoke generator etc. 
- 🔷 RTC / Backup / LPTIM  


#### 3.1 STM32G473CET6 Pinout (LQFP-48)

| #  | Pin / Function                                                                                    | Usage/Label                            | Description                              |
|----|---------------------------------------------------------------------------------------------------|----------------------------------------|------------------------------------------|
| 1  | VBAT                                                                                              | 🔷 Supply for RTC and backup registers | Used to power Backup Domain with CR2032  |
| 2  | PC13 / TIM1_BKIN / TIM1_CH1N / TIM8_CH4N / EVENTOUT / WKUP2 / RTC_TAMP1 / RTC_TS / RTC_OUT1       | 🟡 GPIO BUZZER                         | Used to generate PWM for on-board buzzer |
| 3  | PC14 / EVENTOUT / OSC32_IN                                                                        | 🔷 Quartz 32kHz                        | Used to provide high precision RTC       |
| 4  | PC15 / EVENTOUT / OSC32_OUT                                                                       | 🔷 Quartz 32kHz                        | Used to provide high precision RTC       |
| 5  | PF0 / I2C2_SDA / SPI2_NSS/I2S2_WS / TIM1_CH3N / EVENTOUT / ADC1_IN10 / OSC_IN                     | 🟡 GPIO - ARM BB Gun                   | Used to arm/enable BB gun                |
| 6  | PF1 / SPI2_SCK/I2S2_CK / EVENTOUT / ADC2_IN10 / COMP3_INM / OSC_OUT                               | 🟡 GPIO - Fire BB Gun                  | Used to fire bb gun                      |
| 7  | PG10 / MCO / EVENTOUT / NRST                                                                      | MCU's reset                            | Used to reset the MCU either by button or ST-Link programmer |
| 8  | PA0 / TIM2_CH1 / TIM5_CH1 / USART2_CTS / COMP1_OUT / TIM8_BKIN/ TIM8_ETR / TIM2_ETR / EVENTOUT / ADC12_IN1 / COMP1_INM / COMP3_INP / RTC_TAMP2 / WKUP1 | 🟡 ADC12_IN1             | Used to read analog data from MQ-7 Carbon Monoxide sensor   |
| 9  | PA1 / RTC_REFIN / TIM2_CH2 / TIM5_CH2 / USART2_RTS_DE / TIM15_CH1N / EVENTOUT / ADC12_IN2 / COMP1_INP / OPAMP1_VINP / OPAMP3_VINP / OPAMP6_VINM        | 🟡 ADC12_IN2             | Used to read analog data from MQ-6 LPG/Flammable gas sensor |
| 10 | PA2 / TIM2_CH3 / TIM5_CH3 / USART2_TX / COMP2_OUT / TIM15_CH1 / QUADSPI1_BK1_NCS / LPUART1_TX / UCPD1_FRSTX / EVENTOUT / ADC1_IN3 / COMP2_INM / OPAMP1_VOUT / WKUP4/LSCO           | 🟠 USART2_TX | Used for communication with GPS module |
| 11 | PA3 / TIM2_CH4 / TIM5_CH4 / SAI1_CK1 / USART2_RX / TIM15_CH2 / QUADSPI1_CLK / LPUART1_RX / SAI1_MCLK_A / EVENTOUT ADC1_IN4 / COMP2_INP / OPAMP1_VINM/OPAMP / 1_VINP / OPAMP5_VINM  | 🟠 USART2_RX | Used for communication with GPS module |
| 12 | PA4 / TIM3_CH2 / SPI1_NSS / SPI3_NSS/I2S3_WS / USART2_CK / SAI1_FS_B / EVENTOUT / ADC2_IN17 / DAC1_OUT1 / COMP1_INM                                                       | 🟡 GPIO - MQ_HEATERS  | Used to control BD-135 MQ sensors heaters transistor     |
| 13 | PA5 / TIM2_CH1 / TIM2_ETR / SPI1_SCK / UCPD1_FRSTX / EVENTOUT / ADC2_IN13 / DAC1_OUT2 / COMP2_INM / OPAMP2_VINM                                                           | 🟣 SPI1_SCK    | Used for communication with nRF24L01, MCP23S17 and FM25L16B-GTR |
| 14 | PA6 / TIM16_CH1 / TIM3_CH1 / TIM8_BKIN / SPI1_MISO / TIM1_BKIN / COMP1_OUT / QUADSPI1_BK1_IO3 / LPUART1_CTS / EVENTOUT / ADC2_IN3 / DAC2_OUT1 / OPAMP2_VOUT               | 🟣 SPI1_MISO   | Used for communication with nRF24L01, MCP23S17 and FM25L16B-GTR |
| 15 | PA7 / TIM17_CH1 / TIM3_CH2 / TIM8_CH1N / SPI1_MOSI / TIM1_CH1N / COMP2_OUT / QUADSPI1_BK1_IO2 / UCPD1_FRSTX / EVENTOUT / ADC2_IN4 / COMP2_INP / OPAMP1_VINP / OPAMP2_VINP | 🟣 SPI1_MOSI   | Used for communication with nRF24L01, MCP23S17 and FM25L16B-GTR |
| 16 | PB0 / TIM3_CH3 / TIM8_CH2N / TIM1_CH2N / QUADSPI1_BK1_IO1 / UCPD1_FRSTX / EVENTOUT / ADC3_IN12 / ADC1_IN15 / COMP4_INP / OPAMP2_VINP / OPAMP3_VINP                        | 🟣 NRF24L01_CS | Chip select NRF24L01                                            |
| 17 | PB1 / TIM3_CH4 / TIM8_CH3N / TIM1_CH3N / COMP4_OUT / QUADSPI1_BK1_IO0 / EVENTOUT / ADC3_IN1 / ADC1_IN12 / COMP1_INP / OPAMP3_VOUT / OPAMP6_VINM                           | 🟣 FRAM_CS     | Chip select FM25L16B-GTR                                        |
| 18 | PB2 / RTC_OUT2 / LPTIM1_OUT / TIM5_CH1 / TIM20_CH1 / I2C3_SMBA / QUADSPI1_BK2_IO1 / EVENTOUT / ADC2_IN12 / COMP4_INM / OPAMP3_VINM                                        | 🟣 MCP_CS      | Chip select MCP23S17                                            |
| 19 | VSSA                     | GND        |  Filtered ground for built-in ADC |
| 20 | VREF+                    | +3.3V      |  Filtered and precise 3.3 V for built-in ADC |
| 21 | VDDA                     | +3.3V      |  Filtered and precise 3.3 V for built-in ADC |
| 22 | PB10 / TIM2_CH3 / USART3_TX / LPUART1_RX / QUADSPI1_CLK / TIM1_BKIN / SAI1_SCK_A / EVENTOUT / COMP5_INM / OPAMP3_VINM / OPAMP4_VINM                                       | 🔵 INFINEON_RST| Used to reset XMC4200 in case of fault            |
| 23 | VSS                      | GND      | Common ground            |
| 24 | VDD                      | +3.3V    | Supply voltage from low ripple linear voltage regulator            |
| 25 | PB11 / TIM2_CH4 / USART3_RX / LPUART1_TX / QUADSPI1_BK1_NCS / EVENTOUT / ADC12_IN14 / COMP6_INP / OPAMP4_VINP / OPAMP6_VOUT                                               | 🔵 RENESANS CS | Chip select RENESANS MCU  |
| 26 | PB12 / TIM5_ETR / I2C2_SMBA / SPI2_NSS/I2S2_WS / TIM1_BKIN / USART3_CK / LPUART1_RTS_DE / FDCAN2_RX / EVENTOUT / ADC4_IN3 / ADC1_IN11 / COMP7_INM / OPAMP4_VOUT / OPAMP6_VINP  | 🔵 INFINEON_CS | Chip select Infineon MCU |
| 27 | PB13 / SPI2_SCK/I2S2_CK / TIM1_CH1N / USART3_CTS / LPUART1_CTS / FDCAN2_TX / EVENTOUT / ADC3_IN5 / COMP5_INP / OPAMP3_VINP / OPAMP4_VINP / OPAMP6_VINP                    | 🔵 SPI2_SCK  | Communication witn MCU's |
| 28 | PB14 / TIM15_CH1 / SPI2_MISO / TIM1_CH2N / USART3_RTS_DE / COMP4_OUT / EVENTOUT / ADC4_IN4 / ADC1_IN5 / COMP7_INP / OPAMP2_VINP / OPAMP5_VINP                             | 🔵 SPI2_MISO | Communication witn MCU's |
| 29 | PB15 / RTC_REFIN / TIM15_CH2 / TIM15_CH1N / COMP3_OUT / TIM1_CH3N / SPI2_MOSI/I2S2_SD / EVENTOUT / ADC4_IN5 / ADC2_IN15 / COMP6_INM / OPAMP5_VINM                         | 🔵 SPI2_MOSI | Communication witn MCU's |
| 30 | PA8 / MCO / I2C3_SCL / I2C2_SDA / I2S2_MCK / TIM1_CH1 / USART1_CK / COMP7_OUT / TIM4_ETR / FDCAN3_RX / SAI1_CK2 / SAI1_SCK_A / EVENTOUT / ADC5_IN1 / OPAMP5_VOUT | 🟢 I2C2_SDA     | Communication with ADXL345 accelerometer |
| 31 | PA9 / I2C3_SMBA / I2C2_SCL / I2S3_MCK / TIM1_CH2 / USART1_TX / OMP5_OUT / TIM15_BKIN / TIM2_CH3 / SAI1_FS_A / EVENTOUT / ADC5_IN2 / UCPD1_DBCC1                  | 🟢 I2C2_SCL     | Communication with ADXL345 accelerometer |
| 32 | PA10 / TIM17_BKIN / USB_CRS_SYNC / I2C2_SMBA / SPI2_MISO / TIM1_CH3 / USART1_RX / COMP6_OUT / TIM2_CH4 / TIM8_BKIN / SAI1_D1 / SAI1_SD_A / EVENTOUT / UCPD1_DBCC2 / PVD_IN         | 🔵 NXP_CS      | Chip select NXP MCU |
| 33 | PA11 / SPI2_MOSI / I2S2_SD / TIM1_CH1N / USART1_CTS / COMP1_OUT / FDCAN1_RX / TIM4_CH1 / TIM1_CH4 / TIM1_BKIN2 / EVENTOUT / USB_DM                                                 | 🔵 FPGA_CS     | Chip select FPGA MCU |
| 34 | PA12 / TIM16_CH1 / I2SCKIN / TIM1_CH2N / USART1_RTS_DE / COMP2_OUT / FDCAN1_TX / TIM4_CH2 / TIM1_ETR / EVENTOUT / USB_DP                                                           | 🔵 NXP_RST     | Used to reset NXP MCU in case of fault |
| 35 | VSS                      | GND     | Common ground            |
| 36 | VDD                      | +3.3V   | Supply voltage from low ripple linear voltage regulator            |
| 37 | PA13 / SWDIO-JTMS / TIM16_CH1N / I2C4_SCL / I2C1_SCL / IR_OUT / USART3_CTS / TIM4_CH3 / SAI1_SD_B / EVENTOUT                    | 🔴 ST-Link      | JTAG data input (JTDI); used for programming/debugging via ST-Link |
| 38 | PA14 / SWCLK-JTCK / LPTIM1_OUT / I2C4_SMBA / I2C1_SDA / TIM8_CH2 / TIM1_BKIN / USART2_TX / SAI1_FS_B / EVENTOUT                 | 🔴 ST-Link      | Debug clock input for SWD interface; used for programming and debugging via ST-Link |
| 39 | PA15 / JTDI / TIM2_CH1 / TIM8_CH1 / I2C1_SCL / SPI1_NSS / SPI3_NSS / I2S3_WS / USART2_RX / UART4_RTS_DE / TIM1_BKIN / FDCAN3_TX / TIM2_ETR / EVENTOUT                              | 🔵 RENESANS_RST | Used to reset RENESANS MCU in case of fault |
| 40 | PB3 / JTDO-TRACESWO / TIM2_CH2 / TIM4_ETR / UCPD1_CRS_SYNC / TIM8_CH1N / SPI1_SCK / SPI3_SCK / I2S3_CK / USART2_TX / TIM3_ETR / FDCAN3_RX / SAI1_SCK_B / EVENTOUT                  | 🔵 FPGA_RST     | Used to reset FPGA MCU in case of fault     |
| 41 | PB4 / JTRST / TIM16_CH1 / TIM3_CH1 / TIM8_CH2N / SPI1_MISO / SPI3_MISO / USART2_RX / UART5_RTS_DE / TIM17_BKIN / FDCAN3_TX / SAI1_MCLK_B / EVENTOUT / UCPD1_CC2                    | ⚪ NC           | Not used                                    |
| 42 | PB5 / TIM16_BKIN / TIM3_CH2 / TIM8_CH3N / I2C1_SMBA / SPI1_MOSI / SPI3_MOSI / I2S3_SD / USART2_CK / I2C3_SDA / FDCAN2_RX / TIM17_CH1 / LPTIM1_IN1 / SAI1_SD_B / UART5_CTS / EVENTOUT | ⚪ NC         | Not used                                    |
| 43 | PB6 / TIM16_CH1N / TIM4_CH1 / TIM8_CH1 / TIM8_ETR / USART1_TX / COMP4_OUT / FDCAN2_TX / TIM8_BKIN2 / LPTIM1_ETR / SAI1_FS_B / EVENTOUT / UCPD1_CC1 | 🔴 USART1_TX      | Used to communicate with PC's serial port via Nucleo Board Programmer |
| 44 | PB7 / TIM17_CH1N / TIM4_CH2 / I2C4_SDA / I2C1_SDA / TIM8_BKIN / USART1_RX / COMP3_OUT / TIM3_CH4 / LPTIM1_IN2 / FMC_NL / UART4_CTS / EVENTOUT      | 🔴 USART1_RX      | Used to communicate with PC's serial port via Nucleo Board Programmer |
| 45 | PB8 / BOOT0 / TIM16_CH1 / TIM4_CH3 / SAI1_CK1 / I2C1_SCL / USART3_RX / COMP1_OUT / FDCAN1_RX / TIM8_CH2 / TIM1_BKIN / SAI1_MCLK_A / EVENTOUT       | ⚪ NC           | Not used                                    |
| 46 | PB9 / TIM17_CH1 / TIM4_CH4 / SAI1_D2 / I2C1_SDA / IR_OUT / USART3_TX / COMP2_OUT / FDCAN1_TX / TIM8_CH3 / TIM1_CH3N / SAI1_FS_A / EVENTOUT         | ⚪ NC           | Not used                                    |
| 47 | VSS                      | GND      | Common ground            |
| 48 | VDD                      | +3.3V    | Supply voltage from low ripple linear voltage regulator            |

#### 3.2 XMC4200F64K256BAXQSA1 Pinout (TQFP-64)

Due to the extensive configuration options of Infineon MCUs, I chose to use the official **XMC Pinout Tool** provided on Infineon's website.
Additionally due to the large number of pins, I decided to use colors to group them.  
Colors and hardware related to them are described below:
- 🟠 UART2 - Communication
- 🔵 U1C1 SPI, GPIO - Communication with STM32
- 🟢 U0C1 I²C - Communication with ADC converters on **HardwareControlBoard**
- 🔴 ST-Link, UART1 - Debug, SWD, Programming 
- ⚪ Unused
- 🟡 GPIO - used for controlling external hardware components such as DC motors and heater via CCU8 timers and their associated encoders, as well as for handling H-bridge direction control signals (A and B)
- 🟣 CCU4 timer inputs/outputs
- 🔷 HRPWM - PWM/Encoder signals for radar servo positioning with position feedback

| #  | Pin / Function | Usage/Label | Description |
|----|----------------|-------|-------------|
| 1  | P0.1 / **U1C1** / DOUT0 / CCU80 / OUT11 / LEDTS0 / COL3 / ERU0.0 / A0 / HRPWM / 0.C2INB                                        | 🔵 U1C1 SPI_MISO    | Communication with STM32 |
| 2  | P0.0 / CAN / N0_TXD / CCU80 / OUT21 / LEDTS0 / COL2 / **U1C1** / DX0D / ERU0.0 / B0 / USB.VB / USDETE / CT A / HRPWM / 0.C1INB | 🔵 U1C1 SPI_MOSI    | Communication with STM32 |
| 3  | P0.10 / **U1C1** / SCLKO / UT / CCU80 / OUT02 / LEDTS0 / COL1 / U1C1 / DX1A / ERU0.1 / A0                                      | 🔵 U1C1 SPI_SCK     | Communication with STM32 |
| 4  | P0.9 / HRPWM / 0.HROU / T31 / **U1C1** / SELO0 / CCU80 / OUT12 / LEDTS0 / COL0 / U1C1 / DX2A / ERU0.1 / B0                     | 🔵 U1C1 CS_INFINEON | Communication with STM32 |
| 5  | P3.0 / U0C1 / SCLKO / UT / U0C1.D / X1B / CCU80.I / N2C                                                                    |       |             |
| 6  | USB_DM                                                                                                                     |       |             |
| 7  | USB_DP                                                                                                                     |       |             |
| 8  | VDDP                | +3.3V    | Supply voltage from low ripple linear voltage regulator            |
| 9  | VDDC               |       |             |
| 10 | HIB_IO_0 / HIBOUT / WWDT / SERVICE_OUT / WAKEUPA / USB.VB / USDETE / CT C                                                  |                     |             |
| 11 | RTC_XTAL1 / ERU0.1 / B1                                                                                                    | ⚪ NC               | Not used                |
| 12 | RTC_XTAL2                                                                                                                  | ⚪ NC               | Not used                |
| 13 | VBAT                                                                                                                       | ⚪ NC               | Not used                |
| 14 | P14.14 / VADC.G1CH6 / G1ORC6                                                                                               |       |             |
| 15 | P14.7 / VADC.G0CH7 / POSIF0.IN0B                                                                                           |       |             |
| 16 | P14.6 / VADC.G0CH6 / POSIF0.IN1B / G0ORC6                                                                                  |       |             |
| 17 | P14.5 / VADC.G0CH5 / POSIF0.IN2B                                                                                           |       |             |
| 18 | P14.4 / VADC.G0CH4                                                                                                         |       |             |
| 19 | P14.3 / VADC.G0CH3 / VADC.G1CH3 / CAN.N0_RXDB                                                                              |       |             |
| 20 | P14.0 / VADC.G0CH0                                                                                                         |       |             |
| 21 | VSSA/VAGND                | GND       | Unfiltered common ground as internal ADC won't be used             |
| 22 | VDDA/VAREF                | +3.3V     | Unfiltered Supply voltage from low ripple linear voltage regulator as internal ADC won't be used |
| 23 | P14.9 / DAC.OUT_1 / VADC.G1CH1                                                                                             |       |             |
| 24 | P14.8 / DAC.OUT_0 / VADC.G1CH0                                                                                             |       |             |
| 25 | P2.15 / VADC.E.MUX12 / **CCU80.OUT11** / LEDTS0.LINE6 / LEDTS0.EXTENDED6 / LEDTS0.TSIN6A / U1C0.DX0C                       | 🟡 HEATER_PWM       | Used to generate PWM signal for the glycerol heater |
| 26 | P2.14 / VADC.E.MUX11 / U1C0.DOUT0 / CCU80.OUT21 / U1C0.DX0D                                                                |       |             |
| 27 | P2.9 / CCU80.OUT22 / LEDTS0.LINE5 / LEDTS0.EXTENDED5 / LEDTS0.TSIN5A / DAC.TRIGGER4 / CCU41.IN0B / CCU41.IN1B / CCU41.IN2B / CCU41.IN3B |       |             |
| 28 | P2.8 / CCU80.OUT32 / LEDTS0.LINE4 / LEDTS0.EXTENDED4 / LEDTS0.TSIN4A / DAC.TRIGGER5 / CCU40.IN0B / CCU40.IN1B / CCU40.IN2B / CCU40.IN3B |       |             |
| 29 | P2.5 / U0C1.DOUT0 / CCU41.OUT0 / LEDTS0.LINE3 / LEDTS0.EXTENDED3 / LEDTS0.TSIN3A / **U0C1.DX0B** / ERU0.0.A2 / CCU41.IN0A / HRPWM0.BL2A | 🟢 U0C1 I²C SDA     | Used to communicate with ADC converter on **HardwareControlBoard** |
| 30 | P2.4 / VADC.E.MUX02 / U0C1.SCLKO / U0C1.UT / CCU41.OUT1 / LEDTS0.LINE2 / LEDTS0.EXTENDED2 / LEDTS0.TSIN2A / **U0C1.DX1A** / ERU0.0.B2 / CCU41.IN1A / HRPWM0.BL1A | 🟢 U0C1 I²C SCL     | Used to communicate with ADC converter on **HardwareControlBoard** |
| 31 | P2.3 / VADC.E.MUX01 / U0C1.SELO0 / **CCU41.OUT2** / LEDTS0.LINE1 / LEDTS0.EXTENDED1 / LEDTS0.TSIN1A / U0C1.DX2A / ERU0.1.A2 / CCU41.IN2A| 🟣 SERVO_3_PWM | Used to control servo 3 |
| 32 | P2.2 / VADC.E.MUX00 / **CCU41.OUT3** / LEDTS0.LINE0 / LEDTS0.EXTENDED0 / LEDTS0.TSIN0A / U0C1.DX0A / ERU0.1.B2 / CCU41.IN3A             | 🟣 SERVO_4_PWM | Used to control servo 4 |
| 33 | P2.1 / LEDTS0.COL0 / DB.TDO / TRACES.WO / ERU1.0.B0 / CCU40.I.N0C                                                                       |       |             |
| 34 | P2.0 / CAN.N0_TXD / LEDTS0.COL1 / ERU0.0.B3 / CCU40.I.N1C                                                                               |       |             |
| 35 | P2.7 / CAN.N1_TXD / CCU80.OUT03 / LEDTS0.COL2 / ERU1.1.B0 / CCU40.I.N2C                                                                 |       |             |
| 36 | P2.6 / **CCU80.OUT13** / LEDTS0.COL3 / CAN.N1_RXDA / ERU0.1.B3 / CCU40.I.N3C                                                            | 🟡 GLYCEROL_PWM  | Used to generate PWM signal for the glycerol pump |
| 37 | VSS               | GND      | Unfiltered common ground as internal ADC won't be used            |
| 38 | VDDP               | +3.3V    | Supply voltage from low ripple linear voltage regulator            |
| 39 | XTAL1 / U0C0.D.X0F / U0C1.D.X0F / U1C0.D.X0F / U1C1.D.X0F                                                                               |       |             |
| 40 | XTAL2                |       |             |
| 41 | VSSO               | GND       | Unfiltered common ground as internal ADC won't be used             |
| 42 | VDDC               |       |             |
| 43 | PORST               | MCU's reset                            | Used to reset the MCU either by button or programmer |
| 44 | TMS / DB.TMS / SWDIO                |       |             |
| 45 | TCK / DB.TCK / SWCLK                |       |             |
| 46 | P1.15 / SCU.EXTCLK / U1C0.D.OUT0 / ERU1.1.A0                                                                                            | 🟡 DC_MOTOR_1A       | Used to select forward direction of DC motor 1|
| 47 | P1.5 / CAN.N1_TXD / U0C0.DOUT0 / CCU80.OUT23 / U0C0.DOUT0 / U0C0.HWIN0 / U0C0.DX0A / CAN.N0_RXDA / ERU0.2.A0 / ERU1.0.A0 / CCU41.I.N1C  | 🟡 DC_MOTOR_2B       | Used to select reverse direction of DC motor 2|
| 48 | P1.4 / WWDT.SERVICE_OUT / CAN.N0_TXD / CCU80.OUT33 / U0C0.DOUT1 / U0C0.HWIN1 / U0C0.DX0B / CAN.N1_RXDD / ERU0.2.B0 / CCU41.I.N0C / HRPWM0.BL0A| 🟡 DC_MOTOR_2A | Used to select forward direction of DC motor 2|
| 49 | P1.3 / U0C0.MCLKO / UT / CCU40.OUT0 / ERU1.PDOUT0 / U0C0.DOUT2 / U0C0.HWIN2 / POSIF0.IN0A / ERU1.2.A0 / CCU40.I.N0A / HRPWM0.C0INB      | 🟡 DC_MOTOR_1B       | Used to select reverse direction of DC motor 1|
| 50 | P1.2 / CCU40.OUT1 / ERU1.PDOUT1 / U0C0.DOUT3 / U0C0.HWIN3 / POSIF0.IN1A / ERU1.2.B0 / CCU40.I.N1A / **HRPWM0.C2INA**                    | 🔷 HRPWM RADAR_SERVO_ENC       | Encoder for radar's servomechanism |
| 51 | P1.1 / U0C0.SCLKO / UT / **CCU40.OUT2** / ERU1.PDOUT2 / U0C0.DX1A / POSIF0.IN2A / ERU0.3.A0 / CCU40.I.N2A / HRPWM0.C1INA                | 🟣 SERVO_1_PWM | Used to control servo 1 |
| 52 | P1.0 / U0C0.SELO0 / **CCU40.OUT3** / ERU1.PDOUT3 / U0C0.DX2A / ERU0.3.B0 / CCU40.I.N3A / HRPWM0.C0INA                                   | 🟣 SERVO_2_PWM | Used to control servo 2 |
| 53 | P1.9 / U0C0.SCLKO / U1C1.DOUT0               |       |             |
| 54 | P1.8 / U0C0.SELO1 / U1C1.SCLKO               |       |             |
| 55 | P1.7 / U0C0.DOUT0 / U1C1.SELO2 / USB.VB / USDETE / CT_B            |       |             |
| 56 | VDDP                | +3.3V    | Supply voltage from low ripple linear voltage regulator            |
| 57 | P0.8 / SCU.EXTCLK / U0C0.SCLKO / UT / HRPWM0.HROUT10 / DB.TRST / U0C0.DX1B / ERU0.2.A1 / CCU80.IN1B               |       |             |
| 58 | P0.7 / WWDT.SERVICE_OUT / U0C0.SELO0 / HRPWM0.HROUT11 / DB.TDI / U0C0.DX2B / ERU0.2.B1 / **CCU80.IN0A** / CCU80.IN1A / CCU80.IN2A / CCU80.IN3A | 🟡 DC_MOTOR_1_ENC | Used to count encoder pulses from motor 1 |
| 59 | P0.11 / U1C0.SCLKO / UT / CCU80.OUT31 / U1C0.DX1A / ERU0.3.A2               |       |             |
| 60 | P0.6 / U1C0.SELO0 / CCU80.OUT30 / HRPWM0.HROU.T30 / U1C0.DX2A / ERU0.3.B2 / **CCU80.I.N2B**           |🟡 DC_MOTOR_2_ENC | Used to count encoder pulses from motor 2 |
| 61 | P0.5 / U1C0.DOUT0 / CCU80.OUT00 / HRPWM0.HROUT00 / U1C0.HWIN0 / U1C0.DX0B / ERU1.3.A0                 |       |             |
| 62 | P0.4 / CCU80.OUT10 / **HRPWM0.HROUT21** / U1C0.DOUT1 / U1C0.HWIN1 / U1C0.DX0A / ERU0.2.B3             |🔷 HRPWM RADAR_SERVO_PWM      | High precision PWM signal for radar's servomechanism
| 63 | P0.3 / **CCU80.OUT20** / HRPWM0.HROUT20 / U1C0.DOUT2 / U1C0.HWIN2 / ERU1.3.B0                         |🟡 DC_MOTOR_2_PWM        | Used to generate PWM signal for motor's 2 H-Bridge |
| 64 | P0.2 / U1C1.SELO1 / **CCU80.OUT01** / HRPWM0.HROUT01 / U1C0.DOUT3 / U1C0.HWIN3 / ERU0.3.B3            |🟡 DC_MOTOR_1_PWM        | Used to generate PWM signal for motor's 1 H-Bridge |

---

### 4. Control board logic diagram

![Control board logic diagram](/MainControlBoard/Media/PCB_Architecture.png)

---

### 5. STM32G473CET6

#### 5.1 Clock configuration

![STM32G473 Clock Configuration](/MainControlBoard/Media/STM32G473CET6_ClockConfiguration.png)

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
   

### 6. Infineon

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

#### 7.3 L76L-M33 GPS

#### 7.4 STWD100NYWY3F external watchdog timer

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

#### 7.5 FM25L16B-GTR FRAM

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

#### 7.6 ADXL-345 Accelerometer

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

#### 7.7 Buzzer

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

#### 7.8 STM32G473RET6

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

#### 7.9 XMC4200F64K256BAXQSA1

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

#### 7.10 Programming and Debug Interfaces

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

#### 7.11 Hardware Control Connector

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

#### 7.12 PCB Stack SPI Interface

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

**General notes:**
- Several components are intentionally left as DNP to allow flexibility during bring-up
- Final values may be adjusted after testing and signal validation

#### 8.3 BOM (Bill of Materials)

| Designator | Value | Tolerance | Footprint | Quantity | Mouser No | Notes |
|------------|-------|-----------|------------|----------|------------|--------|
| SJ1–SJ28 | 0Ω (configurable) | 5% | R0603 | 28 | - | Jumpers on bottom layer for external signals; allows optional series resistors for tuning or direct short (0Ω) |
| **U1** | STM32G473RET6 | - | LQFP-64_L10.0-W10.0-P0.50-LS12.0-BL | 1 | 511-STM32G473RET6 | Main MCU |
| C5, C6, C7, C23, C29 | 100nF | 5% | C0603 | 5 | - | Decoupling capacitors placed close to STM32G473RET6 VDD pins |
| C8 | 4.7µF | 5% | C0603 | 1 | - | Bulk capacitor for STM32G473RET6 VDD supply |
| C11, C22 | 1µF | 5% | C0603 | 2 | - | Bulk capacitors for STM32G473RET6 analog supply (VREF+, VDDA) |
| C21 | 10nF | 5% | C0603 | 1 | - | Filtering capacitor for STM32G473RET6 VDDA |
| | | | | | | |
| **U2** | L76L-M33 | - | COMM-SMD_18P-L10.1-W9.7-P1.1-TL_L76-LB | 1 | 277-L76L-M33 | Onboard GNSS (GPS) module |
| | | | | | | |
| **U3** | XMC4200F64K256BAXQSA1 | - | LQFP-64_L10.0-W10.0-P0.50-LS12.0-TL-EP5.7 | 1 | 726-XMC4200F64K256BA | Motor control MCU (PWM/CCU, encoder interfaces, SPI slave) |
| | | | | | | |
| | | | | | | |
| Q1 | IRF7380TRPBF-VB | - | SOP-8_L4.9-W3.9-P1.27-LS6.0-BL | 1 | 726-IRF7380TRPBFXTMA | N-channel MOSFET for MQ sensor heater control |
| | | | | | | |
| **U6** | TLV76133DCYR | - | SOT-223-4_L6.5-W3.5-P2.30-LS7.0-TR | 1 | 595-TLV76133DCYR | 3.3V LDO regulator (main PCB power supply) |
| | | | | | | |
| **U7** | nRF24L01P-R | - | QFN-20_L4.0-W4.0-P0.50-BL-EP | 1 | 949-NRF24L01P-T | 2.4 GHz RF transceiver module |
| L1 | 8.2nH | 1% | L0402 | 1 | - | RF matching network (nRF24L01+) |
| L2 | 2.7nH | 1% | L0402 | 1 | - | RF matching network (nRF24L01+) |
| L3 | 3.9nH | 1% | L0402 | 1 | - | RF matching network (nRF24L01+) |
| | | | | | | |
| **U8** | FM25L16B-GTR | - | SOIC-8_L4.9-W3.9-P1.27-LS6.0-BL | 1 | 877-FM25L16B-GTR | SPI FRAM memory for data logging (telemetry, events, GPS position) |
| C4 | 100nF | 5% | C0603 | 1 | - | FRAM decoupling capacitor |
| R2 | 10k | 5% | R0603 | 1 | - | FRAM CS pull-up resistor |
| R15 | 22Ω | 5% | R0603 | 1 | - | FRAM SPI1 MOSI series resistor |
| | | | | | | |
| | | | | | | |
