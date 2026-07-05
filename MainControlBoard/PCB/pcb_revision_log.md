# PCB Revision Log

This document tracks the evolution of the PCB hardware throughout the development process.

---

# RevA — Initial Prototype

> 🟡 **Status:** Prototype

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

---

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

---

## Known Issues

- None

---

## Lessons Learned

- To be completed during hardware validation.

---

## Engineering Change Requests (RevB)

- None

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
