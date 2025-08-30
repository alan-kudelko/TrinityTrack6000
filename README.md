TrinityTrack6000 is a custom-built, multi-MCU remote-controlled tank constructed from 2 mm and 3 mm steel sheets. The system runs on a custom PCB integrating STM32, Infineon, and AVR microcontrollers, designed from scratch with fully deterministic memory allocation and task management.

The project leverages **ThreadX RTOS** on the STM32 for high-level coordination and **Micrium µC/OS RTOS** on the Infineon MCU for precise motor and servo control. The AVR MCU functions as an I²C slave for reading data from 1-Wire temperature sensors, allowing time-critical acquisition without blocking main task execution.

All tasks on STM32 and Infineon MCUs are allocated with **guard zones** in RAM, using custom **linker scripts** to arrange task stacks and guard areas contiguously. This setup enables continuous memory monitoring and deterministic stack overflow detection, fully compatible with static memory allocation principles.

The project is developed according to **MISRA C:2025** standards, ensuring safe, maintainable, and portable code. The system includes robust diagnostics and fault-handling mechanisms, including monitoring of RAM and CPU usage, task stack overflows, and EEPROM-based error logging.

Powering the tank is a **custom-designed switching power supply**, built from scratch to provide stable and reliable voltage for all MCUs and high-current peripherals.

> 🔧 Status: Planned after finishing DrinkCreator6000  
> 🧪 Goal: Serve as a futuristic, educational platform for learning **STM32**, **Infineon MCUs**, and real-time operating systems **ThreadX** and **Micrium µC/OS**, while exploring multi-MCU coordination and embedded system design  

---

## ✳️ Planned Technologies & Tools

This project integrates a wide range of hardware, software, and system-level concepts typically found in professional embedded systems. Below is a breakdown of the core technologies and components involved in the design and implementation of TrinityTrack6000:

### 🔌 Hardware & Electronics
- Custom PCB hosting all MCUs:
  - **STM32 MCU** – Handles high-level coordination tasks and system management
  - **Infineon MCU** – Controls precise motor and servo operations with real-time current monitoring
  - **ATmega AVR MCU** as I²C Slave – Reads data from 1-Wire temperature sensors
- **nRF24l01** module
- **ADXL345** module
- Separate custom switching power supply PCB – Provides stable voltage rails for all MCUs and high-current peripherals

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
- I²C bus for AVR slave communication and peripheral integration
- Multi-level diagnostic LCD menu (planned) for viewing system status and sensor data
- Wireless interface: nRF24L01 for remote control and telemetry

### 🧰 Development Tools & Libraries
- **C / C++** for firmware development
- Native MCU libraries with low-level register access
- **ThreadX** for STM32, **Micrium µC/OS** for Infineon
- **VS Code, CMake, Ninja** for building and managing the project
- **Doxygen** for automatic code documentation

### 📚 Documentation
The source code is fully documented using **Doxygen**, which generates up-to-date, browsable documentation from the annotated source files. The generated docs can be found in the `docs/` directory and are updated as part of the build process.

---

## 🧠 Design Goals

- 💾 Use 100% static memory allocation (no malloc, no heap)
- 🧰 Track system stability via runtime task/memory debug tools, including guard zones and stack overflow detection
- 🔁 Ensure fault safety: robust recovery after failure or communication loss using EEPROM fault logging
- 📟 Provide full system visibility through LCD diagnostics and monitoring
- 🧪 Serve as a practical educational platform for learning **STM32** and **Infineon MCUs**, as well as real-time operating systems **ThreadX** and **Micrium µC/OS**
- 📚 Deepen understanding of multitasking, resource sharing, fail-safe system design, and embedded system best practices
- 🧩 Implement low-level memory management techniques for optimized and reliable resource control
- 🧱 Integrate tightly with custom hardware (PCB, sensors, actuators, display)
- 🌐 Explore principles of distributed embedded systems and multi-MCU communication commonly used in commercial applications

---

## 🗺️ Roadmap

---

## Technical Table of Contents

---

## ⚙️ Technical Overview

---

## STM32 Pinout

