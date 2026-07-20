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

### 4.3 High Power Section

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
| Winch Rated Current | I | 15 A |
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
Pshunt = 0.85 × (15 A)² × 5 mΩ
Pshunt = 0.956 W
```

##### Total Current Sense Shunt Power Dissipation

| Current Sense Channel | Quantity | Power Loss per Shunt | Total Power Loss |
|-----------------------|---------:|---------------------:|-----------------:|
| HPS Shunts (1 mΩ) | 2 | 1.36 W | 2.72 W |
| Winch Shunt (5 mΩ) | 1 | 0.956 W | 0.956 W |
| **Total Shunt Power Dissipation** | **3** | — | **3.676 W** |

> **Note:** The calculations assume a maximum PWM duty cycle of **85%**, representing the worst-case continuous operating condition of the H-bridge power stages.


### 4.5 Other Components

## 5. Thermal Summary

This section summarizes the estimated power dissipation of all major heat-generating subsystems implemented on the `HardwareControlBoard`.

The objective is to establish the overall PCB thermal budget and verify that the selected PCB technology, copper area, and cooling method are capable of dissipating the generated heat under worst-case operating conditions.

Unless stated otherwise, all values represent **worst-case theoretical estimates** based on conservative first-order calculations and maximum datasheet parameters.

| Section | Worst-Case Power Dissipation | Notes |
|---------|-----------------------------:|-------|
| Low Power Section (LPS) | 0.448 W | MOSFET conduction and switching losses |
| Medium Power Section (MPS) | TBD | |
| High Power Section (HPS) | TBD | |
| Current Sense Shunts | 3.676 W | Shunt conduction losses |
| Other Components | TBD | Gate drivers, analog front-end, logic ICs, etc. |
| **Total PCB Thermal Budget** | **TBD** | Worst-case estimate |

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

The thermal evaluation presented in this document is currently incomplete.

Additional analysis is required for the remaining power stages and supporting circuitry before the final PCB thermal budget can be established.

The following items remain to be evaluated:

- Medium Power Section (MPS),
- High Power Section (HPS),
- Gate driver power dissipation,
- Analog front-end and measurement circuitry,
- Auxiliary power supplies,
- Total PCB thermal budget,
- Prototype thermal measurements and validation.

The conclusions of this document will be updated after completing the remaining analytical calculations and validating the design using hardware measurements.