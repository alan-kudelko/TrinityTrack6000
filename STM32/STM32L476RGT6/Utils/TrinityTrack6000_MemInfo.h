//Doxy
#ifndef _TRINITYTRACK6000_MEMINFO_H_
    #define _TRINITYTRACK6000_MEMINFO_H_

#define MEMINFO_UART_TIMEOUT 1000
#define MEMINFO_HEX_BUFFER_SIZE 20

/** @name Headers and footers for RAM memory dumps
 *  @{
 */
extern const char msg_ramDump_header1[];      /**< General RAM dump header part 1 */
extern const char msg_ramDump_header2[];      /**< General RAM dump header part 2 */
extern const char msg_ramDump_data_header[];  /**< Header for initialized data section */
extern const char msg_ramDump_data_footer[];  /**< Footer for initialized data section */
extern const char msg_ramDump_bss_header[];   /**< Header for BSS section */
extern const char msg_ramDump_bss_footer[];   /**< Footer for BSS section */
extern const char msg_ramDump_tdat_header[];  /**< Header for task data section */
extern const char msg_ramDump_tdat_footer[];  /**< Footer for task data section */
extern const char msg_ramDump_heap_header[];  /**< Header for heap section */
extern const char msg_ramDump_heap_footer[];  /**< Footer for heap section */
extern const char msg_ramDump_stack_header[]; /**< Header for stack section */
extern const char msg_ramDump_stack_footer[]; /**< Footer for stack section */
extern const char msg_ramDump_free_header[];  /**< Header for free RAM section */
extern const char msg_ramDump_free_footer[];  /**< Footer for free RAM section */
extern const char msg_ramDump_addr_header[];  /**< Header for memory address listings */
extern const char msg_ramDump_seperator[];    /**< Separator string for memory output */

extern const char msg_ram2Dump_header1[];     /**< General RAM2 dump header part 1 */
/** @} */

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

// RAM memory info bank 1
void ram_dump(void);

// RAM memory info bank 2
void ram2_dump(void);

// Flash memory info
void flash_dump(void);

#ifdef __cplusplus
    }
#endif // __cplusplus

#endif // _TRINITYTRACK6000_MEMINFO_H_