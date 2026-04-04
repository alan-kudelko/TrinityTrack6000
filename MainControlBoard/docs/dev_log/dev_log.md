# 🧪 Development Log

## ⚠️ Notes

This file contains raw development notes, observations, and decisions made during the project.

Content may be informal, incomplete, or experimental by nature.



## 🟢 STM32G4

### [001] CubeIDE project setup (initial HAL)

> ✅ **Status: OK**

### [002] CMake integration

> ✅ **Status: OK**

### [003] Programming + UART

> ✅ **Status: OK**

**Notes:**
- Programming works after lowering ST-LINK frequency
- Debugging is slow (likely SWD-related)


### [004] ThreadX bring-up

> ✅ **Status: OK**

**Notes:**
- No major issues during setup
- Builds successfully
- Similar to FreeRTOS
- HAL uses TIM6 instead of SysTick (clock configuration change)
- STM32Cube automatically generates MCU-specific ThreadX port
- Using ThreadX integration from STM32Cube (not the official ThreadX repository)

### [005] Buzzer diagnostics (TIM + BJT)

> ✅ **Status: OK**

### [006] SPI communication (nRF24L01, 5 MHz)

> ✅ **Status: OK**

**Notes:**
- Initial issue caused by missing CSN connection

### [007] DMA SPI (interrupt-driven queue)

> ✅ **Status: OK**

**Notes:**
- Queue-based DMA transfer (tx/rx buffer, size, CS port/pin)
- Supports half-duplex and full-duplex modes
- Implemented using transmission flags
- Callback mechanism integrated (device-specific handling)
- Works well for SPI1 (variable frame length)
- SPI2 requires a different approach, but overall architecture remains reusable

### [008] SPI communication (FM25L16B-GTR)

> 🔄 **Status: WIP**

### [009] DMA UART (interrupt-driven queue)

> ✅ **Status: OK**

### [010] GPS communication (L76L-M33)

> 🔄 **Status: WIP**

### [011] DMA I2C (interrupt-driven queue)

> 🔄 **Status: WIP**

### [012] I2C communication (ADXL345)

> 🔄 **Status: WIP**

### [013] ADC + DMA

> ❌ **Status: ISSUE**

**Notes:**
- DMA_HT interrupt should be disabled
- VREF+ must be connected
- Conversion time depends on input impedance
- Issue with double callback still present (requires further investigation)

### [014] nRF24L01 driver

> ✅ **Status: OK**

**Notes:**
- Well-structured architecture
- Detailed description available in Doxygen and "Radio Communication" section

### [015] L76K-M33 GPS driver

> 🔄 **Status: WIP**

**Notes:**
- Architecture similar to nRF24L01 driver

### [016] FM25L16B-GTR driver

> 🔄 **Status: WIP**

**Notes:**
- Architecture similar to nRF24L01 driver

### [017] RAM section attributes (DMA buffers)

> 🔄 **Status: WIP**

**Notes:**
- DMA buffers for high-load operations placed in separate RAM bank
- Reduces contention and improves performance
- Non time-critical interfaces can share the same bank with lower priority (DMA arbitration)


### [018] Default pin states after reset

> 🔄 **Status: WIP**

**Notes:**
- Default pin states should be configured according to their intended function (safe state at startup)


### [019] Linker update (CCSRAM usage)

> ✅ **Status: OK**

**Notes:**
- Removed aliasing for CCSRAM section
- CCSRAM dedicated exclusively for CPU usage (I-Code Bus access, no AHB arbitration conflicts)
- DMA buffers relocated to separate RAM regions
- System stability verified after changes

### [020] Radio IRQ falling edge callback

> ✅ **Status: OK**

**Notes:**
- Falling edge interrupt callback implemented for radio IRQ pin
- Straightforward implementation

### [021] DMA load balancing

> 🔄 **Status: WIP**

**Notes:**
- DMA operations should be distributed across multiple controllers
- Goal: reduce jitter and avoid contention
- Requires analysis of current DMA channel usage and priorities

### [022] ADC1 initialization refactor

> 🔄 **Status: WIP**

**Notes:**
- Introduce helper function wrapping HAL initialization
- Disable `DMA_IT_HT` interrupt
- Ensure proper clearing of DMA flags
- Goal: stabilize ADC + DMA behavior and remove redundant interrupts

### [023] File dependencies cleanup (C / C++ integration)

> 🔄 **Status: WIP**

**Notes:**
- Refactor required due to extensive use of `extern "C"`
- Need to standardize include order between `.c` and `.cpp` files
- Goal: eliminate hidden dependencies and improve build consistency

### [024] ISR safety and interrupt priority alignment

