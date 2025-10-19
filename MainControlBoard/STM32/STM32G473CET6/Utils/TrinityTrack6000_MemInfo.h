/**
 * @file TrinityTrack6000_MemInfo.h
 * @brief RAM diagnostics interface for TrinityTrack6000 project.
 *
 * This module provides functions and variables for monitoring and 
 * reporting memory usage across different RAM banks (RAM1, RAM2, CCSRAM). 
 * It defines diagnostics headers/footers, format strings for 
 * generating ASCII reports, and dedicated variables stored in 
 * `.ramDiagnostics` linker sections to track memory sizes and usage.
 *
 * Features:
 * - Initialization and refresh of memory usage information
 * - General RAM diagnostics overview
 * - Per-bank diagnostics for RAM1, RAM2, and CCSRAM
 * - Tracking of heap and stack pointers in RAM1
 * - Allocation of diagnostic variables in separate linker sections
 *
 * Usage:
 * - Call `ramInfoInit()` during system initialization to set up total sizes
 * - Call `ramInfoRefresh()` on-demand or periodically to update usage data
 * - Call `ramInfoGeneral()`, `ramInfoRAM1()`, etc. to print details
 *
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */
#ifndef TRINITYTRACK6000_MEMINFO_H_
    #define TRINITYTRACK6000_MEMINFO_H_

#define MEMINFO_LINE_BUFFER_SIZE 90
#define MEMINFO_BAR_BUFFER_SIZE 11

/** @name Headers and footers for RAM memory dumps
 *  @{
 */
extern const char msg_ramDiagnosticsGeneral_header1[];  /**< General RAM diagnostics header line 1 */
extern const char msg_ramDiagnosticsGeneral_header2[];  /**< General RAM diagnostics header line 2 */
extern const char msg_ramDiagnosticsGeneral_header3[];  /**< General RAM diagnostics header line 3 */
extern const char msg_ramDiagnosticsGeneral_formatStringRAM1[];  /**< General RAM diagnostics format string for RAM1 */
extern const char msg_ramDiagnosticsGeneral_formatStringRAM2[];  /**< General RAM diagnostics format string for RAM2 */
extern const char msg_ramDiagnosticsGeneral_formatStringCCSRAM[];  /**< General RAM diagnostics format string for CCSRAM */
extern const char msg_ramDiagnosticsGeneral_formatStringFreeRAM[];  /**< General RAM diagnostics format string for free RAM */
extern const char msg_ramDiagnosticsGeneral_footer1[];  /**< General RAM diagnostics footer line 1 */
extern const char msg_ramDiagnosticsGeneral_footer2[];  /**< General RAM diagnostics footer line 2 */

extern const char msg_ramDiagnosticsRAM1_header1[];  /**< RAM1 diagnostics header line 1 */
extern const char msg_ramDiagnosticsRAM1_header2[];  /**< RAM1 diagnostics header line 2 */
extern const char msg_ramDiagnosticsRAM1_header3[];  /**< RAM1 diagnostics header line 3 */

extern const char msg_ramDiagnosticsRAM1_formatStringData[];   /**< RAM1 diagnostics format string for .data section */
extern const char msg_ramDiagnosticsRAM1_formatStringBSS[];    /**< RAM1 diagnostics format string for .bss section */
extern const char msg_ramDiagnosticsRAM1_formatStringHeap[];   /**< RAM1 diagnostics format string for .heap section */
extern const char msg_ramDiagnosticsRAM1_formatStringStack[];  /**< RAM1 diagnostics format string for .stack section */

extern const char msg_ramDiagnosticsRAM1_formatStringFreeRAM[];  /**< RAM1 diagnostics format string for free RAM */


extern const char msg_ramDiagnosticsRAM2_header1[]; /**< RAM2 diagnostics header line 1 */
extern const char msg_ramDiagnosticsRAM2_formatStringRamDia[]; /**< RAM2 diagnostics format string for .ramDiagnostics section */
extern const char msg_ramDiagnosticsRAM2_formatStringSysDia[]; /**< RAM2 diagnostics format string for .sysDiag section */

extern const char msg_ramDiagnosticsCCSRAM_header1[]; /**< CCSRAM diagnostics header line 1 */
extern const char msg_ramDiagnosticsCCSRAM_formatStringCrit[]; /**< CCSRAM diagnostics format string for .crit section */
/** @} */

/**
 * @brief RAM diagnostics variables
 * @{
 */
