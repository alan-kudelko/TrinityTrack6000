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

#define TASK_DIAGNOSTICS_STACK_SIZE 256  /**< Stack size for diagnostics task */
#define TASKS_DIAGNOSTICS_PRIORITY    2  /**< Priority for diagnostics task */

extern const char task_diagnostics_name[];  /**< Name of the diagnostics task */

extern TX_THREAD task_diagnostics_handle __attribute((section(".task_handles.task_diagnostics"))); /**< Thread handlefor diagnostics task */
extern ULONG task_diagnostics_stack[TASK_DIAGNOSTICS_STACK_SIZE] __attribute((section(".task_stacks.task_diagnostics"))); /**< Stack for diagnostics task */


#ifdef __cplusplus
    extern "C"{
#endif // __cplusplus

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