> 🔄 **Status: WIP**

**Notes:**
- Driver callbacks (e.g. SPI, DMA) must be ISR-safe
- Avoid blocking operations and non-RTOS-safe API usage inside ISRs
- NVIC interrupt priorities must not violate ThreadX constraints
  (no ISR should preempt RTOS critical sections improperly)
- Partial refactor completed
- DMA load balancing introduced, requiring further interrupt priority tuning


### [025] Initial system architecture (dispatcher-based)

> 🔄 **Status: WIP**

**Notes:**
- System designed as a message-driven RTOS architecture
- Central **Dispatcher task** introduced as a routing layer

**Architecture overview:**
- `CLI task`
  - Used for diagnostics and manual control (debug mode)
- `Wireless task`
  - Provides incoming data frames and telemetry
  - Planned extension: health monitoring (heartbeat tracking)
- `Dispatcher task`
  - Core routing component
  - Receives requests via a queue (union-based message structure)
  - Does NOT perform direct hardware control
  - Delegates execution to dedicated modules (peripheral-specific handlers)

**System modes:**
- `RUN`
  - Dispatcher processes commands from wireless communication
  - Control delegated to external MCU (Infineon)
- `DEBUG`
  - CLI allowed to inject control commands via Dispatcher
- `FAULT`
  - Limited control, diagnostics enabled
- `FAILSAFE`
  - Restricted operation, safety-first behavior

**Design principles:**
- Clear separation of responsibilities (no direct control in Dispatcher)
- Message-based communication between tasks
- Hardware abstraction via specialized handlers/modules
- Mode-dependent behavior control at system level

**Notes:**
- Dispatcher queue uses union-based request structure with type field
- Architecture is scalable and allows adding new subsystems without modifying core logic

### [026] SystemDispatcher task implementation

> 🔄 **Status: WIP**

**Responsibilities:**
- Receive commands from multiple sources:
  - CLI task (debug/control)
  - Wireless task (NRF24L01 raw frames or modified)

- Route incoming requests based on:
  - System mode (`RUN`, `DEBUG`, `FAULT`, `FAILSAFE`)
  - Request type (union-based message structure)

- Act as a central decision layer:
  - In `RUN` mode → process wireless commands and forward control to external MCU (Infineon)
  - In `DEBUG` mode → allow CLI to inject control commands
  - In `FAULT / FAILSAFE` → restrict or override command execution

- Dispatch commands to dedicated handlers:
  - No direct hardware control inside Dispatcher
  - Forward requests to peripheral-specific modules/tasks

- Manage internal request queue:
  - Queue entries defined as union-based structures with type field
  - Support multiple request types (control, diagnostics, communication, etc.)

**Notes:**
- Dispatcher acts as a control-plane component, not execution layer
- Designed for scalability and clean separation of responsibilities

### [027] Wireless task implementation

> 🔄 **Status: WIP**

**Responsibilities:**
- Handle communication with NRF24L01 module (receiver role)
  - RX frame handling
  - SPI communication

- Provide incoming data to the system:
  - Forward received control frames to SystemDispatcher
  - Ensure basic validation and parsing

- Maintain link quality statistics:
  - Packet loss rate
  - Retransmissions / failed transmissions
  - Link quality estimation (based on ACK / retries)

- Implement frequency hopping:
  - Dynamic RF channel switching
  - Maintain synchronization with transmitter
  - Improve robustness against interference

- Collect telemetry data:
  - Gather system data from other tasks (e.g. diagnostics, sensors, status)
  - Prepare telemetry payload

- Transmit telemetry via ACK payload:
  - Use NRF24L01 ACK payload feature (receiver-side response)
  - No standalone TX mode required

- Monitor link health:
  - Detect degraded or lost connection
  - Track heartbeat / communication timeout

- Report link status:
  - Provide metrics to Health Monitor task
  - Enable failsafe decision logic

**Parameters:**
- Target communication rate: ~500 Hz (500 packets per second)

**Future extensions:**
- AES-128 encryption for payload protection

**Notes:**
- Wireless task does NOT implement failsafe logic
- Health Monitor decides about failsafe conditions
- Dispatcher executes mode transitions
- Clear separation of responsibilities:
  - Wireless → measurement + communication
  - Health Monitor → decision
  - Dispatcher → execution
  
### [028] CLI task implementation

> 🔄 **Status: WIP**

**Responsibilities:**
- Provide human-to-system interface over UART
  - Command-line based interaction
  - Structured command parsing

- Enable system diagnostics:
  - Read system status (tasks, memory, link, sensors, etc.)
  - Query internal state via Dispatcher

