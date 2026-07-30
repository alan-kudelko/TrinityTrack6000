# Thermal Budget

## 1. Purpose

This document defines the thermal budget of the `HardwareControlBoard` for a specified hardware configuration.

Its purpose is to estimate the worst-case power dissipation of all major heat-generating components and to verify that the PCB, selected components, and cooling strategy satisfy the project's thermal design requirements.

The presented calculations are valid only for the hardware configuration and operating conditions specified in this document. Any significant changes to the power stage topology, component selection, operating conditions, or PCB design shall require the thermal budget to be re-evaluated.

## 2. Design Assumptions

Unless stated otherwise, all calculations presented in this document are based on the following design assumptions.

| Parameter | Value | Notes |
|-----------|------:|-------|
| Supply Voltage | 12 V | Nominal operating voltage |
| Ambient Temperature | 40 °C | Worst-case design condition |
| Cooling Method | Natural convection | No forced airflow |
| PCB Technology | FR-4, 4-layer | 1 oz copper |
| PCB Thickness | 1.6 mm | Standard stack-up |
| PWM Frequency | 40 kHz | Unless stated otherwise |
| Duty Cycle | Worst-case | Defined per subsystem |
| Component Parameters | Datasheet maximum values | Conservative design approach |

## 3. PCB Thermal Limits

The `HardwareControlBoard` is designed for continuous operation under natural convection without forced airflow. The PCB itself is used as the primary heat-spreading element for all power components.

This section defines the thermal capability of the PCB and establishes the allowable thermal dissipation budget used throughout this document.

| Parameter | Value | Notes |
|-----------|------:|-------|
| PCB Technology | FR-4 | 4-layer PCB |
| PCB Dimensions | 100 × 200 mm | Overall board dimensions |
| Total PCB Area | 200 cm² | Single PCB surface area |
| Copper Thickness | 35 µm (1 oz) | All copper layers |
| Number of Copper Layers | 4 | Standard stack-up |
| Effective Copper Coverage | 75 % | Estimated average copper fill across all layers |
| Effective Copper Area | 600 cm² | Total heat-spreading copper area |
| Cooling Method | Natural convection | No forced airflow |
| Ambient Temperature | 40 °C | Worst-case design condition |

> **Note:** The effective copper area is estimated as the sum of the usable copper surface on all PCB layers. The actual value depends on the final PCB layout and may vary between revisions.

## 4. Heat Sources

This section estimates the power dissipation of each major heat-generating subsystem implemented on the `HardwareControlBoard`.

The total thermal budget is obtained by summing the worst-case power dissipation of all individual heat sources under the design assumptions defined in Section 2.

### 4.1 Power Loss Model

#### Conduction Losses

The conduction losses of a MOSFET are calculated using:

```text
Pcond = IRMS² × RDS(on)
```

where:

- **Pcond** — conduction power loss [W]
- **IRMS** — RMS current through the MOSFET [A]
- **RDS(on)** — drain-to-source on-state resistance [Ω]

#### Switching Losses

The switching losses are estimated using:

```text
Psw = ½ × VDS × ID × (tr + tf) × fPWM
```

where:

- **Psw** — switching power loss [W]
- **VDS** — drain-to-source voltage [V]
- **ID** — drain current during switching [A]
- **tr** — MOSFET rise time [s]
- **tf** — MOSFET fall time [s]
- **fPWM** — PWM switching frequency [Hz]

### 4.2 Low Power Section

The Low Power Section (LPS) consists of **eight independent low-side outputs** intended for low-current loads supplied from the 12 V system rail.

The outputs are divided into two functional groups:

- **4 PWM outputs** operating up to **40 kHz**, driven by dedicated **2EDN7533FXTMA1** gate drivers.
- **4 digital outputs** intended for ON/OFF control using the same MOSFET architecture.

All outputs utilize the **PJA3406_R1_00001** N-channel MOSFET operating as a low-side switch.

The gate driver is supplied directly from the **12 V rail**, allowing the MOSFET to be driven close to its fully enhanced operating region. Therefore, all electrical parameters used in the following calculations are taken from the datasheet values specified for **VGS = 10 V**, representing the closest guaranteed operating point.

