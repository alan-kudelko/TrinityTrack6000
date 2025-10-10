# TrinityTrack6000

**TrinityTrack6000** is a physically built, custom, multi-MCU remote-controlled tank, constructed from 2 mm and 3 mm steel sheets. It features a suspension system and a movable turret equipped with a BB‑pellet pistol for non-lethal target practice and sensor payload mounting. The project emphasizes a modular hardware approach: each functionality is implemented on a dedicated PCB module, enabling easy system expansion, parallel development of hardware and firmware, and straightforward integration of new features. This platform combines real-world mechanical construction with advanced electronics, serving both as a functional robotic vehicle and as an educational system for exploring embedded multi-MCU coordination, real-time control, and hardware integration.

The system consists of multiple PCBs based on STM32, Infineon, NXP, IXYS and FPGA microcontrollers, responsible for:

1. **ControlBoard** – **STM32 + Infineon**  
   - **STM32**: Main logic and system coordination, communication via radio 
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
- Understanding and working with different real-time operating systems
- Synchronization between MCUs and failsafe implementations
- FPGA programming and real-time video/audio processing
- Radar theory and object detection
- Power electronics (switch-mode power supplies) and H-bridge motor control

Given the project’s complexity, it is developed in stages, focusing on one PCB at a time. Each PCB has a dedicated README detailing system operation, component selection, program implementation or theory behind it.

---

## 🔧 Status - Planning phase and component selection for **ControlBoard** (STM32 + Infineon)

---

## 🧪 Project Goals
- Communication between multiple MCUs and real-time systems
- Synchronization between MCUs and failsafe implementations
- FPGA programming and real-time video/audio processing
- Radar theory and object detection
- Power electronics (switch-mode power supplies)
- Learning different real-time system architectures along with other industrial MCUs
- Practical implementation of H-bridge and motor control with current and temperature feedback
- Designing modular PCBs and integrating multiple MCUs within a single system

---

## ✳️ Planned Technologies & Tools

- **MCUs & Processors**: STM32, Infineon, NXP, IXYS, FPGA  
- **Sensor Modules**: Temperature, acceleration, gas sensors (CO₂, O₂, ethanol, methane, butane), GPS (TBD)  
- **Communication**: Radio modules
- **Motor Control**: H-bridge, servo drivers, current and temperature monitoring  
- **Audio/Video**: FPGA-based capture, hardware compression (H.264/265), transmission  
- **Power Electronics**: Switch-mode power supplies, battery voltage conversion, power distribution  
- **Software / RTOS**: ThreadX, Micrium uC (TBD)  
- **Development Tools**: VS Code, CMake, Ninja, ARM-GCC toolchain  
- **PCB Design**: KiCAD 
- **Debugging & Monitoring**: Multimeter, Oscilloscope

---

## 🛠️ Development Strategy

Each system board will be developed, prototyped, and tested sequentially, following the same design pipeline.  
The process for each board includes:

1. **Component Selection** – choosing appropriate components based on functionality, performance, and availability.  
2. **Schematic Design** – creating the circuit schematic and verifying signal flow.  
3. **Breadboard Testing & Firmware Development** – validating the concept on a breadboard while simultaneously developing and testing the firmware.  
4. **Electrical Design** – preparing detailed electrical connections and pin mapping for PCB integration.  
5. **PCB Design & Prototyping** – designing and manufacturing the first PCB version for testing.  
6. **Testing & Validation** – verifying correct operation, signal integrity, and communication between modules.  
7. **Iteration & Optimization** – refining both hardware and firmware based on test results.

*Note:*  
- In parallel, mechanical design and construction of the tank chassis, turret, and suspension are being developed alongside the electronics to ensure seamless integration between hardware and mechanics.

---

## 🌟 Key Features

