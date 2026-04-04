/**
 * @file MCP23S17.h
 * @brief Driver for the MCP23S17 16-bit SPI I/O Expander
 * This module provides a C++ class interface for controlling the MCP23S17 device.
 * It allows configuration of GPIO direction, pull-ups, polarity inversion, and interrupt settings.
 * 
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.
 * Permission is hereby granted, free of charge, to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of this software,
 * subject to inclusion of this copyright notice in all copies or substantial portions of the software.
 */

#ifndef MCP23S17_H_
    #define MCP23S17_H_

#include <stm32g4xx_hal.h>

/** @name MCP23S17 Bank 0 Register Addresses
 *  @{ */
#define MCP23S17_IODIRA    0x00 /**< I/O direction register for Port A (1=input, 0=output) */
#define MCP23S17_IODIRB    0x01 /**< I/O direction register for Port B (1=input, 0=output) */
#define MCP23S17_IPOLA     0x02 /**< Input polarity register for Port A */
#define MCP23S17_IPOLB     0x03 /**< Input polarity register for Port B */
#define MCP23S17_GPINTENA  0x04 /**< Interrupt-on-change enable for Port A */
#define MCP23S17_GPINTENB  0x05 /**< Interrupt-on-change enable for Port B */
#define MCP23S17_DEFVALA   0x06 /**< Default comparison value for interrupt on Port A */
#define MCP23S17_DEFVALB   0x07 /**< Default comparison value for interrupt on Port B */
#define MCP23S17_INTCONA   0x08 /**< Interrupt control register for Port A */
#define MCP23S17_INTCONB   0x09 /**< Interrupt control register for Port B */
#define MCP23S17_IOCON     0x0A /**< I/O Expander configuration register */
#define MCP23S17_GPPUA     0x0C /**< Pull-up resistor configuration for Port A */
#define MCP23S17_GPPUB     0x0D /**< Pull-up resistor configuration for Port B */
#define MCP23S17_INTFA     0x0E /**< Interrupt flag register for Port A */
#define MCP23S17_INTFB     0x0F /**< Interrupt flag register for Port B */
#define MCP23S17_INTCAPA   0x10 /**< Interrupt captured value for Port A */
#define MCP23S17_INTCAPB   0x11 /**< Interrupt captured value for Port B */
#define MCP23S17_GPIOA     0x12 /**< GPIO register for Port A */
#define MCP23S17_GPIOB     0x13 /**< GPIO register for Port B */
#define MCP23S17_OLATA     0x14 /**< Output latch register for Port A */
#define MCP23S17_OLATB     0x15 /**< Output latch register for Port B */
/** @} */

/** @name MCP23S17 Register Bit Definitions (Bank 0) @{ */

/** @brief IODIR - I/O Direction Register */
#define MCP23S17_IODIR_IO7 (1U << 7)
#define MCP23S17_IODIR_IO6 (1U << 6)
#define MCP23S17_IODIR_IO5 (1U << 5)
#define MCP23S17_IODIR_IO4 (1U << 4)
#define MCP23S17_IODIR_IO3 (1U << 3)
#define MCP23S17_IODIR_IO2 (1U << 2)
#define MCP23S17_IODIR_IO1 (1U << 1)
#define MCP23S17_IODIR_IO0 (1U << 0)

/** @brief IPOL - Input Polarity */
#define MCP23S17_IPOL_IP7 (1U << 7)
#define MCP23S17_IPOL_IP6 (1U << 6)
#define MCP23S17_IPOL_IP5 (1U << 5)
#define MCP23S17_IPOL_IP4 (1U << 4)
#define MCP23S17_IPOL_IP3 (1U << 3)
#define MCP23S17_IPOL_IP2 (1U << 2)
#define MCP23S17_IPOL_IP1 (1U << 1)
#define MCP23S17_IPOL_IP0 (1U << 0)

