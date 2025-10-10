# **Mechanical Design**  

---

## 🧠 Design Goals

---

## 🗺️ Production Roadmap

- 🔄 
- 🔄 

---

## Technical Table of Contents

1. [Mechanical Overview](#1-mechanical-overview)
   - [2.1 SPI Max frequency vs line length and type](#21-spi-max-frequency-vs-line-length-and-type)
   - 
---

## 1. Mechanical Overview

### 1.1 Tank Dimensions and weight

| Dimension    |         |
|--------------|---------|
| $Length$     | 70 [cm] |
| $Width_{1}$  | 32 [cm] |
| $Width_{2}$  | 42 [cm] |
| $Height$     | 20 [cm] |
| $Ground\ clearance$ | 5 [cm] |
| $Weight$     | 25 [kg] |

###### $Width_{1}$ - Width of tank without tracks  
###### $Width_{2}$ - Width of tank with tracks  

Speed of the tank has to be at least $V_{Max}=5\frac{km}{h}$

### 1.2 Tank Body Overview

![Tank Chassis](/Media/Tank_Body_1.jpg)
![Tank Chassis](/Media/Tank_Body_2.jpg)

### 1.3 Drive Train Components

#### 1.3.1 Tracks and Sprocket Wheels

![Tank Tracks](/Media/Tank_Tracks.png)
![Tank Tracks](/Media/Sprocket_Wheel.png)

#### 1.3.2 DC Motors

The selected DC motors are Injora 540 13T models. They were chosen primarily due to their low cost and relatively high peak power output of up to 300 W.  
The main drawback of these motors is their high RPM, which requires the use of a reduction gearbox to achieve suitable torque and speed for the application.

![Tank Tracks](/Media/Injora540_13T_Parameters.png)
![Tank Tracks](/Media/Injora540_Parameters_Comparison.png)
![Tank Tracks](/Media/DC_Motor_Selection.png)

The performance graphs were generated in **MATLAB** based on the manufacturer’s specifications and the fundamental DC motor equations.  

According to these graphs, the Injora 540 13T motor reaches a peak power of around **300 W**, but its RPM is too high for practical use in an RC tank. In order to move a heavy vehicle, a reduction gearbox is required to convert the excessive RPM into usable torque.


#### 1.3.3 Reduction Gearbox

| Drivetrain parameters    |         |
|--------------|---------|
| Sprocket wheel diameter     | D=48 [mm] |
| Motor RPM  | n=32500 [rpm] |
| Gearbox reduction  ratio | r=1:50 |

![Tank Tracks](/Media/DC_Motor_Gearbox_1_50.png)
![Tank Tracks](/Media/DC_Motor_Gearbox_dims_1.png)
![Tank Tracks](/Media/DC_Motor_Gearbox_dims_2.png)
![Tank Tracks](/Media/DC_Motor_Gearbox_Selection.png)

With the selected **DC motor** and **1:50 reduction gearbox**, the theoretical sprocket speed is:  

$n_{out} = \frac{n}{r} = \frac{32500}{50} \approx 650 \ \text{rpm}$

$V_{max} = \frac{\pi \cdot D \cdot n_{out}}{60} = \frac{\pi \cdot 0.048 \cdot 650}{60} \approx 1.63 \ \text{m/s} \approx 5.88 \ \text{km/h}$


