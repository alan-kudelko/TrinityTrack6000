# PCB Revision Log

This document tracks the evolution of the HardwareControlBoard throughout the development process.

---

# RevA — Initial Prototype

> 🟡 **Status:** Requirements Definition • **Current Revision**

## Objectives

- Validate the first HardwareControlBoard revision.
- Verify MCU power-up sequence.
- Validate communication with the MainControlBoard.
- Validate motor and servo interfaces.
- Bring up the bare-metal firmware on custom hardware.
- Verify power electronics interfaces.

---

## Requirements

### Mechanical

- PCB dimensions: 100 × 100/150/200 mm
- 4-layer PCB stackup
- Standard SMD assembly
- High-current routing support

### Electrical

- 3.3 V logic domain
- Continuous GND plane
- Dedicated analog and digital filtering
- Local decoupling for all ICs
- Hardware fault inputs
- Hardware ENABLE input
- External watchdog support

### Motion Control

- DC motor interfaces
- Servo outputs
- Future stepper motor support
- Incremental encoder inputs
- Current sensing
- Temperature monitoring

### Communication

- SPI interface to MainControlBoard
- I²C diagnostics interface
- UART debug interface

### Debug

- Cortex-M Debug connector
- Full ETM / Trace support
- Test points
- Diagnostic LEDs

---

## Design Rationale

### System Architecture

- Dedicated low-level motion controller
- Bare-metal firmware
- Hardware-assisted peripherals (DMA, timers, interrupts)
- Separation between high-level coordination and deterministic control

### PCB Layout

- Functional block separation
- Continuous ground plane
- Short routing of timing-critical signals
- Separation between logic and power electronics
- Modular board architecture

### Reliability

- Hardware fault detection
- Hardware ENABLE / KILL support
- Deterministic execution

---

## Known Issues


---

## Lessons Learned



---

# RevB

> 🔄 **Status:** Planned

To be defined after validation of RevA.
