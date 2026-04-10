# **AVProcessingBoard** – **Lattice LIFCL-17-7SG72C (FPGA)**

**LIFCL-17-7SG72C**: Dedicated FPGA for real-time video and audio processing, handling camera and microphone data, and transmitting processed streams to the receiver.

The system is implemented on a custom PCB and functions as a slave module within the overall architecture, controlled by the **MainControlBoard**.

> ⚠️ Status: Requirements definition & early exploration phase  
> 🔧 Goal: Serve as an educational platform for learning FPGA-based systems, including real-time video and audio processing, as well as wireless streaming of multimedia data

---

## ✳️ Planned Technologies & Tools

- **Processing Unit**: Lattice FPGA (e.g. LIFCL-17-7SG72C)  
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
- Streaming pipeline architecture (no full framebuffer)  
- Deterministic and low-latency data path  
- Modular integration with MainControlBoard (SPI control)  
- Scalable design for higher resolutions and future extensions  

### 🔌 Hardware & Electronics

- Custom PCB hosting:
  - **Lattice FPGA (LIFCL-17-7SG72C or similar)**  
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







## ⚙️ System Requirements

### 📹 Video Input
- Resolution: **800×600 @ 60 fps** *(target, scalable)*  
- Interface: **DVP / parallel (8–12 bit)**  
- Signals: HSYNC / VSYNC / PCLK  

---

### 📡 Video Output
- Throughput: **~300–500 Mbps**  
- Interface: **TBD (LVDS / parallel / serial)**  
- Custom transmission protocol (RF-oriented)

---

### 🧠 Processing Model
- Fully **streaming pipeline**  
- No full framebuffer (initial design)  
- Deterministic latency  

---

### 🔌 MCU Interface
- **SPI slave** (control plane)  
- **RDY (open-drain)** signal for synchronization  

---

### 💾 Memory
- External **QSPI Flash**  
  - configuration  
  - data storage  
  - bitstream / assets  

---

### 🔊 Audio
- **PDM or I2S input**  

---

### 📶 RF Interface
- **High-speed interface (TBD)**  
- Critical subsystem for data transmission  

---

### 🔧 GPIO Requirements
- Estimated: **30–40 GPIOs**  
- Used for:
  - camera interface  
  - RF interface  
  - SPI  
  - external memory  

---

## 🧠 FPGA Requirements (Hardware)

| Resource | Minimum | Recommended |
|----------|---------|-------------|
| LUT      | ≥ 30k  | 45k–85k    |
| BRAM     | ≥ 500 kB | 500 kB – 1 MB |
| DSP      | optional | ✔          |
| PLL / Clocking | ✔ | ✔          |
| IO count | ≥ 40   | ≥ 50       |

---

## ⏱️ Timing Requirements

| Parameter     | Value |
|--------------|------|
| Pixel clock   | ~40–50 MHz |
| System clock  | ~80–150 MHz |
| TX clock      | ~100–300 MHz (interface-dependent) |

---

## 🎯 Target FPGA Devices

| Device | Status | Notes |
|--------|-------|------|
| Lattice ECP5 LFE5U-45F | 🥇 Target | Best performance / cost balance |
| Lattice ECP5 LFE5U-85F | 🟢 Alternative | Resource margin |
| Lattice iCE40          | ❌ Rejected | Insufficient resources |
| Lattice CrossLink-NX   | ❌ Rejected | Limited LUT/IO capacity |

LIFCL-17-7SG72C

---

## 💡 Design Assumptions

- **Streaming architecture** – no full framebuffer (initial stage)  
- **Modular design** – SPI-based control plane  
- **Scalability** – resolution and throughput can be increased  
- **FPGA-first approach** – entire datapath implemented in hardware  

---

## ⚠️ Design Status

The design is currently in the **requirements definition phase**.

At this stage:
- System requirements are defined  
- FPGA family selection is narrowed down  
- High-level architecture is established  

Further work will include:
- Interface selection (camera / RF)  
- Detailed FPGA resource estimation  
- Timing closure strategy  
- PCB design and signal integrity analysis  
