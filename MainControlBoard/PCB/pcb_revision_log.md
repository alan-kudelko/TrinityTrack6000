# PCB Revision Log

This document tracks the evolution of the PCB hardware throughout the development process.

---

# RevA — Initial Prototyp

> 🟡 **Status:** Prototype • **Current Revision**

## Objectives

- Validate the first PCB revision.
- Verify MCU power-up sequence.
- Validate communication interfaces.
- Bring up the firmware on custom hardware.
- Verify RF and GNSS sections.

---

## Requirements

### Mechanical

- PCB dimensions: 100 × 100 mm
- 4-layer PCB stackup
- Standard SMD assembly

### Electrical

- 3.3 V logic domain
- Continuous GND plane
- Dedicated analog and digital filtering
- Local decoupling for all ICs

### RF

- External SMA connector
- 50 Ω impedance-controlled RF traces
- Tunable matching network

### Communication

- SPI
- I²C
- UART
- SPI stack connector

### Debug

- Dedicated debug connectors for both MCUs
- Test points
- Configuration jumpers


## Design Rationale

### System Architecture

- Dual-MCU architecture
- STM32G4 performs system supervision
- XMC4200 executes deterministic real-time control

### PCB Layout

- Functional block separation
- Continuous ground plane
- Short routing of critical signals
- Modular board architecture

### Reliability

- External watchdog
- Multiple reset sources
- Hardware fault detection


## Known Issues

- RF section is partially powered through pull-up resistors instead of the dedicated `NRF_VCC` rail.
- XMC4200 should be moved to a dedicated Power Electronics Control Board to improve system modularity and simplify PCB routing.
- Insufficient test points reduce debugging and bring-up efficiency.
- Limited via stitching in GND areas may negatively affect EMI performance and return current paths.
- Missing TVS protection on selected external interfaces.
- Limited trace capability for STM32 debugging and runtime analysis.


## Lessons Learned

- Increase GND via stitching, especially around RF and high-speed signal areas.
- Add more test points for power rails, SPI, UART, I²C and debugging.
- Use thermal reliefs on large copper pours connected to THT components to improve solderability.
- Reduce the number of THT components where practical to simplify manual assembly and rework.
- Keep RF power completely isolated using the dedicated `NRF_VCC` supply rail.
- Separate real-time motor control hardware (XMC4200) from the system supervisor PCB to improve modularity and reduce routing complexity.
- Reserve PCB space for ETM/trace debugging support on the STM32.
- Add TVS diodes on external connectors to improve ESD robustness.

---


# RevB

> 🔄 **Status:** Planned

## Objectives

...

## Changes from RevA

...

## Validation Results

...

## Known Issues

...

## Lessons Learned

...

## Engineering Change Requests (RevC)

...

---