- **Physically built prototype** — a fully custom-designed, steel-constructed tracked platform with an operational turret and BB gun.  
- **Modular electronic architecture** — each functional domain (control, power, sensors, video, etc.) is handled by a dedicated PCB, designed for scalability and ease of maintenance.  
- **Multi-MCU communication** — cooperative control between STM32, Infineon, FPGA, and other processors using various real-time communication interfaces.  
- **Real-time systems** — development and testing of different RTOS solutions, synchronization methods, and failsafe mechanisms.  
- **Advanced motor control** — H-bridge integration, servo signal routing, and feedback monitoring for current and temperature.  
- **High-performance video/audio processing** — FPGA-based real-time compression (H.264/265) and wireless transmission to the operator.  
- **Custom sensor integration** — environmental data collection (temperature, acceleration, gas concentration, GPS) for real-time feedback.  
- **Scalable power management** — modular power distribution with switch-mode power supplies and battery voltage conversion.  
- **Mechanical and electronic co-design** — parallel development of the chassis, suspension system, and electronic control boards.

---

## 🗺️ Project Roadmap

1. **ControlBoard** — Develop main logic and motor/servo interface, implement communication (radio) and current/temperature monitoring.  
2. **SensorHub** — Integrate and validate all environmental sensors, GPS, and accelerometers.  
3. **Motor & Servo Interface Board** — Finalize H-bridge, signal routing, and power distribution for motors and servos.  
4. **PowerControlBoard** — Design and test battery voltage conversion and power distribution.  
5. **VideoAudioBoard** — Implement FPGA-based audio/video capture, compression, and wireless transmission.  
6. **FireControlBoard** — Develop fire control system using laser rangefinding.  
7. **RadarBoard** — Integrate and validate custom radar for object detection.


## 🖼️ Project Overview

![Tank Chassis](Media/Tank_Body_1.jpg)
![Tank Chassis](Media/Tank_Body_2.jpg)


**Physical Specifications:**

| Dimension            | Value        |
|----------------------|-------------|
| Length               | 70 cm       |
| Width (without tracks)| 32 cm       |
| Width (with tracks)  | 42 cm       |
| Height               | 20 cm       |
| Ground Clearance     | 5 cm        |
| Weight               | 25 kg       |

**Performance:**

- Maximum Speed: 5 km/h
- Features: Suspension system, rotating turret with BB gun

## ⚙️ Technical Overview 

### 1. 📦 Project Structure & File Overview

    📦 TrinityTrack6000/
 	 │
     ├── .git
 	 ├── Datasheets/              # Documentation of used ICs and components
	 ├── docs/                    # Documentation generated by Doxygen for each MCU's files
	 │
	 ├── MainControlBoard/                
	 ├── HardwareControlBoard/
     ├── PowerControlBoard/
     ├── SensorHubBoard/
     ├── RadarModuleBoard/
     ├── AVProcessingBoard/
     ├── FireControlBoard/
     ├── MechanicalDesign/
     |
  	 ├── Media/                   # Folder containing all the drawings and schematics
     |
	 ├── tools/                  # Helper tools
 	 │   └── arm-gnu-toolchain-14.3.rel1-mingw-w64-x86_64-arm-none-eabi/          # arm-gcc toolchain                         
     |
 	 ├── .gitignore            # Git ignore configuration
	 ├── LICENSE               # Project license file
	 └── README.md             # Project overview and documentation

---

### 2. Design Considerations

Given the complexity of the TrinityTrack6000 project, both mechanically and electronically, it is essential to define clear design requirements to guide the development process. 

Mechanically, the tank must be equipped with a robust drivetrain capable of moving a substantial mass from a standstill. This requires careful selection of motors with sufficient torque, appropriate gear ratios, and high-quality mechanical components to ensure reliability and longevity under load.

From an electronic and system perspective, the modularity of the platform imposes strict requirements on inter-board connections. Communication protocols must be chosen with care to ensure reliable data transfer between multiple MCUs and peripheral boards. Signal integrity, latency, and potential interference must be considered, especially given the combination of SPI, I²C, UART, and wireless links.

Additionally, to simplify the interconnections between boards and reduce potential points of failure, the design aims to minimize the number of signals physically routed between PCBs. Wherever possible, data will be transmitted digitally rather than as separate analog or discrete control lines, ensuring a cleaner layout, reduced wiring complexity, and more reliable communication across the modular system.

Proper planning of these design constraints helps prevent integration issues, ensures smooth operation across all modules, and allows the platform to be both expandable and maintainable for future upgrades and experiments.

#### 2.1 Mechanical Considerations

