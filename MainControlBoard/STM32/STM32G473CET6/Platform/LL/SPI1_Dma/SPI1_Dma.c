/**
 * @addtogroup SPI1_DMA
 * @{
 */

#include <SPI1_Dma.h>

#include <stdbool.h>
#include <string.h>

#include <stm32g4xx_hal.h>

#include <TrinityTrack6000_Pinout.h>

/**
 * @name SPI1 Transaction Engine State
 * @brief Internal runtime state used by the SPI1 DMA transaction scheduler
 * @{
 */

static volatile hspi_data hspi1_transaction_buffer[SPI1_HSPI_DATA_BUFFER_SIZE] SECTION(".DMA_RAM2.SPI1_Data"); //!< Buffer for storing SPI1 transaction parameters, stored in .SPI1_Data section in RAM2

static volatile uint16_t hspi1_transaction_buffer_head SECTION(".crit.dmaVars"); //!< Head index for the SPI1 transaction buffer
static volatile uint16_t hspi1_transaction_buffer_tail SECTION(".crit.dmaVars"); //!< Tail index for the SPI1 transaction buffer
static volatile uint16_t hspi1_transaction_buffer_length SECTION(".crit.dmaVars"); //!< Length of the SPI1 transaction buffer, used to keep track of the number of transactions currently queued in the buffer

static volatile bool hspi1_dma_active SECTION(".crit.dmaVars"); //!< Flag indicating whether SPI1 DMA transmission is currently active
static volatile bool hspi1_tx_processed SECTION(".crit.dmaVars"); //!< Flag indicating whether tx transmission is finished (needed when there may be read operation pending after tx)

/**@} */

extern SPI_HandleTypeDef hspi1;
extern DMA_HandleTypeDef hdma_spi1_tx;
extern DMA_HandleTypeDef hdma_spi1_rx;

void spi1_dma_init(void){
    // Initialize ring buffer variables
    // all variables are stored in .dmaBuff section in RAM2 memory region for faster access
    memset((void*)hspi1_transaction_buffer,0,sizeof(hspi1_transaction_buffer));
    hspi1_transaction_buffer_head=0;
    hspi1_transaction_buffer_tail=0;
    hspi1_transaction_buffer_length=0;
    hspi1_dma_active=false;
    hspi1_tx_processed=false;

    __HAL_DMA_DISABLE_IT(&hdma_spi1_rx,DMA_IT_HT);
}

bool spi1_dma_enq_data(hspi_data*transactionData){
    // 1. Check if there is enough space in the buffer for the new transaction
    __disable_irq(); // Fix it
    uint16_t free_space=0;
    if(hspi1_transaction_buffer_head>=hspi1_transaction_buffer_tail){
        free_space=SPI1_HSPI_DATA_BUFFER_SIZE-(hspi1_transaction_buffer_head-hspi1_transaction_buffer_tail);
    }
    else{
        free_space=hspi1_transaction_buffer_head-hspi1_transaction_buffer_tail;
    }

    if(free_space>0){
        // There is enough space, enqueue the data
        hspi1_transaction_buffer[hspi1_transaction_buffer_head]=*transactionData;
        hspi1_transaction_buffer_length++;
        hspi1_transaction_buffer_head=(hspi1_transaction_buffer_head+1)%SPI1_HSPI_DATA_BUFFER_SIZE;
        // If DMA is not active start transmission
        if(hspi1_dma_active==false){
            hspi1_dma_active=true;
            spi1_send_data();
        }
        __enable_irq();
        // If DMA is active, data will be sent upon DMA completion interrupt
        return true;
    }
    else{
        __enable_irq();
        // Not enough space
        // Intented to use with RTOS where caller can retry later - when space is available
        return false;
    }

    return false;
}