/** @brief GPINTEN - Interrupt-on-change enable */
#define MCP23S17_GPINTEN_GPINT7 (1U << 7)
#define MCP23S17_GPINTEN_GPINT6 (1U << 6)
#define MCP23S17_GPINTEN_GPINT5 (1U << 5)
#define MCP23S17_GPINTEN_GPINT4 (1U << 4)
#define MCP23S17_GPINTEN_GPINT3 (1U << 3)
#define MCP23S17_GPINTEN_GPINT2 (1U << 2)
#define MCP23S17_GPINTEN_GPINT1 (1U << 1)
#define MCP23S17_GPINTEN_GPINT0 (1U << 0)

/** @brief DEFVAL - Default Compare Register for Interrupt */
#define MCP23S17_DEFVAL_DEF7 (1U << 7)
#define MCP23S17_DEFVAL_DEF6 (1U << 6)
#define MCP23S17_DEFVAL_DEF5 (1U << 5)
#define MCP23S17_DEFVAL_DEF4 (1U << 4)
#define MCP23S17_DEFVAL_DEF3 (1U << 3)
#define MCP23S17_DEFVAL_DEF2 (1U << 2)
#define MCP23S17_DEFVAL_DEF1 (1U << 1)
#define MCP23S17_DEFVAL_DEF0 (1U << 0)

/** @brief INTCON - Interrupt Control Register */
#define MCP23S17_INTCON_INT7 (1U << 7)
#define MCP23S17_INTCON_INT6 (1U << 6)
#define MCP23S17_INTCON_INT5 (1U << 5)
#define MCP23S17_INTCON_INT4 (1U << 4)
#define MCP23S17_INTCON_INT3 (1U << 3)
#define MCP23S17_INTCON_INT2 (1U << 2)
#define MCP23S17_INTCON_INT1 (1U << 1)
#define MCP23S17_INTCON_INT0 (1U << 0)

/** @brief GPPU - Pull-up Resistor Configuration */
#define MCP23S17_GPPU_PU7 (1U << 7)
#define MCP23S17_GPPU_PU6 (1U << 6)
#define MCP23S17_GPPU_PU5 (1U << 5)
#define MCP23S17_GPPU_PU4 (1U << 4)
#define MCP23S17_GPPU_PU3 (1U << 3)
#define MCP23S17_GPPU_PU2 (1U << 2)
#define MCP23S17_GPPU_PU1 (1U << 1)
#define MCP23S17_GPPU_PU0 (1U << 0)

/** @} */

/** @name MCP23S17 IOCON Register Bits @{ */
#define MCP23S17_IOCON_BANK    (1U << 7) /**< Register addressing mode (0=separate A/B, 1=sequential banked) */
#define MCP23S17_IOCON_MIRROR  (1U << 6) /**< INT pins mirrored */
#define MCP23S17_IOCON_SEQOP   (1U << 5) /**< Sequential operation disabled */
#define MCP23S17_IOCON_DISSLW  (1U << 4) /**< Slew rate disabled */
#define MCP23S17_IOCON_HAEN    (1U << 3) /**< Hardware address enable (for SPI) */
#define MCP23S17_IOCON_ODR     (1U << 2) /**< Open-drain INT output */
#define MCP23S17_IOCON_INTPOL  (1U << 1) /**< INT output polarity */
#define MCP23S17_IOCON_UNUSED  (1U << 0) /**< Reserved / not used */
/** @} */

class MCP23S17{
    SPI_HandleTypeDef*_hspi;
    GPIO_TypeDef*_cs_port;
    uint16_t _cs_pin;
    GPIO_TypeDef*_rst_port;
    uint16_t _rst_pin;

    uint8_t*_tx_buffer;
    uint8_t*_rx_buffer;

    uint8_t _tx_buffer_size;
    uint8_t _rx_buffer_size;
    public:
    MCP23S17(SPI_HandleTypeDef*hspi,GPIO_TypeDef*cs_port,uint16_t cs_pin,GPIO_TypeDef*rst_port,uint16_t rst_pin);
    ~MCP23S17();

    void init(); // Reset MCU to ensure default IOBANK and correct writting operations

    HAL_StatusTypeDef write(uint8_t reg,uint8_t value);
    HAL_StatusTypeDef read(uint8_t reg,uint8_t*value);
};

#endif // MCP23S17_H_
