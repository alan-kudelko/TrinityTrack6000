## 🎥 AVProcessingBoard – Overview

The **AVProcessingBoard** is an FPGA-based module responsible for real-time audio and video data processing.

It is designed as a high-throughput datapath unit, where all time-critical operations are implemented in hardware.  
A separate MCU (STM32) is used only for control and configuration via SPI.

The design is based on **Lattice FPGA devices**, selected for their balance between performance, cost, and toolchain accessibility.

> ⚠️ Note: All parameters below are preliminary and subject to change during the design phase.

---

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