The thermal analysis presented below evaluates the worst-case power dissipation of a single PWM output operating continuously at the maximum specified load current.

##### Design Parameters

| Parameter | Symbol | Value |
|-----------|--------|------:|
| Supply Voltage | VDS | 12 V |
| Gate Voltage | VGS | 10 V (datasheet) |
| Continuous Load Current | ID | 1 A |
| PWM Frequency | fPWM | 40 kHz |
| Maximum PWM Duty Cycle | DMAX | 100 % |
| MOSFET | — | PJA3406_R1_00001 |
| Gate Driver | — | 2EDN7533FXTMA1 |

##### MOSFET Parameters (Worst-Case Datasheet Values)

| Parameter | Symbol | Value |
|-----------|--------|------:|
| Drain-Source On Resistance | RDS(on) | 48 mΩ |
| Total Gate Charge | Qg | 5.8 nC |
| Rise Time | tr | 39 ns |
| Fall Time | tf | 28 ns |
| Maximum Junction Temperature | TJ(max) | 150 °C |

The worst-case values specified above are used throughout the calculations to provide conservative estimates of power dissipation and ensure adequate thermal design margin.


##### Total Low Power Section Power Dissipation

| Output Type | Quantity | Power Loss per Channel | Total Power Loss |
|-------------|---------:|-----------------------:|-----------------:|
| PWM Outputs | 4 | 64.1 mW | 256.4 mW |
| Digital Outputs | 4 | 48.0 mW | 192.0 mW |
| **Total LPS Power Dissipation** | **8** | — | **448.4 mW** |

> **Note:** The calculations assume worst-case operating conditions. All four PWM outputs are assumed to operate continuously at 40 kHz while carrying the maximum continuous load current of 1 A. The remaining four digital outputs are assumed to conduct continuously at the same load current without switching losses.

### 4.2 Medium Power Section

#### 4.2.1 Heater Output

The heater output is implemented as a **single low-side MOSFET** using the **PJQ4548P-AU_R2_002A1** N-channel MOSFET.

Both **conduction losses** and **switching losses** are included in the thermal analysis.

##### Design Parameters

| Parameter | Symbol | Value |
|-----------|--------|------:|
| Supply Voltage | VDS | 12 V |
| Gate Voltage | VGS | 12 V |
| Continuous Load Current | ID | 5 A |
| PWM Frequency | fPWM | 40 kHz |
| Maximum PWM Duty Cycle | DMAX | 100 % |
| MOSFET | — | PJQ4548P-AU_R2_002A1 |
| Gate Driver | — | 2EDN7533FXTMA1 |

##### MOSFET Parameters (Worst-Case Datasheet Values)

| Parameter | Symbol | Value |
|-----------|--------|------:|
| Drain-Source On Resistance | RDS(on) | 9.1 mΩ |
| Total Gate Charge | Qg | 13 nC |
| Rise Time | tr | 3 ns |
| Fall Time | tf | 3 ns |
| Maximum Junction Temperature | TJ(max) | 175 °C |

The worst-case values specified above are used throughout the calculations to provide conservative estimates of power dissipation and ensure adequate thermal design margin.

##### Total Heater Output Power Dissipation

| Loss Type | Quantity | Power Loss per MOSFET | Total Power Loss |
|-----------|---------:|----------------------:|-----------------:|
| Conduction Loss | 1 | 0.228 W | 0.228 W |
| Switching Loss | 1 | 0.007 W | 0.007 W |
| **Total Heater Output Power Dissipation** | **1** | — | **0.235 W** |

> **Note:** The calculations assume worst-case operating conditions with a continuous load current of **5 A** and a maximum PWM duty cycle of **100%**. The heater output is implemented using a single low-side MOSFET. Conduction losses are calculated using the **maximum** datasheet **RDS(on)** value, while switching losses are estimated using the typical switching times specified in the datasheet.

#### 4.2.2 Winch H-Bridge

The winch output is implemented as a **single full H-bridge** using **four PJQ4548P-AU_R2_002A1 N-channel MOSFETs** driven by the **UCC21551** isolated gate driver.

Both **conduction losses** and **switching losses** are included in the thermal analysis.

##### Design Parameters

