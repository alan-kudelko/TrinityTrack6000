# 🧪 Development Log

---

## 🟢 STM32G4

### [001] Programowanie + UART

> ✅ **Status: OK**

**Obserwacje:**
- Programowanie działa po obniżeniu częstotliwości ST-LINK
- Debugowanie wolne (SWD?)
- Konflikty pinów (SPI / UART / I2C)

---

### [016] ThreadX

> ✅ **Status: OK**

**Obserwacje:**
- Kompiluje się bez problemów
- Podobne do FreeRTOS

---

### [018] CORDIC benchmark

> 🔄 **Status: WIP**

---

### [019] DMA SPI (interrupt-driven queue)

> ✅ **Status: OK**

**Obserwacje:**
- Działa dla SPI1 (zmienna długość ramki)
- SPI2 wymaga innego podejścia

---

### [010-2] DMA SPI testy (STM32)

> ✅ **Status: OK**

**Obserwacje:**
- Testy zakończone sukcesem dla SPI1

---

### [012-2] UART DMA (MemInfo)

> ✅ **Status: OK**

**Obserwacje:**
- Zmienione API (timeout, retry – HAL_Delay / tx_thread_sleep)

---

### [014-2] ThreadX (clock config)

> ✅ **Status: OK**

**Obserwacje:**
- HAL korzysta z TIM6 zamiast SysTick

---

### [018-2] ADC + DMA

> ✅ **Status: OK**

**Obserwacje:**
- DMA_HT_IT do wywalenia
- VREF+ musi być podłączone
- Czas konwersji zależny od impedancji

---

### [021] Linker – CCSRAM

> ✅ **Status: OK**

**Obserwacje:**
- CCSRAM tylko dla CPU
- Bufory DMA rozdzielone
- Brak konfliktów AHB

---

### [022] RAM sections / DMA

> 🔄 **Status: WIP**

---

### [024] ISR safety / NVIC

> 🔄 **Status: WIP**

**Obserwacje:**
- Częściowo poprawione
- Wpływ na DMA balancing

---

### [025] Init bez UART

> 🔄 **Status: WIP**

---

### [026] Default pin states

> 🔄 **Status: WIP**

---

### [027] EXTI → LL (radio IRQ)

> 🔄 **Status: WIP**

---

### [028] Radio IRQ callback

> ✅ **Status: OK**

---

### [029] IRQ timing measurement

> 🔄 **Status: WIP**

---

### [030] Wireless task queue

> 🔄 **Status: WIP**

---

### [031] Health Monitor task

> 🔄 **Status: WIP**

---

### [032] Radio API

> 🔄 **Status: WIP**

---

### [033] Migracja G473CET6 → G473RET6

> ✅ **Status: OK**

**Obserwacje:**
- Zmienione mapowanie pinów

---

---

## 🔵 XMC4200

### [002] Programowanie + UART

> ✅ **Status: OK**

**Obserwacje:**
- Brak problemów
- Uwaga na VDDC

---

### [010] PWM (CCU4 / CCU8)

> 🔄 **Status: WIP**

---

### [011] I2C (ADXL345)

> 🔄 **Status: WIP**

---

### [015] Encoder (pulse counter)

> 🔄 **Status: WIP**

---

### [017] Micrium µC/OS

> 🔄 **Status: WIP**

**Obserwacje:**
- Rozważenie przejścia na bare-metal (event-driven)

---

### [020] SPI → nRF24L01 (5 MHz)

> ✅ **Status: OK**

**Obserwacje:**
- Problem z CSN (brak podłączenia)

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

**Obserwacje:**
- Pierwszy bajt niepoprawny
- Reszta transmisji OK

---

### [006] GPS (L76L)

> 🔄 **Status: WIP**

---

### [007] I2C ADXL345 (STM32)

> 🔄 **Status: WIP**

---

### [008] SPI FRAM

> 🔄 **Status: WIP**

---

### [009] SPI FRAM (XMC przez STM32)

> 🔄 **Status: WIP**

---

### [012] MQ-7 heater current

> ✅ **Status: OK**

**Obserwacje:**
- Pobór prądu niższy niż datasheet

---

### [013] Buzzer

> ✅ **Status: OK**

**Obserwacje:**
- Timer2, regulowana częstotliwość

---

### [020-2] RF communication (nRF24L01 ↔ nRF24L01)

> ✅ **Status: OK**

**Obserwacje:**
- Najpierw PWR_UP w CONFIG, potem reszta

---

### [034] SPI STM32 ↔ XMC4200 (hardware check)

> ✅ **Status: OK**

**Obserwacje:**
- PCB poprawne