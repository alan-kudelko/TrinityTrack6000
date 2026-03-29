/**
 * @addtogroup task_McuAnalog
 * @{
 */
#include <stm32g4xx_hal.h>

#include <task_McuAnalog.h>

const char task_McuAnalog_name[]="MCU Analog Task";

TX_THREAD task_McuAnalog_handle SECTION(".task_handles");;
ULONG task_McuAnalog_stack[TASK_MCU_ANALOG_STACK_SIZE] SECTION(".task_stacks_ccsram");;

void task_McuAnalog_init(void){

}

void task_McuAnalog(ULONG arg){
    UNUSED(arg);

    while(1){
        tx_thread_sleep(1000);
    }
}

/**@} */