| Parameter | Symbol | Value |
|-----------|--------|------:|
| Supply Voltage | VDS | 12 V |
| Gate Voltage | VGS | 12 V |
| Continuous Motor Current | ID | 10 A |
| PWM Frequency | fPWM | 40 kHz |
| Maximum PWM Duty Cycle | DMAX | 85 % |
| H-Bridges | — | 1 |
| Conducting MOSFETs | — | 2 |
| MOSFET | — | PJQ4548P-AU_R2_002A1 |
| Gate Driver | — | UCC21551 |

##### MOSFET Parameters (Worst-Case Datasheet Values)

| Parameter | Symbol | Value |
|-----------|--------|------:|
| Drain-Source On Resistance | RDS(on) | 9.1 mΩ |
| Total Gate Charge | Qg | 13 nC |
| Rise Time | tr | 3 ns |
| Fall Time | tf | 3 ns |
| Maximum Junction Temperature | TJ(max) | 175 °C |

The worst-case values specified above are used throughout the calculations to provide conservative estimates of power dissipation and ensure adequate thermal design margin.

##### Total Winch H-Bridge Power Dissipation

| Loss Type | Quantity | Power Loss per MOSFET | Total Power Loss |
|-----------|---------:|----------------------:|-----------------:|
| Conduction Loss | 2 | 0.910 W | 1.820 W |
| Switching Loss | 2 | 0.014 W | 0.029 W |
| **Total Winch H-Bridge Power Dissipation** | **2** | — | **1.849 W** |

> **Note:** The calculations assume worst-case operating conditions with a continuous motor current of **10 A** and a maximum PWM duty cycle of **85%**. During normal operation, only **two MOSFETs** conduct the motor current simultaneously in the H-bridge. Therefore, the total power dissipation is calculated for **two actively conducting and switching MOSFETs**. Conduction losses are calculated using the **maximum** datasheet **RDS(on)** value, while switching losses are estimated using the typical switching times specified in the datasheet.
#### 4.2.3 Stepper Motor Drivers

The `HardwareControlBoard` supports up to **six external stepper motor drivers** implemented using the **Texas Instruments DRV8434S** integrated bipolar stepper motor driver.

Unlike the previous power stages, the DRV8434S integrates both the power MOSFET bridge and current regulation circuitry within a single IC. Consequently, the thermal analysis is based on the power dissipation methodology recommended by **Texas Instruments**, which includes conduction losses, switching losses and the device quiescent power consumption.

##### Design Parameters

| Parameter | Symbol | Value |
|-----------|--------|------:|
| Supply Voltage | VM | 12 V |
| Motor Current | IRMS | 2 A / phase |
| PWM Frequency | fPWM | 40 kHz |
| Stepper Drivers | — | 6 |
| Driver | — | DRV8434S |

##### Driver Thermal Parameters

| Parameter | Symbol | Value |
|-----------|--------|------:|
| Junction-to-Ambient Thermal Resistance | RθJA | 29.7 °C/W |
| Junction-to-Board Thermal Resistance | RθJB | 9.3 °C/W |
| Junction-to-Case (Bottom) | RθJC(bottom) | 2.4 °C/W |
| Maximum Junction Temperature | TJ(max) | 150 °C |

The thermal characteristics specified above correspond to the recommended PCB layout with an exposed thermal pad and multiple thermal vias beneath the device.

##### Total Stepper Driver Power Dissipation

| Loss Type | Quantity | Power Loss per Driver | Total Power Loss |
|-----------|---------:|----------------------:|-----------------:|
| **Total Driver Power Dissipation** | **6** | **~0.9–1.2 W** | **~5.4–7.2 W** |

> **Note:** The DRV8434S integrates the complete H-bridge, gate drive circuitry and current regulation into a single package. Unlike discrete MOSFET implementations, the total power dissipation cannot be accurately determined using only MOSFET conduction and switching loss equations. Instead, the thermal analysis follows the methodology recommended by **Texas Instruments**, taking into account conduction losses, switching losses and quiescent device power. Final power dissipation depends on the selected microstepping mode, current regulation settings, motor operating point and PCB thermal performance.

### 4.3 High Power Section

The High Power Section (HPS) consists of two independent full H-bridges designed to drive the propulsion motors of the platform.

