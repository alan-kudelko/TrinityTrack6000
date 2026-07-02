/**
 * @addtogroup task_McuAnalog
 * @{
 */
#include <stm32g4xx_hal.h>

#include <task_McuAnalog.h>

#include <usart3_dma.h>

const char task_McuAnalog_name[]="MCU Analog Task";

TX_THREAD task_McuAnalog_handle SECTION(".task_handles");;
ULONG task_McuAnalog_stack[TASK_MCU_ANALOG_STACK_SIZE] SECTION(".task_stacks_ccsram");;

SystemMeasurement_t system_measurements SECTION(".sysDiag"); // Structure holding the latest system measurements

void mcu_analog_timer_callback(void){
    // This function is called from the timer interrupt handler
    // It should trigger ADC conversions and read the results
    // For now, just a placeholder
}

void task_McuAnalog_create(void){
    memset(&system_measurements,0,sizeof(system_measurements));

    tx_thread_create(&task_McuAnalog_handle,
                    (char*)task_McuAnalog_name,
                    task_McuAnalog,
                    0,
                    &task_McuAnalog_stack,
                    sizeof(task_McuAnalog_stack),
                    TASK_MCU_ANALOG_PRIORITY,
                    TASK_MCU_ANALOG_PRIORITY,
                    TX_NO_TIME_SLICE,
                    TX_AUTO_START);
}

void task_McuAnalog(ULONG arg){
    UNUSED(arg);

    while(1){

        tx_thread_sleep(5000);
    }
}

/**@} */