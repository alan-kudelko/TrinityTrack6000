# TrinityTrack6000

**TrinityTrack6000** is a custom-built, multi-MCU remote-controlled tank constructed from 2 mm and 3 mm steel sheets.

The project focuses on designing PCB modules corresponding to different system functionalities, allowing for easy expansion and addition of new features.

The system consists of multiple PCBs based on STM32, Infineon, NXP, IXYS, FPGA, and AVR microcontrollers, responsible for:

- Remote control via radio or Bluetooth
- Audio and video capture, hardware compression (H.264/265) and transmission back to the operator
- Motor and servo control
- Fire control system using a laser rangefinder
- Sensor integration (temperature, acceleration, gas sensors, GPS, etc.)
- Integration of a custom radar for object detection

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
- **Microcontrollers:** STM32, Infineon, NXP, IXYS, AVR
- **FPGA:** real-time video/audio processing
- **Communication:** radios, optional Wi-Fi module
- **Compression:** H.264/H.265 hardware
- **Sensors:** GPS, accelerometer, gas sensors (CO₂, O₂, EtOH, CH₄, C₄H₁₀), temperature, laser rangefinder
- **PCB Design:** modular architecture
- **Power Management:** switch-mode power supplies
- **Development Tools:** VS Code, CMake, Ninja, STM32Cube, AVR GCC, etc.

---

## 🛠️ Development Strategy
1. **ControlBoard** – Main MCU (STM32 + Infineon): radio/Bluetooth, motor & servo control, current/temperature measurement
2. **SensorHub** – AVR module: environmental sensing (GPS, gases, temperature, accelerometer, others)
3. **RadarBoard** – NXP module: radar-based object detection
4. **BalisticComputer** – IXYS MCU: fire control calculations
5. **VideoBoard** – FPGA: audio/video capture, hardware compression, transmission
6. **HBridgeBoard** – Motor driver module (H-Bridge, 3 channels)
7. **HardwareControlBoard** – Power management and failsafe circuitry

---

## 🌟 Key Features
- Modular multi-MCU system
- Real-time audio/video processing and transmission
- Wireless remote control
- Sensor fusion for environmental awareness
- Fire control and targeting calculations
- Failsafe and redundant communication design
- Expandable PCB architecture

---

## 🗺️ Project Roadmap
1. Develop **ControlBoard** – radio, Bluetooth, motor/servo control
2. Develop **SensorHub** – environmental sensing
3. Develop **RadarBoard** – radar integration
4. Develop **BalisticComputer** – fire control algorithms
5. Develop **VideoBoard** – audio/video capture, compression, transmission
6. Develop **HBridgeBoard** – motor driver integration
7. Develop **HardwareControlBoard** – power supply and failsafe

