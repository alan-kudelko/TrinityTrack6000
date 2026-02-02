/**
 * @file task_diagnostics.h
 * @brief Diagnostics task for system monitoring with CLI interface.
 * 
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */

#ifndef TASK_DIAGNOSTICS_H_
    #define TASK_DIAGNOSTICS_H_

#include <tx_api.h>

#define TASK_DIAGNOSTICS_STACK_SIZE 512  /**< Stack size for diagnostics task */
#define TASKS_DIAGNOSTICS_PRIORITY    2  /**< Priority for diagnostics task */

extern const char task_diagnostics_name[];  /**< Name of the diagnostics task */

extern TX_THREAD task_diagnostics_handle __attribute((section(".task_handles.task_diagnostics"))); /**< Thread handlefor diagnostics task */
extern ULONG task_diagnostics_stack[TASK_DIAGNOSTICS_STACK_SIZE] __attribute((section(".task_stacks.task_diagnostics"))); /**< Stack for diagnostics task */
extern TX_SEMAPHORE sem_task_diagnostics_command_ready __attribute((section(".task_semaphores.task_diagnostics"))); /**< Semaphore for diagnostics task indicating command ready to parse */


#ifdef __cplusplus
    extern "C"{
#endif // __cplusplus

/**
 * @brief Initialize diagnostics task.
 * This function sets up necessary resources for the diagnostics task,
 * such as semaphores.
 * @param None.
 * @return None.
 */
void task_diagnostics_init(void);

/**
 * @brief Diagnostics task function.
 * This task handles system diagnostics and provides a CLI interface
 * for monitoring system status and performance.
 * @param arg: Argument passed to the task (not used).
 * @return None.
 */

void task_diagnostics(ULONG arg);

#ifdef __cplusplus
    }
#endif // __cplusplus


#endif // TASK_DIAGNOSTICS_H_