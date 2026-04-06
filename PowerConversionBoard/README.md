**PowerConversionBoard** – **Power Management & Conversion**

This board is responsible for powering the entire system.  
It performs battery voltage step-down conversion and distributes regulated voltage rails to all modules.

> ⚠️ **Status: Requirements definition**
> The module is currently in the specification phase. System power requirements, voltage rails, and load characteristics are being defined prior to schematic design.

> 🔧 Goal: Serve as an educational platform for learning **switch-mode power supply (SMPS)** design, including DC-DC conversion topologies, control loop design, component selection (inductors, MOSFETs, drivers), efficiency optimization, thermal management, and EMI/EMC considerations.

---

## 🔧 Power Architecture

The system is based on multiple independent high-efficiency buck converters:

- **TPS40170RGYR** – used for **5 V / 6 V rails**  
- **LM5146** – dedicated **12 V rail**  
- **LTC3774** – high-current **7.4 V rail (main output)**  

The main **7.4 V rail** is planned as a **multiphase converter (2–4 phases)** to:
- distribute thermal load  
- reduce ripple current  
- improve efficiency at high output currents (~40 A)  

Final phase count will be determined during detailed design and validation.

---

## 🛑 Safety & Control

The board includes a **hardware kill switch input**, allowing immediate shutdown of selected power rails:

- 12 V  
- 6 V  
- 7.4 V  

This mechanism is implemented entirely in hardware and does not rely on any MCU, ensuring deterministic and fail-safe operation.

---

## 🧠 Design Approach

The PowerConversionBoard is designed as a **fully standalone hardware module**:
- no microcontroller  
- no firmware dependency  
- all control loops implemented using analog controllers  

This approach ensures:
- predictable real-time behavior  
- high reliability  
- independence from the rest of the system

## ⚠️ Design Status

The design is currently in an early stage.

At this point:
- Power requirements and load distribution are defined  
- Key controller ICs have been selected  
- High-level architecture (including multiphase design for the main rail) is established  

The following aspects are still under development:
- Inductor and power stage component selection  
- Thermal design and efficiency estimation  
- Input filtering and protection circuitry  
- PCB layout strategy (high-current paths, grounding, EMI considerations)
