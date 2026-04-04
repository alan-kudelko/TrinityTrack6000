# 🧪 Development Log

---

## 🟢 STM32G4

### [001] Programming + UART (STM32)

> ✅ **Status: OK**

**Notes:**
- Programming works after lowering ST-LINK frequency
- Debugging is slow (likely SWD-related)
- Pin configuration conflicts (SPI / UART / I2C)

---

### [016] ThreadX bring-up

> ✅ **Status: OK**

**Notes:**
- No major issues during setup
- Behavior similar to FreeRTOS

---

### [018] CORDIC benchmark

> 🔄 **Status: WIP**

---

### [019] DMA SPI (interrupt-driven queue)

> ✅ **Status: OK**

**Notes:**
- Works for SPI1 (variable frame length)
- SPI2 requires a different approach

---

### [010-2] DMA SPI testing

> ✅ **Status: OK**

**Notes:**
- Successfully validated for SPI1

---

### [012-2] UART DMA (MemInfo)

> ✅ **Status: OK**

**Notes:**
- API updated to support timeout and retransmission (HAL_Delay / tx_thread_sleep)

---

### [014-2] ThreadX clock configuration

> ✅ **Status: OK**

**Notes:**
- HAL uses TIM6 instead of SysTick

---

### [018-2] ADC + DMA

> ✅ **Status: OK**

**Notes:**
- DMA_HT interrupt should be disabled
- VREF+ must be connected
- Conversion time depends on input impedance

---

### [021] Linker update (CCSRAM)

> ✅ **Status: OK**

**Notes:**
- CCSRAM dedicated to CPU
- DMA buffers separated
- No AHB arbitration conflicts

---

### [022] RAM sections / DMA placement

> 🔄 **Status: WIP**

---

### [024] ISR safety / NVIC priorities

> 🔄 **Status: WIP**

**Notes:**
- Partially improved
- Impacts DMA balancing

---

### [025] Initialization without UART

> 🔄 **Status: WIP**

---

### [026] Default pin states

> 🔄 **Status: WIP**

---

### [027] EXTI optimization (radio IRQ → LL)

> 🔄 **Status: WIP**

---

### [028] Radio IRQ callback

> ✅ **Status: OK**

---

### [029] IRQ latency measurement

> 🔄 **Status: WIP**

---

### [030] Wireless task queue

> 🔄 **Status: WIP**

---

### [031] Health Monitor task

> 🔄 **Status: WIP**

---

### [032] Radio abstraction (API)

> 🔄 **Status: WIP**

---

### [033] Migration G473CET6 → G473RET6

> ✅ **Status: OK**

**Notes:**
- Pin mapping updated

---

---

## 🔵 XMC4200

### [002] Programming + UART

> ✅ **Status: OK**

**Notes:**
- No issues during programming
- Important: verify VDDC connections

---

### [010] PWM generation (CCU4 / CCU8)

> 🔄 **Status: WIP**

---

### [011] I2C (ADXL345)

> 🔄 **Status: WIP**

---

### [015] Encoder (pulse counter mode)

> 🔄 **Status: WIP**

---

### [017] Micrium µC/OS evaluation

> 🔄 **Status: WIP**

**Notes:**
- Considering bare-metal event-driven approach instead

---

### [020] SPI → nRF24L01 (5 MHz)

> ✅ **Status: OK**

**Notes:**
- Issue caused by missing CSN connection

---

---

## 🟡 System / Hardware

### [003] MQ sensors – burn-in

> ✅ **Status: OK**

---

### [004] MQ sensors – calibration

> ✅ **Status: OK**

---

### [005] SPI STM32 ↔ XMC4200

> ❌ **Status: ISSUE**

**Notes:**
- First byte corrupted
- Remaining data correct

---

### [006] GPS (L76L)

> 🔄 **Status: WIP**

---

### [007] I2C (ADXL345, STM32)

> 🔄 **Status: WIP**

---

### [008] SPI FRAM

> 🔄 **Status: WIP**

---

### [009] SPI FRAM (XMC via STM32)

> 🔄 **Status: WIP**

---

### [012] MQ-7 heater current verification

> ✅ **Status: OK**

**Notes:**
- Measured current lower than datasheet

---

### [013] Buzzer diagnostics

> ✅ **Status: OK**

**Notes:**
- TIM2 used
- Adjustable frequency

---

### [020-2] RF communication (nRF24L01 ↔ nRF24L01)

> ✅ **Status: OK**

**Notes:**
- CONFIG must be initialized with PWR_UP first

---

### [034] SPI STM32 ↔ XMC4200 (hardware validation)

> ✅ **Status: OK**

**Notes:**
- PCB connections verified as correct