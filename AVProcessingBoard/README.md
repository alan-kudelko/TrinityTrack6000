# **AVProcessingBoard** – **Lattice LIFCL-17-7SG72C (FPGA)**

**LIFCL-17-7SG72C**: Dedicated FPGA for real-time video and audio processing, handling camera and microphone data, and transmitting processed streams to the receiver.

The system is implemented on a custom PCB and functions as a slave module within the overall architecture, controlled by the **MainControlBoard**.

> ⚠️ Status: Requirements definition & early exploration phase  
> 🔧 Goal: Serve as an educational platform for learning FPGA-based systems, including real-time video and audio processing, as well as wireless streaming of multimedia data

---

## ✳️ Planned Technologies & Tools

- **Processing Unit**: Lattice FPGA (LIFCL-17-7SG72C)
  - real-time video and audio processing
  - streaming datapath implementation
- **Interfaces**:
  - Camera interface (DVP / parallel) *(TBD)*
  - Audio input (PDM / I2S) *(TBD)*  
  - SPI (control interface with MainControlBoard)
  - TX / RF interface *(TBD)*
- **Memory**:  
  - External Flash / EEPROM *(TBD)*  
    - configuration storage  
    - image / asset storage  
- **Development Tools**:  
  - **Lattice Radiant** (primary FPGA design environment)  
  - **Lattice Diamond** *(optional / legacy)*  
  - **Verilog / SystemVerilog** (HDL design)  

## Development Strategy

- **Prototyping Phase:**  
  Early validation of interfaces and data pipeline using development boards and simple test setups.  
  Focus on:
  - camera interface bring-up  
  - basic video pipeline (capture → processing → output)  
  - clocking and timing validation  

- **Production Phase:**  
  Final implementation on a custom PCB featuring:
  - Lattice FPGA as a dedicated video/audio processing unit  
  - integration with camera, memory and RF interface  
  - SPI-based control from MainControlBoard  

## Key Features
- FPGA-based real-time video and audio processing  
- Streaming pipeline architecture (full framebuffer)  
- Deterministic and low-latency data path  
- Modular integration with MainControlBoard (SPI control)  
- Scalable design for higher resolutions and future extensions  

### 🔌 Hardware & Electronics
- Custom PCB hosting:
  - **Lattice FPGA (LIFCL-17-7SG72C)**  
    - real-time video/audio processing  
  - Camera module *(TBD)*  
  - Microphone interface *(TBD)*  
  - External Flash / EEPROM *(TBD)*  
  - RF / transmission interface *(TBD)*  
  - Power regulation circuitry (local FPGA supply)  

### 🧠 System Architecture & Concepts
- Fully hardware-based **streaming datapath**  
- Line-buffer-based processing (no full framebuffer)  
- Separation of **control plane (SPI)** and **data plane (FPGA)**  
- Clock domain separation (camera / system / TX)  
- Deterministic processing latency  

### 🧰 Development Tools & Libraries
- **Verilog / SystemVerilog** for FPGA development  
- **Lattice Radiant / Diamond** toolchain  
- Simulation tools *(TBD – e.g. ModelSim / Questa / open-source)*  
- Basic testbenches for pipeline validation  

---

## 🧠 Design Goals

- 🎯 Design a **dedicated FPGA-based module** for real-time video and audio processing and streaming  

- 📹 Implement camera input support:
  - Resolution: **480×320 @ 30–60 fps** *(target)*
  - Optional: **640×480 @ 30 fps (max)**
  - Interface: **DVP / parallel (8–12 bit)**
  - Signals: HSYNC / VSYNC / PCLK

- 📡 Develop a video transmission path:
  - Throughput: **~40–80 Mbps**
  - Interface: **TBD (parallel / simple serial)**
  - Custom RF-oriented transmission protocol

- 🧠 Build a fully hardware-based processing pipeline:
  - Streaming architecture (full framebuffer)
  - Deterministic latency
  - Line-buffer-based processing

- 💾 Integrate external memory:
  - **QSPI Flash / EEPROM (TBD)**
  - configuration storage
  - image / asset storage
  - potential buffering

- 🔊 Support audio acquisition:
  - **PDM or I2S input**

- 📶 Design a transmission interface:
  - High-speed data path *(TBD)*
  - Optimized for RF communication

- 🧠 Select appropriate FPGA resources:
  - LUT: ~10k–15k
  - BRAM: ~300–600 kB
  - DSP: optional
  - sufficient PLL / clocking resources
  - IO count: ~30–40

- ⏱️ Establish system timing constraints:
  - Pixel clock: ~20–40 MHz
  - System clock: ~40–80 MHz
  - TX clock: application-dependent

- 🧪 Serve as an educational platform for:  
  - FPGA design and HDL development
  - real-time video/audio processing
  - streaming architectures
  - clock domain crossing and timing closure
  - hardware-software system integration

- 🧩 Maintain modular system integration:
  - seamless cooperation with **MainControlBoard**
  - SPI-based control architecture
  - scalable for future extensions

---

## 🗺️ Development Progress

🔄 The design is currently in the **requirements definition and early exploration phase**.

At this stage:
- System requirements are being defined
- FPGA ecosystem is being explored (tools, architecture, workflow)
- Initial FPGA family selection is in progress
- High-level system architecture is being established

Further work will include:
- Selection of development board for prototyping
- Interface definition (camera / audio / RF)
- Detailed FPGA resource estimation
- Initial pipeline architecture design
- Timing constraints definition and validation
- PCB design and signal integrity analysis