| Pin / Function | Usage |
|----------------|-------|
| PA0  ADC1_IN0 / TIM2_CH1 / USART2_RX / I2C1_SDA | ⚪ Unused |
| PA1  ADC1_IN1 / TIM2_CH2 / USART2_TX / I2C1_SCL | ⚪ Unused |
| PA2  ADC1_IN2 / TIM2_CH3 / USART2_CK / I2C1_SDA | ⚪ Unused |
| PA3  ADC1_IN3 / TIM2_CH4 / USART2_RTS / I2C1_SCL | ⚪ Unused |
| PA4  ADC1_IN4 / TIM3_CH1 / USART2_CTS / I2C1_SDA | ⚪ Unused |
| PA5  ADC1_IN5 / TIM3_CH2 / SPI1_SCK / I2C1_SCL | ⚪ Unused |
| PA6  ADC1_IN6 / TIM3_CH3 / SPI1_MISO / I2C1_SDA | ⚪ Unused |
| PA7  ADC1_IN7 / TIM3_CH4 / SPI1_MOSI / I2C1_SCL | ⚪ Unused |
| PA8  MCO / USART1_CK / I2C1_SDA | ⚪ Unused |
| PA9  USART1_TX / I2C1_SCL | ⚪ Unused |
| PA10 USART1_RX / I2C1_SDA | ⚪ Unused |
| PA11 USART1_RTS / I2C1_SCL | ⚪ Unused |
| PA12 USART1_CTS / I2C1_SDA | ⚪ Unused |
| PA13 SWDIO / JTAG_TDI | ⚪ Unused |
| PA14 SWCLK / JTAG_TMS | ⚪ Unused |
| PA15 JTDI / SPI1_NSS | ⚪ Unused |
| PB0  ADC2_IN8 / TIM3_CH1 / SPI2_NSS | ⚪ Unused |
| PB1  ADC2_IN9 / TIM3_CH2 / SPI2_SCK | ⚪ Unused |
| PB2  ADC2_IN10 / TIM3_CH3 / SPI2_MISO | ⚪ Unused |
| PB3  ADC2_IN11 / TIM3_CH4 / SPI2_MOSI | ⚪ Unused |
| PB4  ADC2_IN12 / TIM4_CH1 / SPI2_NSS | ⚪ Unused |
| PB5  ADC2_IN13 / TIM4_CH2 / SPI2_SCK | ⚪ Unused |
| PB6  ADC2_IN14 / TIM4_CH3 / SPI2_MISO | ⚪ Unused |
| PB7  ADC2_IN15 / TIM4_CH4 / SPI2_MOSI | ⚪ Unused |
| PB8  I2C2_SCL / USART3_TX | ⚪ Unused |
| PB9  I2C2_SDA / USART3_RX | ⚪ Unused |
| PB10 I2C2_SCL / USART3_CK | ⚪ Unused |
| PB11 I2C2_SDA / USART3_RTS | ⚪ Unused |
| PB12 I2C2_SCL / USART3_CTS | ⚪ Unused |
| PB13 SWDIO / JTAG_TDI | ⚪ Unused |
| PB14 SWCLK / JTAG_TMS | ⚪ Unused |
| PB15 JTDI / SPI1_NSS | ⚪ Unused |
| PC0  ADC3_IN10 / TIM8_CH1 / SPI3_NSS | ⚪ Unused |
| PC1  ADC3_IN11 / TIM8_CH2 / SPI3_SCK | ⚪ Unused |
| PC2  ADC3_IN12 / TIM8_CH3 / SPI3_MISO | ⚪ Unused |
| PC3  ADC3_IN13 / TIM8_CH4 / SPI3_MOSI | ⚪ Unused |
| PC4  ADC3_IN14 / TIM8_BKIN / SPI3_NSS | ⚪ Unused |
| PC5  ADC3_IN15 / TIM8_CH1N / SPI3_SCK | ⚪ Unused |
| PC6  ADC3_IN16 / TIM8_CH2N / SPI3_MISO | ⚪ Unused |
| PC7  ADC3_IN17 / TIM8_CH3N / SPI3_MOSI | ⚪ Unused |
| PC8  ADC3_IN18 / TIM8_CH4N / SPI3_NSS | ⚪ Unused |
| PC9  ADC3_IN19 / TIM8_BKIN2 / SPI3_SCK | ⚪ Unused |
| PC10 ADC3_IN20 / TIM8_CH1 / SPI3_MISO | ⚪ Unused |
| PC11 ADC3_IN21 / TIM8_CH2 / SPI3_MOSI | ⚪ Unused |
| PC12 ADC3_IN22 / TIM8_CH3 / SPI3_NSS | ⚪ Unused |
| PC13 ADC3_IN23 / TIM8_CH4 / SPI3_SCK | ⚪ Unused |
| PC14 OSC32_IN / LSE_IN | ⚪ Unused |
| PC15 OSC32_OUT / LSE_OUT | ⚪ Unused |
| PD0  ADC4_IN0 / TIM4_CH1 / USART2_RX | ⚪ Unused |
| PD1  ADC4_IN1 / TIM4_CH2 / USART2_TX | ⚪ Unused |
| PD2  ADC4_IN2 / TIM4_CH3 / USART2_CK | ⚪ Unused |
| PD3  ADC4_IN3 / TIM4_CH4 / USART2_RTS | ⚪ Unused |
| PD4  ADC4_IN4 / TIM5_CH1 / USART2_CTS | ⚪ Unused |
| PD5  ADC4_IN5 / TIM5_CH2 / USART2_RX | ⚪ Unused |
| PD6  ADC4_IN6 / TIM5_CH3 / USART2_TX | ⚪ Unused |
| PD7  ADC4_IN7 / TIM5_CH4 / USART2_CK | ⚪ Unused |
| PD8  ADC4_IN8 / TIM6_CH1 / USART2_RTS | ⚪ Unused |
| PD9  ADC4_IN9 / TIM6_CH2 / USART2_CTS | ⚪ Unused |
| PD10 ADC4_IN10 / TIM6_CH3 / USART2_RX | ⚪ Unused |
| PD11 ADC4_IN11 / TIM6_CH4 / USART2_TX | ⚪ Unused |
| PD12 ADC4_IN12 / TIM7_CH1 / USART2_CK | ⚪ Unused |
| PD13 ADC4_IN13 / TIM7_CH2 / USART2_RTS | ⚪ Unused |
| PD14 ADC4_IN14 / TIM7_CH3 / USART2_CTS | ⚪ Unused |
| PD15 ADC4_IN15 / TIM7_CH4 / USART2_RX | ⚪ Unused |
| PE0  ADC5_IN0 / TIM8_CH1 / SPI1_NSS | ⚪ Unused |
| PE1  ADC5_IN1 / TIM8_CH2 / SPI1_SCK | ⚪ Unused |
| PE2  ADC5_IN2 / TIM8_CH3 / SPI1_MISO | ⚪ Unused |
| PE3  ADC5_IN3 / TIM8_CH4 / SPI1_MOSI | ⚪ Unused |
| PE4  ADC5_IN4 / TIM8_BKIN / SPI1_NSS | ⚪ Unused |
| PE5  ADC5_IN5 / TIM8_CH1N / SPI1_SCK | ⚪ Unused |
| PE6  ADC5_IN6 / TIM8_CH2N / SPI1_MISO | ⚪ Unused |
| PE7  ADC5_IN7 / TIM8_CH3N / SPI1_MOSI | ⚪ Unused |
| PE8  ADC5_IN8 / TIM8_CH4N / SPI1_NSS | ⚪ Unused |
| PE9  ADC5_IN9 / TIM8_BKIN2 / SPI1_SCK | ⚪ Unused |
| PE10 ADC5_IN10 / TIM8_CH1 / SPI1_MISO | ⚪ Unused |
| PE11 ADC5_IN11 / TIM8_CH2 / SPI1_MOSI | ⚪ Unused |
| PE12 ADC5_IN12 / TIM8_CH3 / SPI1_NSS | ⚪ Unused |
| PE13 ADC5_IN13 / TIM8_CH4 / SPI1_SCK | ⚪ Unused |
| PE14 ADC5_IN14 / TIM8_BKIN / SPI1_MISO | ⚪ Unused |
| PE15 ADC5_IN15 / TIM8_CH1N / SPI1_MOSI | ⚪ Unused |
| PF0  ADC6_IN0 / TIM9_CH1 / I2C1_SDA | ⚪ Unused |
| PF1  ADC6_IN1 / TIM9_CH2 / I2C1_SCL | ⚪ Unused |
| PF2  ADC6_IN2 / TIM10_CH1 / I2C2_SDA | ⚪ Unused |
| PF3  ADC6_IN3 / TIM10_CH2 / I2C2_SCL | ⚪ Unused |
| PF4  ADC6_IN4 / TIM11_CH1 / I2C3_SDA | ⚪ Unused |
| PF5  ADC6_IN5 / TIM11_CH2 / I2C3_SCL | ⚪ Unused |
| PF6  ADC6_IN6 / TIM12_CH1 / USART1_RX | ⚪ Unused |
| PF7  ADC6_IN7 / TIM12_CH2 / USART1_TX | ⚪ Unused |
| PF8  ADC6_IN8 / TIM13_CH1 / USART1_CK | ⚪ Unused |
| PF9  ADC6_IN9 / TIM13_CH2 / USART1_RTS | ⚪ Unused |
| PF10 ADC6_IN10 / TIM14_CH1 / USART1_CTS | ⚪ Unused |
| PF11 ADC6_IN11 / TIM14_CH2 / USART1_RX | ⚪ Unused |
| PF12 ADC6_IN12 / TIM15_CH1 / USART1_TX | ⚪ Unused |
| PF13 ADC6_IN13 / TIM15_CH2 / USART1_CK | ⚪ Unused |
| PF14 ADC6_IN14 / TIM16_CH1 / USART1_RTS | ⚪ Unused |
| PF15 ADC6_IN15 / TIM16_CH2 / USART1_CTS | ⚪ Unused |
| PG0  ADC7_IN0 / TIM17_CH1 / I2C1_SDA | ⚪ Unused |
| PG1  ADC7_IN1 / TIM17_CH2 / I2C1_SCL | ⚪ Unused |
| PG2  ADC7_IN2 / TIM18_CH1 / I2C2_SDA | ⚪ Unused |
| PG3  ADC7_IN3 / TIM18_CH2 / I2C2_SCL | ⚪ Unused |
| PG4  ADC7_IN4 / TIM19_CH1 / I2C3_SDA | ⚪ Unused |
| PG5  ADC7_IN5 / TIM19_CH2 / I2C3_SCL | ⚪ Unused |

