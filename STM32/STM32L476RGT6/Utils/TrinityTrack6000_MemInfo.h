//Doxy
#ifndef _TRINITYTRACK6000_MEMINFO_H_
    #define _TRINITYTRACK6000_MEMINFO_H_

#define MEMINFO_UART_TIMEOUT 1000
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

/** @} */

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

// Initialize RAM diagnostics - calculate total RAM sizes and refresh current usage
// Move to initializeSystem()
void ramDiagnositcsInit(void);

// Calculate current RAM usage
void ramDiagnosticsRefresh(void);

// RAM memory info - general
void ramDiagnosticsGeneral(void);

// RAM memory info bank 1
void ramDiagnosticsRAM1(void);

// RAM memory info bank 2
void ramDiagnosticsRAM2(void);

// RAM memory info CCSRAM
void ramDiagnosticsCCSRAM(void);


#ifdef __cplusplus
    }
#endif // __cplusplus

#endif // _TRINITYTRACK6000_MEMINFO_H_