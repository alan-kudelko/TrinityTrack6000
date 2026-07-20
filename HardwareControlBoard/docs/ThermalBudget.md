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

\[
P_{cond}=I_{RMS}^{2}\cdot R_{DS(on)}
\]

where:

- \(P_{cond}\) — conduction power loss [W]
- \(I_{RMS}\) — RMS current through the MOSFET [A]
- \(R_{DS(on)}\) — drain-to-source on-state resistance [Ω]

#### Switching Losses

The switching losses are estimated using:

\[
P_{sw}=\frac{1}{2}V_{DS}\cdot I_D\cdot(t_r+t_f)\cdot f_{PWM}
\]

where:

- \(P_{sw}\) — switching power loss [W]
- \(V_{DS}\) — drain-to-source voltage [V]
- \(I_D\) — drain current [A]
- \(t_r\) — rise time [s]
- \(t_f\) — fall time [s]
- \(f_{PWM}\) — switching frequency [Hz]

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

##### Conduction Loss

The worst-case conduction loss is calculated assuming continuous conduction at the maximum rated load current.

\[
P_{cond}=I_D^2 \cdot R_{DS(on)}
\]

Substituting the design parameters:

\[
P_{cond}=(1\ \mathrm{A})^2 \cdot 48\,\mathrm{m\Omega}
\]

\[
P_{cond}=0.048\ \mathrm{W}
\]

##### Switching Loss

The switching loss is calculated assuming continuous PWM operation at the maximum switching frequency.

\[
P_{sw}=\frac{1}{2} \cdot V_{DS} \cdot I_D \cdot (t_r+t_f)\cdot f_{PWM}
\]

Substituting the design parameters:

\[
P_{sw}=\frac{1}{2}\cdot12\cdot1\cdot(39+28)\,\mathrm{ns}\cdot40\,000
\]

\[
P_{sw}=0.0161\ \mathrm{W}
\]

##### Total Low Power Section Power Dissipation

| Output Type | Quantity | Power Loss per Channel | Total Power Loss |
|-------------|---------:|-----------------------:|-----------------:|
| PWM Outputs | 4 | 64.1 mW | 256.4 mW |
| Digital Outputs | 4 | 48.0 mW | 192.0 mW |
| **Total LPS Power Dissipation** | **8** | — | **448.4 mW** |

> **Note:** The calculations assume worst-case operating conditions. All four PWM outputs are assumed to operate continuously at 40 kHz while carrying the maximum continuous load current of 1 A. The remaining four digital outputs are assumed to conduct continuously at the same load current without switching losses.

### 4.2 Medium Power Section

### 4.3 High Power Section

### 4.4 Current Sense Shunts

### 4.5 Other Components

## 5. Thermal Summary

## 6. Design Conclusions