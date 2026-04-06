
/**
 * @cond
 ***********************************************************************************************************************
 *
 * Copyright (c) 2015-2020, Infineon Technologies AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,are permitted provided that the
 * following conditions are met:
 *
 *   Redistributions of source code must retain the above copyright notice, this list of conditions and the  following
 *   disclaimer.
 *
 *   Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *   following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *   Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote
 *   products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT  OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * To improve the quality of the software, users are encouraged to share modifications, enhancements or bug fixes
 * with Infineon Technologies AG (dave@infineon.com).
 ***********************************************************************************************************************
 *
 * Change History
 * --------------
 *
 * 2015-08-06:
 *     - Initial version for DAVEv4.
 *
 * @endcond
 *
 */
/***********************************************************************************************************************
 * HEADER FILES
 **********************************************************************************************************************/
#include "spi_slave.h"

/***********************************************************************************************************************
 * EXTERN DECLARATIONS
 ***********************************************************************************************************************/

extern void SPI_SLAVE_lProtocolHandler(const SPI_SLAVE_t * const handle);


/**********************************************************************************************************************
 * DATA STRUCTURES
 **********************************************************************************************************************/
SPI_SLAVE_STATUS_t SPI_BUS_init(void);

/*USIC channel configuration*/
const XMC_SPI_CH_CONFIG_t SPI_BUS_channel_config =
{
  .bus_mode     = XMC_SPI_CH_BUS_MODE_SLAVE,
  .parity_mode   = XMC_USIC_CH_PARITY_MODE_NONE
};
/*MISO pin configuration*/
const XMC_GPIO_CONFIG_t SPI_BUS_mosi1_pin_config   = 
{ 
#ifdef P2_5_AF_U0C1_DOUT0
  .mode             = (XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT2| P2_5_AF_U0C1_DOUT0), 
#else
  .mode             = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT2, 
#endif
  .output_level     = XMC_GPIO_OUTPUT_LEVEL_HIGH,
  .output_strength  = XMC_GPIO_OUTPUT_STRENGTH_STRONG_SHARP_EDGE 
};

/*MISO pin configuration used for initializing*/
const SPI_SLAVE_PIN_CONFIG_t SPI_BUS_miso_pin = 
{
  .port = (XMC_GPIO_PORT_t *)PORT2_BASE,
  .config = &SPI_BUS_mosi1_pin_config,
  .hw_control = XMC_GPIO_HWCTRL_DISABLED,
  .pin = 5U
};

/*MOSI pin configuration*/
const XMC_GPIO_CONFIG_t SPI_BUS_mosi_pin_config   = 
{ 
  .mode             = XMC_GPIO_MODE_INPUT_TRISTATE, 
  .output_level     = XMC_GPIO_OUTPUT_LEVEL_HIGH
};

/*MOSI pin configuration used for initializing*/
const SPI_SLAVE_PIN_CONFIG_t SPI_BUS_mosi_pin = 
{
  .port = (XMC_GPIO_PORT_t *)PORT2_BASE,
  .config = &SPI_BUS_mosi_pin_config,
  .hw_control = XMC_GPIO_HWCTRL_DISABLED,
  .pin = 2U
};

/*SCLK pin configuration*/
const XMC_GPIO_CONFIG_t SPI_BUS_sclk_pin_config   = 
{ 
  .mode             = XMC_GPIO_MODE_INPUT_TRISTATE, 
  .output_level     = XMC_GPIO_OUTPUT_LEVEL_HIGH,
  .output_strength  = XMC_GPIO_OUTPUT_STRENGTH_STRONG_MEDIUM_EDGE
};

/*SELIN pin configuration*/
const XMC_GPIO_CONFIG_t SPI_BUS_slavesel_pin_config   = 
{ 
  .mode             = XMC_GPIO_MODE_INPUT_TRISTATE, 
  .output_level     = XMC_GPIO_OUTPUT_LEVEL_HIGH,
  .output_strength  = XMC_GPIO_OUTPUT_STRENGTH_STRONG_MEDIUM_EDGE
};

