/**
 * @file NRF24L01.h
 * @brief Dedicated class for NRF24L01 module
 * 
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */

#ifndef NRF24L01_H_
    #define NRF24L01_H_

#ifdef __DOXYGEN__
    #define SECTION(x)
    #define ALIGNED(x)
#else
    #define SECTION(x) __attribute((section(x)))
    #define ALIGNED(x) __attribute((aligned(x)))
#endif // __DOXYGEN__ 

#include <Device_Def.h>
#include <SPI1_Dma.h>

/**
 * @defgroup NRF24L01_Registers Registers
 * @{
 */

#define NRF_REG_CONFIG        0x00
#define NRF_REG_EN_AA         0x01
#define NRF_REG_EN_RXADDR     0x02
#define NRF_REG_SETUP_AW      0x03
#define NRF_REG_SETUP_RETR    0x04
#define NRF_REG_RF_CH         0x05
#define NRF_REG_RF_SETUP      0x06
#define NRF_REG_STATUS        0x07
#define NRF_REG_OBSERVE_TX    0x08
#define NRF_REG_CD            0x09

#define NRF_REG_RX_ADDR_P0    0x0A
#define NRF_REG_RX_ADDR_P1    0x0B
#define NRF_REG_RX_ADDR_P2    0x0C
#define NRF_REG_RX_ADDR_P3    0x0D
#define NRF_REG_RX_ADDR_P4    0x0E
#define NRF_REG_RX_ADDR_P5    0x0F

#define NRF_REG_TX_ADDR       0x10
#define NRF_REG_RX_PW_P0      0x11
#define NRF_REG_RX_PW_P1      0x12
#define NRF_REG_RX_PW_P2      0x13
#define NRF_REG_RX_PW_P3      0x14
#define NRF_REG_RX_PW_P4      0x15
#define NRF_REG_RX_PW_P5      0x16

#define NRF_REG_FIFO_STATUS   0x17
#define NRF_REG_DYNPD         0x1C
#define NRF_REG_FEATURE       0x1D

/** @} */


/**
 * @defgroup NRF24L01_Commands Commands
 * @{
 */

#define NRF_CMD_R_REGISTER         0x00
#define NRF_CMD_W_REGISTER         0x20
#define NRF_CMD_R_RX_PAYLOAD       0x61
#define NRF_CMD_W_TX_PAYLOAD       0xA0
#define NRF_CMD_FLUSH_TX           0xE1
#define NRF_CMD_FLUSH_RX           0xE2
#define NRF_CMD_REUSE_TX_PL        0xE3
#define NRF_CMD_ACTIVATE           0x50
#define NRF_CMD_R_RX_PL_WID        0x60
#define NRF_CMD_W_ACK_PAYLOAD      0xA8
#define NRF_CMD_W_TX_PAYLOAD_NOACK 0xB0
#define NRF_CMD_NOP                0xFF

/** @} */

/**
 * @defgroup NRF24L01_BitMasks Bit Masks
 * @{
 */

#define NRF_BIT_MASK_RX_DR         (1<<6)
#define NRF_BIT_MASK_TX_DS         (1<<5)
#define NRF_BIT_MASK_MAX_RT        (1<<4)
#define NRF_BIT_EN_CRC             (1<<3)
#define NRF_BIT_CRCO               (1<<2)
#define NRF_BIT_PWR_UP             (1<<1)
#define NRF_BIT_PRIM_RX            (1<<0)

#define NRF_BIT_ENAA_P5            (1<<5)
#define NRF_BIT_ENAA_P4            (1<<4)
#define NRF_BIT_ENAA_P3            (1<<3)
#define NRF_BIT_ENAA_P2            (1<<2)
#define NRF_BIT_ENAA_P1            (1<<1)
#define NRF_BIT_ENAA_P0            (1<<0)

#define NRF_BIT_ERX_P5             (1<<5)
#define NRF_BIT_ERX_P4             (1<<4)
#define NRF_BIT_ERX_P3             (1<<3)
#define NRF_BIT_ERX_P2             (1<<2)
#define NRF_BIT_ERX_P1             (1<<1)
#define NRF_BIT_ERX_P0             (1<<0)

#define NRF_BIT_AW1                (1<<1)
#define NRF_BIT_AW0                (1<<0)

#define NRF_BIT_ARD7               (1<<7)
#define NRF_BIT_ARD6               (1<<6)
#define NRF_BIT_ARD5               (1<<5)
#define NRF_BIT_ARD4               (1<<4)

#define NRF_BIT_ARC3               (1<<3)
#define NRF_BIT_ARC2               (1<<2)
#define NRF_BIT_ARC1               (1<<1)
#define NRF_BIT_ARC0               (1<<0)

#define NRF_BIT_RF_CH6             (1<<6)
#define NRF_BIT_RF_CH5             (1<<5)
#define NRF_BIT_RF_CH4             (1<<4)
#define NRF_BIT_RF_CH3             (1<<3)
#define NRF_BIT_RF_CH2             (1<<2)
#define NRF_BIT_RF_CH1             (1<<1)
#define NRF_BIT_RF_CH0             (1<<0)

#define NRF_BIT_PLL_LOCK           (1<<4)
#define NRF_BIT_RF_DR              (1<<3)
#define NRF_BIT_RF_PWR2            (1<<2)
#define NRF_BIT_RF_PWR1            (1<<1)
#define NRF_BIT_RF_LNA_HCURR       (1<<0)

