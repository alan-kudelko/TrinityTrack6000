# **Mechanical Design**

---

## 🧠 Design Goals

* Achieve reliable mobility for a ~25 kg tracked platform
* Ensure sufficient torque for movement from standstill
* Maintain modularity for easy integration with electronics and PCB stack
* Design a mechanically robust chassis resistant to load and vibrations

---

## 🗺️ Production Roadmap

* 🔄 Chassis refinement and weight optimization
* 🔄 Drive train tuning (gear ratio, efficiency, thermal behavior)

---

## 1. Mechanical Overview

### 1.1 Dimensions and Weight

| Parameter              | Value |
| ---------------------- | ----- |
| Length                 | 70 cm |
| Width (without tracks) | 32 cm |
| Width (with tracks)    | 42 cm |
| Height                 | 20 cm |
| Ground clearance       | 5 cm  |
| Weight                 | 25 kg |

Target maximum speed: **~5 km/h**

---

### 1.2 Chassis Overview

![Tank Chassis](/Media/Tank_Body_1.jpg)
![Tank Chassis](/Media/Tank_Body_2.jpg)

---

### 1.3 Drive Train

#### 1.3.1 Tracks and Sprocket

![Tank Tracks](/Media/Tank_Tracks.png)
![Sprocket Wheel](/Media/Sprocket_Wheel.png)

---

#### 1.3.2 DC Motors

The system uses **Injora 540 13T DC motors**, selected for:

* high peak power (~300 W)
* availability and cost-effectiveness

The main limitation is **very high RPM**, which makes direct drive impractical and requires gear reduction.

Motor characteristics and performance curves were analyzed in **MATLAB** based on manufacturer data and standard DC motor equations.

![Motor Parameters](/Media/Injora540_13T_Parameters.png)
![Motor Comparison](/Media/Injora540_Parameters_Comparison.png)
![Motor Selection](/Media/DC_Motor_Selection.png)

---

#### 1.3.3 Gearbox and Kinematics

| Parameter         | Value     |
| ----------------- | --------- |
| Sprocket diameter | 48 mm     |
| Motor speed       | 32500 rpm |
| Gear ratio        | 1:50      |

![Gearbox](/Media/DC_Motor_Gearbox_1_50.png)
![Gearbox Dimensions](/Media/DC_Motor_Gearbox_dims_1.png)
![Gearbox Dimensions](/Media/DC_Motor_Gearbox_dims_2.png)
![Gearbox Selection](/Media/DC_Motor_Gearbox_Selection.png)

After reduction:

* Output speed ≈ **650 rpm**
* Estimated vehicle speed ≈ **5.9 km/h**

This configuration provides a balance between achievable speed and usable torque for a heavy tracked platform.