Each H-bridge is implemented using discrete **STL160N4F7** N-channel MOSFETs driven by an isolated **UCC21551** gate driver. The thermal analysis includes both conduction and switching losses, which dominate the overall power dissipation of this section.

##### Design Parameters

| Parameter | Symbol | Value |
|-----------|--------|------:|
| Supply Voltage | VDS | 12 V |
| Gate Voltage | VGS | 10 V (datasheet) |
| Continuous Motor Current | ID | 40 A |
| PWM Frequency | fPWM | 40 kHz |
| Maximum PWM Duty Cycle | DMAX | 85 % |
| H-Bridges | — | 2 |
| Conducting MOSFETs | — | 4 |
| MOSFET | — | STL160N4F7 |
| Gate Driver | — | UCC21551 |

##### MOSFET Parameters (Worst-Case Datasheet Values)

| Parameter | Symbol | Value |
|-----------|--------|------:|
| Drain-Source On Resistance | RDS(on) | 2.5 mΩ |
| Total Gate Charge | Qg | 29 nC |
| Rise Time | tr | 6.6 ns |
| Fall Time | tf | 5.7 ns |
| Maximum Junction Temperature | TJ(max) | 175 °C |

The worst-case values specified above are used throughout the calculations to provide conservative estimates of power dissipation and ensure adequate thermal design margin.

##### Total High Power Section Power Dissipation

| Loss Type | Quantity | Power Loss per MOSFET | Total Power Loss |
|-----------|---------:|----------------------:|-----------------:|
| Conduction Loss | 4 | 3.400 W | 13.600 W |
| Switching Loss | 4 | 0.118 W | 0.472 W |
| **Total HPS Power Dissipation** | **4** | — | **14.07 W** |

> **Note:** The calculations assume worst-case operating conditions with a continuous motor current of **40 A** and a maximum PWM duty cycle of **85%**. During normal operation, only two MOSFETs conduct the motor current simultaneously in each H-bridge. Since the High Power Section consists of two independent H-bridges, the total power dissipation is calculated for **four actively conducting and switching MOSFETs**. Conduction losses are calculated using the **maximum** datasheet **RDS(on)** value, while switching losses are estimated using the typical switching times specified in the datasheet.

### 4.4 Current Sense Shunts

Current sensing within the `HardwareControlBoard` is performed using low-resistance precision shunt resistors.

The High Power Section (HPS) utilizes two independent current measurement channels for the left and right H-bridges, while the winch motor uses a dedicated shunt resistor for current monitoring.

The power dissipation of each shunt resistor is calculated using:

```text
Pshunt = I² × Rshunt
```

where:

- **Pshunt** — shunt power dissipation [W]
- **I** — load current [A]
- **Rshunt** — shunt resistance [Ω]

##### Design Parameters

| Parameter | Symbol | Value |
|-----------|--------|------:|
| HPS Rated Current | I | 40 A |
| Winch Rated Current | I | 10 A |
| HPS Shunt Resistance | Rshunt | 1 mΩ |
| Winch Shunt Resistance | Rshunt | 5 mΩ |
| Maximum PWM Duty Cycle | DMAX | 85 % |

##### HPS Current Sense Shunt

```text
Pshunt = D × I² × Rshunt
```

Substituting the design parameters:

```text
Pshunt = 0.85 × (40 A)² × 1 mΩ
Pshunt = 1.36 W
```

##### Winch Current Sense Shunt

```text
Pshunt = D × I² × Rshunt
```

Substituting the design parameters:

```text
Pshunt = 0.85 × (10 A)² × 5 mΩ
Pshunt = 0.425 W
```

##### Total Current Sense Shunt Power Dissipation

| Current Sense Channel | Quantity | Power Loss per Shunt | Total Power Loss |
|-----------------------|---------:|---------------------:|-----------------:|
| HPS Shunts (1 mΩ) | 2 | 1.36 W | 2.720 W |
| Winch Shunt (5 mΩ) | 1 | 0.425 W | 0.425 W |
| **Total Shunt Power Dissipation** | **3** | — | **3.145 W** |

> **Note:** The calculations assume a maximum PWM duty cycle of **85%**, representing the worst-case continuous operating condition of the H-bridge power stages.


#### 4.5 Other Components

##### 4.5.1 3.3 V LDO Regulator

