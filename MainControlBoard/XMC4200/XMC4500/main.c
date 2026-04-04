/*
 * main.c
 *
 *  Created on: 2025 Oct 16 15:03:20
 *  Author: kujon
 */




#include "DAVE.h"                 //Declarations from DAVE Code Generation (includes SFR declaration)

/**

 * @brief main() - Application entry point
 *
 * <b>Details of function</b><br>
 * This routine is the application entry point. It is invoked by the device startup code. It is responsible for
 * invoking the APP initialization dispatcher routine - DAVE_Init() and hosting the place-holder for user application
 * code.
 */
#define SPI_CH       XMC_SPI0_CH0

void SPI_Slave_Init(void)
{
    // MOSI jako input (DX0)
    XMC_GPIO_SetMode(XMC_GPIO_PORT2, 2, XMC_GPIO_MODE_INPUT_TRISTATE); // DX0

    // MISO jako output
    XMC_GPIO_SetMode(XMC_GPIO_PORT2, 5, XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT2); // DOUT

    // SCLK jako input
    XMC_GPIO_SetMode(XMC_GPIO_PORT2, 4, XMC_GPIO_MODE_INPUT_TRISTATE); // SCLK

    // Konfiguracja SPI SLAVE
    XMC_SPI_CH_CONFIG_t config = {
        .baudrate = 1000000, // nieistotne w slave
        .bus_mode = XMC_SPI_CH_BUS_MODE_SLAVE,
        .selo_inversion = XMC_SPI_CH_SLAVE_SEL_INV_TO_MSLS,
        .parity_mode = XMC_USIC_CH_PARITY_MODE_NONE
    };

    XMC_SPI_CH_Init(SPI_CH, &config);

    // Input source (DX0 = MOSI)
    XMC_SPI_CH_SetInputSource(SPI_CH, XMC_SPI_CH_INPUT_DIN0, USIC0_C0_DX0_P2_2);

    // FIFO RX
    XMC_USIC_CH_RXFIFO_Configure(SPI_CH,
        0, // start index
        XMC_USIC_CH_FIFO_SIZE_16WORDS,
        1  // trigger level
    );

    // FIFO TX (żeby nie było underruna)
    XMC_USIC_CH_TXFIFO_Configure(SPI_CH,
        16,
        XMC_USIC_CH_FIFO_SIZE_16WORDS,
        1
    );

    // start channel
    XMC_SPI_CH_Start(SPI_CH);
}

int main(void)
{
  DAVE_STATUS_t status;

  status = DAVE_Init();           /* Initialization of DAVE APPs  */

  if (status != DAVE_STATUS_SUCCESS)
  {
    /* Placeholder for error handler code. The while loop below can be replaced with an user error handler. */
    XMC_DEBUG("DAVE APPs initialization failed\n");

    while(1U)
    {

    }
  }


  /* Placeholder for user application code. The while loop below can be replaced with user application code. */
  XMC_GPIO_SetMode(XMC_GPIO_PORT1,1,XMC_GPIO_MODE_OUTPUT_PUSH_PULL);
  XMC_GPIO_SetOutputHigh(XMC_GPIO_PORT1, 1);

  while(1U)
  {
      for (volatile uint32_t i = 0; i < 10000000; ++i){

      }
      XMC_GPIO_SetOutputLow(XMC_GPIO_PORT1,1);
       for (volatile uint32_t i = 0; i < 10000000; ++i){

       }
      
       	   XMC_GPIO_SetOutputHigh(XMC_GPIO_PORT1,1);
  }
}