extern uint16_t ramDiagnosticsGeneral_total_size __attribute((section(".ramDiagnostics.uint16_t"))); /**<  Total size of all RAM in kB */
extern uint8_t ramDiagnosticsRAM1_total_size     __attribute((section(".ramDiagnostics.uint8_t"))); /**<  Total size of bank RAM1 in kB */
extern uint8_t ramDiagnosticsRAM2_total_size     __attribute((section(".ramDiagnostics.uint8_t"))); /**<  Total size of bank RAM2 in kB */
extern uint8_t ramDiagnosticsCCSRAM_total_size   __attribute((section(".ramDiagnostics.uint8_t"))); /**<  Total size of bank CCSRAM in kB */

extern uint16_t ramDiagnosticsGeneral_used       __attribute((section(".ramDiagnostics.uint16_t"))); /**<  Total amount of used RAM memory kB */
extern uint8_t ramDiagnosticsRAM1_used           __attribute((section(".ramDiagnostics.uint8_t")));   /**<  Amount of used memory in bank RAM1 kB */
extern uint8_t ramDiagnosticsRAM2_used           __attribute((section(".ramDiagnostics.uint8_t")));   /**<  Amount of used memory in bank RAM2 kB */
extern uint8_t ramDiagnosticsCCSRAM_used         __attribute((section(".ramDiagnostics.uint8_t"))); /**<  Amount of used memory in bank CCSRAM kB */

extern uint32_t ramDiagnosticsRAM1_lastMSP       __attribute((section(".ramDiagnostics.uint32_t")));  /**<  Last value of Main Stack Pointer in RAM1 */
extern uint32_t ramDiagnosticsRAM1_lastHeapEnd   __attribute((section(".ramDiagnostics.uint32_t"))); /**<  Last value of heap end pointer in RAM1 */
extern uint8_t ramDiagnosticsRAM1_data_size      __attribute((section(".ramDiagnostics.uint8_t")));  /**<  Size of .data section in RAM1 */
extern uint8_t ramDiagnosticsRAM1_bss_size       __attribute((section(".ramDiagnostics.uint8_t")));   /**<  Size of .bss section in RAM1 */
extern uint8_t ramDiagnosticsRAM1_tdat_size      __attribute((section(".ramDiagnostics.uint8_t")));  /**<  Size of .tdat section in RAM1 */
extern uint8_t ramDiagnosticsRAM1_heap_size      __attribute((section(".ramDiagnostics.uint8_t")));  /**<  Size of .heap section in RAM1 */
extern uint8_t ramDiagnosticsRAM1_stack_size     __attribute((section(".ramDiagnostics.uint8_t"))); /**<  Size of .stack section in RAM1 */

extern uint8_t ramDiagnosticsRAM2_ramDiagnostics_size __attribute((section(".ramDiagnostics.uint8_t"))); /**< Size of .ramDiagnostics section in RAM2 */
extern uint8_t ramDiagnosticsRAM2_sysDiagnostics_size __attribute((section(".ramDiagnostics.uint8_t"))); /**< Size of .sysDiagnostics section in RAM2 */

extern uint8_t ramDiagnosticsCCSRAM_crit_size __attribute((section(".ramDiagnostics.uint8_t"))); /**< Size of .crit section in CCSRAM */
/** @} */

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

/**
 * @brief Initialize RAM diagnostics.
 *
 * Calculates the total sizes of all RAM banks and refreshes the current usage.  
 * This function should be called once during system initialization 
 * (e.g., inside `initializeSystem()`).
 */
void ramInfoInit(void);

/**
 * @brief Refresh RAM diagnostics data.
 *
 * Updates the usage information for all RAM banks (RAM1, RAM2, CCSRAM).  
 * Should be called periodically or on-demand when up-to-date memory 
 * information is required.
 */
void ramInfoRefresh(void);

/**
 * @brief Print general RAM usage information.
 *
 * Displays an overview of all RAM banks, including start/end addresses, 
 * total sizes, usage bars, and percentage utilization.
 */
void ramInfoGeneral(void);

/**
 * @brief Print detailed RAM1 diagnostics.
 *
 * Displays section-level information for RAM1, including `.data`, `.bss`, 
 * `.heap`, and `.stack` sizes, usage, and high-water marks.
 */
void ramInfoRAM1(void);

/**
 * @brief Print detailed RAM2 diagnostics.
 *
 * Displays section-level information for RAM2.  
 * By default, this includes `.ramDiagnostics` or any custom sections 
 * assigned to RAM2.
 */
void ramInfoRAM2(void);

/**
 * @brief Print detailed CCSRAM diagnostics.
 *
 * Displays section-level information for CCMRAM/CCSRAM.
 * Yes to be implemented
 */
void ramInfoCCSRAM(void);


#ifdef __cplusplus
    }
#endif // __cplusplus

#endif // TRINITYTRACK6000_MEMINFO_H_