The **TLV76133** low-dropout regulator provides the primary **3.3 V** supply rail for the control electronics. The regulator is powered from the **5 V** rail generated by the onboard power conversion stage.

##### Design Parameters

| Parameter | Symbol | Value |
|-----------|--------|------:|
| Input Voltage | VIN | 5 V |
| Output Voltage | VOUT | 3.3 V |
| Maximum Output Current | IOUT | 500 mA |
| Regulator | — | TLV76133 |

##### Total LDO Power Dissipation

| Loss Type | Quantity | Power per Device | Total Power |
|-----------|---------:|-----------------:|------------:|
| LDO Dissipation | 1 | 0.850 W | 0.850 W |

> **Note:** The worst-case power dissipation assumes the regulator continuously supplies the maximum rated output current of **500 mA**. The power loss is calculated using the linear regulator equation **P = (VIN − VOUT) × IOUT**. Under normal operating conditions, the average power dissipation is expected to be significantly lower due to the substantially reduced current consumption of the digital control circuitry.

## 5. Thermal Summary

This section summarizes the estimated power dissipation of all major heat-generating subsystems implemented on the `HardwareControlBoard`.

The objective is to establish the overall PCB thermal budget and verify that the selected PCB technology, copper area, and cooling method are capable of dissipating the generated heat under worst-case operating conditions.

Unless stated otherwise, all values represent **worst-case theoretical estimates** based on conservative first-order calculations and maximum datasheet parameters.

| Section | Worst-Case Power Dissipation | Notes |
|---------|-----------------------------:|-------|
| Low Power Section (LPS) | 0.448 W | MOSFET conduction and switching losses |
| Medium Power Section (MPS) | 9.284 W | Heater (0.235 W), Winch (1.849 W), Stepper Drivers (7.200 W) |
| High Power Section (HPS) | 14.070 W | MOSFET conduction and switching losses |
| Current Sense Shunts | 3.145 W | Shunt conduction losses |
| Other Components | 0.850 W | TLV76133 LDO, gate drivers, analog front-end, logic ICs |
| **Total PCB Thermal Budget** | **27.797 W** | Worst-case estimate |

The resulting thermal budget will be used to verify:

- PCB heat spreading capability,
- copper area utilization,
- thermal via placement,
- component temperature margins,
- heatsink requirements (if applicable),
- expected PCB operating temperature.

> **Note:**
> - The presented values are conservative worst-case design estimates intended for thermal budgeting.
> - Individual subsystem losses are calculated independently using worst-case operating assumptions. Consequently, the total thermal budget represents an upper design limit rather than a simultaneously achievable operating condition.
> - The final thermal performance will be validated experimentally during prototype bring-up and hardware characterization.

## 6. Design Conclusions

The completed thermal analysis estimates a **worst-case PCB power dissipation of approximately 30.618 W** under maximum design loading. This value represents a conservative thermal budget used to evaluate the PCB layout, component placement and cooling capability.

The analysis leads to the following design conclusions:

- The **High Power Section (HPS)** is the dominant heat source on the PCB and therefore requires the largest copper area and efficient thermal spreading.
- The **Medium Power Section (MPS)**, particularly the integrated stepper motor drivers and winch H-bridge, represents the second largest source of heat and should be physically separated from the HPS to improve thermal distribution.
- Components with significant power dissipation, including MOSFETs, stepper motor drivers, current shunts and linear regulators, should be distributed across the PCB to avoid localized thermal hotspots.
- Components utilizing exposed thermal pads, such as the **DRV8434S** stepper motor drivers, should follow the manufacturer's recommended PCB layout with adequate copper area and multiple thermal vias.
- The calculated thermal budget should be used as a guideline for PCB layout optimization and heatsinking rather than as an indication of continuous operating power.

> **Note:** The total PCB thermal budget represents a theoretical worst-case scenario in which all major power stages are assumed to operate simultaneously at their maximum design load. Such operating conditions are not expected during normal system operation but are intentionally used to provide adequate design margin and verify the thermal robustness of the HardwareControlBoard.

The analytical results presented in this document should be verified during prototype testing using temperature measurements under representative operating conditions. Any discrepancies between calculated and measured temperatures should be incorporated into future PCB revisions.