## Infineon Pinout

| Pin | Usage |
|-----|-------|
| PE0 (RXD0/PCINT8)  | 🟢 USART0 RX / ICSP Serial Data in |
| PE1 (TXD0)         | 🟢 USART0 TX / ICSP Serial Data out |
| PE3 (OC3A/AIN1)    | 🟢 Radiator fan 1 MOSFET's gate |
| PE4 (OC3B/INT4)    | 🟢 Radiator fan 2 MOSFET's gate |
| PE5 (OC3C/INT5)    | 🟢 Cooler fan MOSFET's gate |
| PE6 (T3/INT6)      | 🟢 Circulation pump MOSFET's gate |
| PB1 (SCK/PCINT1)   | 🟢 ICSP Serial Clock |
| PB5 (OC1A/PCINT5)  | 🟢 Buzzer NPN's base |
| PB6 (OC1B/PCINT6)  | 🔴 Open Drain Slave Data Ready |
| PC0 (A8)           | 🟢 74HC595 Serial data input |
| PC1 (A9)           | 🟢 74HC595 Storage register clock input |
| PC2 (A10)          | 🟢 74HC595 Shift register clock input |
| PC3 (A11)          | 🟢 74HC595 Output enable (active LOW) |
| PC4 (A12)          | 🟢 LED Ring DI |
| PC5 (A13)          | 🟢 LED Ring DO |
| PD0 (SCL/INT0)     | 🔵 I2C SCL |
| PD1 (SDA/INT1)     | 🔵 I2C SDA |	
| PD2 (RXD1/INT2)    | 🟢 MCP23008 INT |
| PD3 (TXD1/INT3)    | 🟢 MCP23008 RESET |
| PD4 (ICP1)         | 🟢 Peltier element 1 MOSFET's gate |
| PD5 (XCK1)         | 🟢 Peltier element 2 MOSFET's gate |
| PD6 (T1)           | 🟢 LED Ring DI |
| PD7 (T0)           | 🟢 LED Ring DO |