- Allow manual control (debug mode only):
  - Send control commands through SystemDispatcher
  - Inject requests into system in a controlled way

- Support raw communication:
  - Send raw frames to selected interfaces (SPI / I2C / UART via Dispatcher)
  - Read raw data from devices for debugging purposes

- Manage system mode transitions:
  - Enter / exit `DEBUG` mode
  - Restrict control access depending on current system mode

**Architecture constraints:**
- CLI communicates ONLY with SystemDispatcher
- No direct interaction with peripheral-specific modules
- All operations go through Dispatcher routing and validation

**Notes:**
- CLI is intended strictly for development and diagnostics
- Acts as a controlled entry point into the system
- Prevents accidental bypass of system logic and safety mechanisms

### [029] Health Monitor task implementation

> 🔄 **Status: WIP**

**Responsibilities:**
- Monitor system health:
  - Collect heartbeats from all critical tasks
  - Detect task stalls, timeouts, or unexpected behavior

- Evaluate system state:
  - Determine if system is operating normally, degraded, or critical
  - Classify failures (recoverable vs non-recoverable)

- Watchdog management:
  - Feed watchdog only if system is in a valid operational state
  - Stop feeding watchdog in case of critical failure (force system reset)

- Fault handling:
  - Attempt basic recovery actions (if applicable)
  - Request transition to `FAULT` mode via SystemDispatcher

- Radio link supervision:
  - Receive link quality metrics from Wireless task
  - Detect poor or lost connection

- Failsafe triggering:
  - Request transition to `FAILSAFE` mode via SystemDispatcher
  - Based on link degradation or loss of communication

**Architecture constraints:**
- Health Monitor does NOT change system mode directly
- All mode transitions are requested via SystemDispatcher

**Notes:**
- Acts as system supervisor (safety layer)
- Separates:
  - detection (Health Monitor)
  - decision enforcement (Dispatcher)
- Critical for system stability and fail-safe behavior

### [030] MCU migration (STM32G473 → STM32G474)

> 🔄 **Status: WIP**

**Notes:**
- Migration required due to STM32G473 availability issues
- STM32G474 used as a fully compatible replacement (pin-to-pin, same family)

**Changes performed:**
- Updated CMake configuration:
  - Target MCU changed to STM32G474
  - Updated compiler flags and linker references if required

- Reconfigured STM32CubeMX project:
  - Selected STM32G474RET6 as target device
  - Verified peripheral configuration and pin mapping

- Updated header files:
  - Replaced device-specific headers with STM32G474 equivalents
  - Ensured correct CMSIS and HAL includes

**Result:**
- No functional changes required
- Project builds
- I will be sure if the project runs as soon as I finish my PCB

---

## XMC4200

### [001] XMC4200 project setup (DAVE IDE)

> ✅ **Status: OK**

### [002] CMake integration

> ✅ **Status: OK**

### [003] Programming + UART

> ✅ **Status: OK**

**Notes:**
- Programming works without issues
- VDDC is core voltage (~1.3 V, internal domain)
- Must not be connected to main supply (3.3 V)

### [004] PWM configuration and testing (CCU4 / CCU8)

> 🔄 **Status: WIP**

### [005] Encoder (pulse counter mode)

> 🔄 **Status: WIP**

### [006] Micrium µC/OS evaluation

> 🔄 **Status: WIP**

**Notes:**
- Considering switching to bare-metal (interrupt-based, event-driven architecture)



---

## System/Hardware

### [001] RF communication (nRF24L01 ↔ nRF24L01)

> ✅ **Status: OK**

**Notes:**
- Successful bidirectional communication between modules
- CONFIG register must be initialized with PWR_UP first before other settings

### [002] Glycerin evaporator test

> ✅ **Status: OK**

**Notes:**
- Temperature regulation is critical
- High thermal inertia and large surface area required
- High glycerin density improves control stability (lower risk of overflow)

### [003] MQ-6, MQ-7 heaters current verification

> ✅ **Status: OK**

**Notes:**
- Measured current is lower than specified in the datasheet

### [004] MQ sensors – burn-in

> ✅ **Status: OK**

### [005] MQ sensors – calibration

> ✅ **Status: OK**

### [006] SPI connection verification (STM32 ↔ XMC4200)

> ✅ **Status: OK**

**Notes:**
- PCB schematic verified as correct
- Clarified SPI wiring (MISO ↔ MISO, MOSI ↔ MOSI)

### [007] SPI communication (STM32 ↔ XMC4200)

> ❌ **Status: ISSUE**

**Notes:**
- First byte corrupted
- Remaining data correct
- Likely software-related (frame handling / synchronization)

