# TrinityTrack6000

**TrinityTrack6000** is a physically built, custom, multi-MCU remote-controlled tank, constructed from 2 mm and 3 mm steel sheets.

The project emphasizes a modular hardware approach: each functionality is implemented on a dedicated PCB module, enabling easy system expansion, parallel development of hardware and firmware, and straightforward integration of new features.

This platform combines real-world mechanical construction with advanced electronics, serving both as a functional robotic vehicle and as an educational system for exploring embedded multi-MCU coordination, real-time control, and hardware integration.

The system consists of multiple PCBs based on STM32, Infineon, NXP, IXYS and FPGA microcontrollers, responsible for:

1. **ControlBoard** – **STM32 + Infineon**  
   - **STM32**: Main logic and system coordination, communication via radio and Bluetooth  
   - **Infineon**: Motor and servo control, current and temperature monitoring
2. **SensorHub** – **Sensor Interface Board**  
   - Handles integration of various sensors: temperature, acceleration, gas sensors (CO₂, O₂, ethanol, methane, butane), GPS, and others
3. **MotorInterfaceBoard** – **Motor & Servo Interface Board**  
   - Routes control signals to motors and servos; includes an H-bridge for motor control and handles power distribution and signal conditioning
4. **PowerControlBoard** – **Power Conversion Board**  
   - Converts battery voltage to the required voltages for all system boards; manages power distribution and protection
5. **VideoAudioBoard** – **FPGA**  
   - Handles audio and video capture, hardware compression (H.264/265), and transmission back to the operator
6. **FireControlBoard** – **NXP / IXYS (TBD)**  
   - Responsible for the fire control system using a laser rangefinder
7. **RadarBoard** – **NXP / IXYS (TBD)**  
   - Responsible for integration of a custom radar for object detection

The project serves as a learning platform for:

- Communication between multiple MCUs and real-time systems
- Synchronization between MCUs and failsafe implementations
- FPGA programming and real-time video/audio processing
- Radar theory and object detection
- Power electronics (switch-mode power supplies)

Given the project’s complexity, it is developed in stages, focusing on one PCB at a time. Each PCB has a dedicated README detailing system operation, component selection, and program implementation.

---

## 🔧 Status
Planning phase

---

## 🧪 Goals
- Implement a modular, multi-MCU tank platform
- Enable remote control via radio and Bluetooth
- Capture, compress, and transmit audio/video in real-time
- Control motors and servos with feedback on current and temperature
- Integrate multiple sensors for environmental monitoring
- Implement a fire control system with laser rangefinding
- Learn communication, synchronization, and failsafe mechanisms between MCUs
- Explore FPGA-based processing for video/audio
- Understand radar operation and power electronics

---

## ✳️ Planned Technologies & Tools


---

## 🛠️ Development Strategy


---

## 🌟 Key Features


---

## 🗺️ Project Roadmap