#define NRF_BIT_RX_DR              (1<<6)
#define NRF_BIT_TX_DS              (1<<5)
#define NRF_BIT_MAX_RT             (1<<4)
#define NRF_BIT_P_NO3              (1<<3)
#define NRF_BIT_P_NO2              (1<<2)
#define NRF_BIT_P_NO1              (1<<1)
#define NRF_BIT_TX_FULL            (1<<0)

#define NRF_BIT_PLOS_CNT7          (1<<7)
#define NRF_BIT_PLOS_CNT6          (1<<6)
#define NRF_BIT_PLOS_CNT5          (1<<5)
#define NRF_BIT_PLOS_CNT4          (1<<4)
#define NRF_BIT_ARC_CNT3           (1<<3)
#define NRF_BIT_ARC_CNT2           (1<<2)
#define NRF_BIT_ARC_CNT1           (1<<1)
#define NRF_BIT_ARC_CNT0           (1<<0)

#define NRF_BIT_CD                 (1<<0)
// RX_ADDR_P0-P5 39:0

#define NRF_BIT_TX_REUSE           (1<<6)
#define NRF_BIT_RX_FULL            (1<<1)
#define NRF_BIT_RX_EMPTY           (1<<0)

#define NRF_BIT_DPL_P5             (1<<5)
#define NRF_BIT_DPL_P4             (1<<4)
#define NRF_BIT_DPL_P3             (1<<3)
#define NRF_BIT_DPL_P2             (1<<2)
#define NRF_BIT_DPL_P1             (1<<1)
#define NRF_BIT_DPL_P0             (1<<0)

#define NRF_BIT_EN_DPL             (1<<2)
#define NRF_BIT_EN_ACK_PAY         (1<<1)
#define NRF_BIT_EN_DYN_ACK         (1<<0)

/** @} */


class NRF24L01{
    DEVICE_IO _cePin;  // Send/receive data pin (depending on mode)
    DEVICE_IO _csnPin; // Chip select pin for SPI interface
    DEVICE_IO _irqPin; // Interrupt pin

    hspi_data _transaction_data;
    void initialize_hspi_data();
public:
    NRF24L01(DEVICE_IO ce={0},DEVICE_IO csn={0}, DEVICE_IO irq={0});
    NRF24L01(GPIO_TypeDef*cePort,uint16_t cePin,GPIO_TypeDef*csnPort,uint16_t csnPin,GPIO_TypeDef*irqPort,uint16_t irqPin);

    ~NRF24L01();

    void attach_tx_buffer(uint8_t*txBuffer,uint8_t txBufferLength);
    void attach_rx_buffer(uint8_t*rxBuffer,uint8_t rxBufferLength);

    void attach_callback_function(void(*callbackFn)(void));

    bool send_data();
// Registers operation
// 0x00 CONFIG R/W
    bool write_reg_config(uint8_t flags);
    bool read_reg_config();
// 0x01 EN_AA R/W
    bool write_reg_en_aa(uint8_t flags);
    bool read_reg_en_aa();
// 0x02 EN_RXADDR R/W
    bool write_reg_en_rxAddr(uint8_t flags);
    bool read_reg_en_rxAddr();
// 0x03 SETUP_AW R/W
    bool write_reg_setup_aw(uint8_t flags);
    bool read_reg_setup_aw();
// 0x04 SETUP_RETR R/W
    bool write_reg_setup_retr(uint8_t flags);
    bool read_reg_setup_retr();
// 0x05 RF_CH R/W
    bool write_reg_rf_ch(uint8_t flags);
    bool read_reg_rf_ch();
// 0x06 RF_SETUP R/W
    bool write_reg_rf_setup(uint8_t flags);
    bool read_reg_rf_setup();
// 0x07 STATUS R/W
    bool write_reg_status(uint8_t flags);
    bool read_reg_status();
// 0x08 OBSERVE_TX R
    bool read_reg_observe_tx();
// 0x09 CD R
    bool read_reg_cd();


// 0x17 FIFO_STATUS R/W
    bool write_reg_fifo_status(uint8_t flags);
    bool read_reg_fifo_status();
// 0x1C DYNPD R/W
    bool write_reg_dynpd(uint8_t flags);
    bool read_reg_dynpd();
// 0x1D FEATURE R/W
    bool write_reg_feature(uint8_t flags);
    bool read_reg_feature();

    // Function for getting information from NRF (parsing values from raw registers)
};

// Test functions

#ifdef __cplusplus
    extern "C"{
#endif // __cplusplus

void nrf24l01_test(void);

#ifdef __cplusplus
    }
#endif // __cplusplus

// Dobra to co jest istotne:
// Port i pin CE CSN IRQ?
// Jakiś callback gdy będzie IRQ
// Na pewno wewnętrzne lub zewnętrzne bufory do obsługi DMA i interfejs do udostępniania wskaźników na te bufory
// Struktura przechowująca piny i porty. Też tak myślę, że ta struktura powinna być uniwersalna dla różnych urządzeń
// Podobnie będzie np. z klasą FRAM'u oraz potencjalnie innymi urzązeniami nie tylko SPI ale też UART czy I2C
// No oczywiscie rejestry i opis bitow
// Funkcje do konfiguracji i obslugi z abstrakcja rejestrow i bitow
// Bedzie to wykorzystywac na pewno moj interfejs do SPI1 DMA
// wskaźnik na funkcję timeout kolejkowania dla mojego SPI
// sygnatura zgodna z tx_thread_sleep
// 


#endif // NRF24L01_H_