uint8_t spi1_send_data(void){
    // Check if communication is full duplex or half duplex
    switch(hspi1_transaction_buffer[hspi1_transaction_buffer_tail].flags){
        case HSPI_FLAG_HALF_DUPLEX:
            if(hspi1_tx_processed==false){
                // First transmission is always tx
                // So that, we have to select slave by pulling CS pin LOW
                HAL_GPIO_WritePin(hspi1_transaction_buffer[hspi1_transaction_buffer_tail].gpio_port,
                    hspi1_transaction_buffer[hspi1_transaction_buffer_tail].gpio_pin,GPIO_PIN_RESET);
                HAL_SPI_Transmit_DMA(&hspi1,
                    hspi1_transaction_buffer[hspi1_transaction_buffer_tail].txBuffer,
                    hspi1_transaction_buffer[hspi1_transaction_buffer_tail].txLength);
            }
            else{
                HAL_SPI_Receive_DMA(&hspi1,
                    hspi1_transaction_buffer[hspi1_transaction_buffer_tail].rxBuffer,
                    hspi1_transaction_buffer[hspi1_transaction_buffer_tail].rxLength);
            }
        break;
        case HSPI_FLAG_FULL_DUPLEX:
            HAL_GPIO_WritePin(hspi1_transaction_buffer[hspi1_transaction_buffer_tail].gpio_port,
                hspi1_transaction_buffer[hspi1_transaction_buffer_tail].gpio_pin,GPIO_PIN_RESET);
            HAL_SPI_TransmitReceive_DMA(&hspi1,
                hspi1_transaction_buffer[hspi1_transaction_buffer_tail].txBuffer,
                hspi1_transaction_buffer[hspi1_transaction_buffer_tail].rxBuffer,
                hspi1_transaction_buffer[hspi1_transaction_buffer_tail].txLength);
        break;
        default:
        // Should not happen, invalid flag value
    }

    return 0;
}

void spi1_dma_tx_complete(void){
    __disable_irq();
    switch(hspi1_transaction_buffer[hspi1_transaction_buffer_tail].flags){
        case HSPI_FLAG_HALF_DUPLEX:
            // Check if there was a read operation
            // Read operation requires rx buffer to be provided
            // If there is no rx buffer (NULL or nullptr) move on to the next package
            if((hspi1_transaction_buffer[hspi1_transaction_buffer_tail].rxBuffer!=NULL)&&(hspi1_tx_processed==false)){
                hspi1_tx_processed=true;
            }
            else{
                // Notify the owner of the data that the transmission was complete
                if(hspi1_transaction_buffer[hspi1_transaction_buffer_tail].callbackFn!=NULL){
                    hspi1_transaction_buffer[hspi1_transaction_buffer_tail].callbackFn();
                }

                HAL_GPIO_WritePin(
                    hspi1_transaction_buffer[hspi1_transaction_buffer_tail].gpio_port,
                    hspi1_transaction_buffer[hspi1_transaction_buffer_tail].gpio_pin,GPIO_PIN_SET);

                hspi1_transaction_buffer_tail=(hspi1_transaction_buffer_tail+1)%SPI1_HSPI_DATA_BUFFER_SIZE;
                hspi1_transaction_buffer_length--;
                hspi1_tx_processed=false;
            }
        break;
        case HSPI_FLAG_FULL_DUPLEX:
            if(hspi1_transaction_buffer[hspi1_transaction_buffer_tail].callbackFn!=NULL){
                hspi1_transaction_buffer[hspi1_transaction_buffer_tail].callbackFn();
            }
            HAL_GPIO_WritePin(
                hspi1_transaction_buffer[hspi1_transaction_buffer_tail].gpio_port,
                hspi1_transaction_buffer[hspi1_transaction_buffer_tail].gpio_pin,GPIO_PIN_SET);

            hspi1_transaction_buffer_tail=(hspi1_transaction_buffer_tail+1)%SPI1_HSPI_DATA_BUFFER_SIZE;
            hspi1_transaction_buffer_length--;
            hspi1_tx_processed=false;
        break;
        default:
        // Should not happen, invalid flag value
    }
    // If there is more data to send
    if(hspi1_transaction_buffer_length>0){
        spi1_send_data();
    }
    else{
        hspi1_dma_active=false;
    }
    __enable_irq();
}

/**@} */