## ATmega328p Pinout

| Pin | Usage |
|-----|-------|
| PC6 (RESET/PCINT14)     | ⚪ Unused |
| PD0 (RXD/PCINT16)       | ⚪ Unused |
| PD1 (TXD/PCINT17)       | ⚪ Unused |
| PD2 (INT0/PCINT18)      | ⚪ Unused |
| PD3 (INT1/OC2B/PCINT19) | ⚪ Unused |
| PD4 (T0/XCK/PCINT20)    | ⚪ Unused |
| VCC                     | ⚪ Power |
| GND                     | ⚪ Ground |
| PB6 (XTAL1/TOSC1/PCINT6)| ⚪ Crystal |
| PB7 (XTAL2/TOSC2/PCINT7)| ⚪ Crystal |
| PD5 (T1/OC0B/PCINT21)   | ⚪ Unused |
| PD6 (AIN0/OC0A/PCINT22) | ⚪ Unused |
| PD7 (AIN1/PCINT23)      | ⚪ Unused |
| PB0 (ICP1/CLKO/PCINT0)  | ⚪ Unused |
| PB1 (OC1A/PCINT1)       | ⚪ Unused |
| PB2 (SS/OC1B/PCINT2)    | ⚪ Unused |
| PB3 (MOSI/OC2A/PCINT3)  | ⚪ Unused |
| PB4 (MISO/PCINT4)       | ⚪ Unused |
| PB5 (SCK/PCINT5)        | ⚪ Unused |
| AVCC                    | ⚪ Power |
| ADC6                    | ⚪ Unused |
| AREF                    | ⚪ Reference |
| GND                     | ⚪ Ground |
| PC0 (ADC0/PCINT8)       | ⚪ Unused |
| PC1 (ADC1/PCINT9)       | ⚪ Unused |
| PC2 (ADC2/PCINT10)      | ⚪ Unused |
| PC3 (ADC3/PCINT11)      | ⚪ Unused |
| PC4 (ADC4/SDA/PCINT12)  | ⚪ Unused |
| PC5 (ADC5/SCL/PCINT13)  | ⚪ Unused |
| ADC7                    | ⚪ Unused |