#### 2.2 Electrical Considerations

Given the modular nature of the TrinityTrack6000 and the high current and data requirements of its subsystems, careful planning of inter-board electrical connections is crucial. This includes minimizing signal lines, ensuring reliable data transmission, and safely distributing power to all PCBs.

- **Signal count per board**: Determine the minimum number of control and feedback signals required between each PCB to ensure full functionality while minimizing wiring complexity.
- **Connector selection**: Choose robust connectors suitable for modular stacking and repeated assembly/disassembly. Consider mechanical alignment, current rating, and pin density.
- **Data bus frequency & throughput**: Define the operating frequency for digital communication buses (I²C, SPI, or UART) and estimate achievable data rates to meet system requirements.
- **Power connectors**: Select power connectors capable of handling the maximum current demand of each PCB, including peak currents for motors and servos, while ensuring safe voltage distribution.

##### 2.2.1 Interconnection between boards

Interconnections between PCBs will be implemented using a set of standardized connectors to ensure reliable mechanical and electrical connections, ease of assembly, and maintainability. Each connector will be selected based on pin count, current rating, and mechanical robustness.

##### 2.2.1.1 **Data lines:**  

The digital communication between boards, including SPI, I²C, and UART signals, will be routed through dedicated data connectors. The choice of connector type and layout will aim to minimize crosstalk, signal reflections, and propagation delays while supporting the required bus speeds.

For connection of the data lines, I decided to use **JST PH 2.0 mm pitch connectors**. These will carry the necessary control and feedback signals between the PCBs.

![JST](/Media/ElectricalComponents/JST_Connectors.png)


##### 2.2.1.2 **Power lines:**  

Power distribution between PCBs will utilize high-current connectors capable of handling both continuous and peak loads. Proper conductor sizing, contact materials, and secure locking mechanisms will be considered to ensure safe and stable voltage supply across all modules.

All system boards and peripherals will receive power through **Phoenix Contact connectors**. These connectors provide secure and high-current-capable connections for modular assembly. Maximum current rating: 40 A for **PowerControlBoard** , 20 A for **HardwareControlBoard** (DC motors and H-Bridge) 

**Logic & Low-Power Boards**  
   - Connections with Phoenix Contact COMBICON connectors (2–6 A rated)  
   - Handles power for logic and low-current peripherals safely

![Phoenix Contact 1764875](/Media/ElectricalComponents/PHOENIX_1764875_Connector.png)

**Motor H-Bridge Board**  
   - Connections with Phoenix Contact COMBICON / screw-terminal style (up to 20 A)  
   - Supplies motors and servo power with robust connections

![Phoenix Contact 1804810](/Media/ElectricalComponents/PHOENIX_1804810_Connector.png)

**PowerControlBoard (Main Supply Input)**  
   - Phoenix Contact 1017521 connector (up to 40 A)  
   - Main battery input to deliver high-current power reliably
     
![Phoenix Contact 1017521](/Media/ElectricalComponents/PHOENIX_1017521_Connector.png)


##### 2.2.2 SPI Max frequency vs line length and type

| Line type         | Length [cm] | Frequency max [MHz] | Single bit period [ns/bit] | Packet transfer time [μs] |
|-------------------|-------------|--------------------|----------------------------|--------------------------|
| PCB trace         | 15          | 25                 | 40                         | 40.96                    |
| Unshielded wire   | 15          | 10                 | 100                        | 102.4                    |
| Unshielded wire   | 30          | 5                  | 200                        | 204.8                    |

*Note:* Example packet size is 128 bytes (1024 bits) to estimate transmission period

---

##### 2.2.3 I²C Max frequency vs line length and type

| Line type         | Length [cm] | Frequency max [MHz] | Single bit period [ns/bit] | Packet transfer time [μs] |
|------------------|-------------|--------------------|----------------------------|--------------------------|
| PCB trace         | 15          | 1                  | 1000                       | 1024                     |
| Unshielded wire   | 15          | 0.8                | 1250                       | 1280                     |
| Unshielded wire   | 30          | 0.5                | 2000                       | 2048                     |

*Note:* Example packet size is 128 bytes (1024 bits) to estimate transmission period

---