const SPI_SLAVE_CONFIG_t SPI_BUS_conf =
{
  .channel_config = &SPI_BUS_channel_config,

  .mosi1_pin_config = &SPI_BUS_miso_pin,
  .mosi0_pin_config = &SPI_BUS_mosi_pin,
  .fptr_spi_slave_config = SPI_BUS_init,
  .parity_error_cbhandler = NULL,
  .slave_select_cbhandler = NULL,
  .transmit_mode = SPI_SLAVE_TRANSFER_MODE_DIRECT,
  .receive_mode = SPI_SLAVE_TRANSFER_MODE_DIRECT,
  .tx_fifo_size = XMC_USIC_CH_FIFO_DISABLED,
  .rx_fifo_size = XMC_USIC_CH_FIFO_DISABLED,
  .spi_configured_mode = XMC_SPI_CH_MODE_STANDARD,
  .tx_sr = 0U,
#ifdef USIC0_C1_DX0_P2_2
  .dx0_source = (SPI_SLAVE_INPUT_t)USIC0_C1_DX0_P2_2,
#else
  .dx0_source = SPI_SLAVE_INPUT_INVALID,
#endif
};

SPI_SLAVE_RUNTIME_t SPI_BUS_runtime =
{
  .spi_current_mode = XMC_SPI_CH_MODE_STANDARD,
  .data_size = SPI_SLAVE_DATA_SIZE_8BITS,
  .tx_busy = false,
  .rx_busy = false
};

SPI_SLAVE_t SPI_BUS =
{
  .channel = XMC_SPI0_CH1,
  .config = &SPI_BUS_conf,
  .runtime = &SPI_BUS_runtime
};

/**********************************************************************************************************************
 * API IMPLEMENTATION
 **********************************************************************************************************************/
/*Channel initialization function*/
SPI_SLAVE_STATUS_t SPI_BUS_init()
{
  SPI_SLAVE_STATUS_t status = SPI_SLAVE_STATUS_SUCCESS;
  /*Reset the runtime state variables*/
  SPI_BUS.runtime->tx_busy = false;
  SPI_BUS.runtime->rx_busy = false;

  /*Configure SCLK pin*/
  XMC_GPIO_Init((XMC_GPIO_PORT_t *)PORT2_BASE, 4U, &SPI_BUS_sclk_pin_config);
  /*Configure slave select pin*/
  XMC_GPIO_Init((XMC_GPIO_PORT_t *)PORT2_BASE, 3U, &SPI_BUS_slavesel_pin_config);
  /*Configure MOSI pin*/
  XMC_GPIO_Init((XMC_GPIO_PORT_t *)PORT2_BASE, 2U, &SPI_BUS_mosi_pin_config);
  /* Initialize USIC channel in SPI slave mode*/
  XMC_SPI_CH_Init(XMC_SPI0_CH1, &SPI_BUS_channel_config);
  XMC_SPI_CH_SetBitOrderMsbFirst(XMC_SPI0_CH1);

  XMC_SPI_CH_SetWordLength(XMC_SPI0_CH1, (uint8_t)8U);
  XMC_SPI_CH_SetFrameLength(XMC_SPI0_CH1, (uint8_t)64U);

  /*Set input source path*/
  XMC_SPI_CH_SetInputSource(XMC_SPI0_CH1, (XMC_SPI_CH_INPUT_t)XMC_USIC_CH_INPUT_DX0, 0U);
  XMC_SPI_CH_SetInputSource(XMC_SPI0_CH1, (XMC_SPI_CH_INPUT_t)XMC_USIC_CH_INPUT_DX1, 0U);
  XMC_SPI_CH_SetInputSource(XMC_SPI0_CH1, (XMC_SPI_CH_INPUT_t)XMC_USIC_CH_INPUT_DX2, 0U);
  XMC_SPI_CH_EnableInputInversion(XMC_SPI0_CH1, XMC_SPI_CH_INPUT_SLAVE_SELIN);
  /*Set service request for SPI protocol events*/
  XMC_USIC_CH_SetInterruptNodePointer(XMC_SPI0_CH1, XMC_USIC_CH_INTERRUPT_NODE_POINTER_PROTOCOL,
     0U);
  /* Start SPI */
  XMC_SPI_CH_Start(XMC_SPI0_CH1);

  /* Initialize SPI Slave MISO pin */
  XMC_GPIO_Init((XMC_GPIO_PORT_t *)PORT2_BASE, 5U, &SPI_BUS_mosi1_pin_config);
  XMC_GPIO_SetHardwareControl((XMC_GPIO_PORT_t *)PORT2_BASE, 5U, XMC_GPIO_HWCTRL_DISABLED);

  return status;
}
