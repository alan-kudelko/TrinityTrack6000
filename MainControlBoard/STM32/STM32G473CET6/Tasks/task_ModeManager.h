/**
 * @file task_ModeManager.h
 * @brief 
 * 
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */

// Doxy what this task does

#ifndef TASK_MODE_MANAGER_H_
    #define TASK_MODE_MANAGER_H_

#ifdef __DOXYGEN__
    #define SECTION(x)
    #define ALIGNED(x)
#else
    #define SECTION(x) __attribute((section(x)))
    #define ALIGNED(x) __attribute((aligned(x)))
#endif // __DOXYGEN__

typedef enum SYSTEM_MODE{RUN,TEST,FAILSAFE,FAULT}SYSTEM_MODE;

extern SYSTEM_MODE system_mode;

#ifdef __cplusplus
    extern "C"{
#endif // __cplusplus




#ifdef __cplusplus
    }
#endif // __cplusplus


#endif // TASK_MODE_MANAGER_H_