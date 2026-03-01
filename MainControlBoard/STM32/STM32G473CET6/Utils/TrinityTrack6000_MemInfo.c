/**
 * @addtogroup TrinityTrack6000_MemInfo
 * @{
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <tx_port.h>

#include <TrinityTrack6000_MemInfo.h>
#include <TrinityTrack6000_Config.h>
#include <USART1_Dma.h>

#include <core_cm4.h>

extern uint32_t __RAM1_start__; // Defined in the linker script by me for RAM1 start
extern uint32_t __RAM1_end__;   // Defined in the linker script by me for RAM1 end

extern uint32_t _edata;
extern uint32_t __bss_start__;
extern uint32_t __bss_end__;
extern uint32_t _end; // Start of heap as heap grows upwards
extern uint32_t _heap_start; // Defined in the linker script by me for heap start

extern uint32_t __RAM2_start__; // Defined in the linker script by me for RAM2 start
extern uint32_t __RAM2_end__; // Defined in the linker script by me for RAM2 end

extern uint32_t __CCSRAM_start__; // Defined in the linker script by me for CCSRAM start
extern uint32_t __CCSRAM_end__; // Defined in the linker script by me for CCSRAM end

extern uint32_t __TASK_HANDLES_START__; // Defined in the linker script by me for start of taskHandles section in RAM1
extern uint32_t __TASK_HANDLES_END__; // Defined in the linker script by me for end of taskHandles section in RAM1

extern uint32_t __TASK_STACKS_START__; // Defined in the linker script by me for start of taskStacks section in RAM1
extern uint32_t __TASK_STACKS_END__; // Defined in the linker script by me for end of taskStacks section in RAM1

extern uint32_t __RAM_DIAGNOSTICS_START__; // Defined in the linker script be me for start of ramDiagnostics section in RAM2
extern uint32_t __RAM_DIAGNOSTICS_END__; // Defined in the linker script by me for end of ramDiagnostics section in RAM2

extern uint32_t __SYS_DIAGNOSTICS_START__; // Defined in the linker scritp be me for start of sysDiag section in RAM2
extern uint32_t __SYS_DIAGNOSTICS_END__; // Defined in the linker script by me for end of sysDiag section in RAM2

extern uint32_t __CRIT_START__; // Defined in the linker script by me for start of crit section in CCSRAM
extern uint32_t __CRIT_END__; // Defined in the linker script by me for end of crit section in CCSRAM
extern uint32_t __DMA_START__; // Defined in the linker script by me for start of dmaBuff section in CCSRAM
extern uint32_t __DMA_END__; // Defined in the linker script by me for end of dmaBuff section in CCSRAM

extern uint8_t* __sbrk_heap_end; //!< Defined in sysmem.c

extern UART_HandleTypeDef huart1;

const char msg_ramDiagnosticsGeneral_header1[]               ="+----------------------------[ RAM DIAGNOSTICS ]-----------------------------+\r\n";
const char msg_ramDiagnosticsGeneral_header2[]               ="| Bank         | Start      | End        | Size    | Usage      | Used       |\r\n"; 
const char msg_ramDiagnosticsGeneral_header3[]               ="+--------------+------------+------------+---------+------------+------------+\r\n";
                                                           //  | RAM1         | 0x20000000 | 0x2001FFFF | 128 KB  | ########## | 80%        |
const char msg_ramDiagnosticsGeneral_formatStringRAM1[]      ="│ RAM1         │ 0x%08lX │ 0x%08lX │ %3u  KB │%11s │ %3u%%       │\r\n";			  
								                           //  | RAM2         | 0x20020000 | 0x2003FFFF |  64 KB  | ####------| 40%         |
const char msg_ramDiagnosticsGeneral_formatStringRAM2[]      ="│ RAM2         │ 0x%08lX │ 0x%08lX │ %3u  KB │%11s │ %3u%%       │\r\n";
								                           //  | CCSRAM       | 0x10000000 | 0x10003FFF |  16 KB  | ##--------| 20%         |
const char msg_ramDiagnosticsGeneral_formatStringCCSRAM[]    ="│ CCSRAM       │ 0x%08lX │ 0x%08lX │ %3u  KB │%11s │ %3u%%       │\r\n";
                                                           //  +--------------+------------+------------+---------+-----------+-------------+
                                                           //  | FREE RAM TOTAL: 600 KB                                                     |
const char msg_ramDiagnosticsGeneral_formatStringFreeRAM[]   ="│ FREE RAM TOTAL: %3u KB / %3u KB                                            │\r\n";
const char msg_ramDiagnosticsGeneral_footer1[]               ="| Commands: s(snapshot) b(bank) q(quit)                                      |\r\n"; 
const char msg_ramDiagnosticsGeneral_footer2[]               ="+----------------------------------------------------------------------------+\r\n";       	

const char msg_ramDiagnosticsRAM1_header1[]                  ="+---------------------------[ BANK RAM1 DETAILS ]----------------------------+\r\n";
const char msg_ramDiagnosticsRAM1_header2[]                  ="| Section       | Start      | End        | Size    | Usage     |            |\r\n"; 
const char msg_ramDiagnosticsRAM1_header3[]                  ="+---------------+------------+------------+---------+-----------+------------+\r\n";
                                                           //  | .DATA         | 0x20000000 | 0x20007FFF | 32 KB   | 32 KB     |            |
const char msg_ramDiagnosticsRAM1_formatStringData[]         ="| .DATA         | 0x%08lX | 0x%08lX | %3u  KB | %3u  KB   |            |\r\n";			  
												           //  | .BSS          | 0x20008000 | 0x2000DFFF | 24 KB   | 18 KB     |            |
const char msg_ramDiagnosticsRAM1_formatStringBSS[]          ="| .BSS          | 0x%08lX | 0x%08lX | %3u  KB | %3u  KB   |            |\r\n";
												           //  | .TDAT         |   4 KB |   4 KB   | ##-------- | 20%  | N/A                |
const char msg_ramDiagnosticsRAM1_formatStringTData[]        ="| .TDAT         | 0x%08lX | 0x%08lX | %3u  KB | %3u  KB   |            |\r\n";
                                                           //  | .HEAP         | 0x2000E000 | 0x2000FFFF | 16 KB   | 8 KB      |            |
const char msg_ramDiagnosticsRAM1_formatStringHeap[]         ="| .HEAP         | 0x%08lX | 0x%08lX | %3u  KB | %3u  KB   |            |\r\n";
												           //  | .taskHandles  | 0x20010000 | 0x20013FFF | 16 KB   | 4 KB      |            |
const char msg_ramDiagnosticsRAM1_formatStringTaskHandles[]  ="| .taskHandles  | 0x%08lX | 0x%08lX | %3u  KB | %3u  KB   |            |\r\n";
												           //  | .taskStacks   | 0x20010000 | 0x20013FFF | 16 KB   | 4 KB      |            |
const char msg_ramDiagnosticsRAM1_formatStringTaskStacks[]   ="| .taskStacks   | 0x%08lX | 0x%08lX | %3u  KB | %3u  KB   |            |\r\n";
												           //  | .STACK        | 0x20010000 | 0x20013FFF | 16 KB   | 4 KB      |            |														
const char msg_ramDiagnosticsRAM1_formatStringStack[]        ="| .STACK        | 0x%08lX | 0x%08lX | %3u  KB | %3u  KB   |            |\r\n";
												           //  +---------------+--------+----------+------------+------+--------------------+
                                                           //  | FREE RAM TOTAL: 60 KB                                                      |                                             
const char msg_ramDiagnosticsRAM1_formatStringFreeRAM[]      ="| FREE RAM TOTAL: %3u KB                                                     |\r\n";      			
                                                           //  | Commands: s(snapshot) b(bank) q(quit)                                      |
														   //  +----------------------------------------------------------------------------+

const char msg_ramDiagnosticsRAM2_header1[]                  ="+---------------------------[ BANK RAM2 DETAILS ]----------------------------+\r\n";
                                                           //  | Section       | Start      | End        | Size    | Usage     |            |
                                                           //  +---------------+------------+------------+---------+-----------+------------+
                                                           //  | .ramDia       | 0x10000000 | 0x10004000 |  4 KB   |  4 KB     |            |
const char msg_ramDiagnosticsRAM2_formatStringRamDia[]       ="| .ramDia       | 0x%08lX | 0x%08lX | %3u  KB | %3u  KB   |            |\r\n";
                                                           //  | .sysDia       | 0x10000000 | 0x10004000 |  4 KB   |  4 KB     |            |
const char msg_ramDiagnosticsRAM2_formatStringSysDia[]       ="| .sysDia       | 0x%08lX | 0x%08lX | %3u  KB | %3u  KB   |            |\r\n";			  
                                                           //  +---------------+--------+----------+------------+------+--------------------+
                                                           //  | FREE RAM TOTAL: 60 KB                                                      |                                      
                                                           //  | Commands: s(snapshot) b(bank) q(quit)                                      |
														   //  +----------------------------------------------------------------------------+

const char msg_ramDiagnosticsCCSRAM_header1[]                ="+---------------------------[ BANK CCSRAM DETAILS ]--------------------------+\r\n";
                                                           //  | Section       | Start      | End        | Size    | Usage     |            |
                                                           //  +---------------+------------+------------+---------+-----------+------------+
const char msg_ramDiagnosticsCCSRAM_formatStringCrit[]       ="| .crit         | 0x%08lX | 0x%08lX | %3u  KB | %3u  KB   |            |\r\n";
const char msg_ramDiagnosticsCCSRAM_formatStringDmaBuff[]    ="| .dmaBuf       | 0x%08lX | 0x%08lX | %3u  KB | %3u  KB   |            |\r\n";			  
														   //  +---------------+--------+----------+------------+------+--------------------+
                                                           //  | FREE RAM TOTAL: 60 KB                                                      |                                      
                                                           //  | Commands: s(snapshot) b(bank) q(quit)                                      |
														   //  +----------------------------------------------------------------------------+

/**
 * @name Memeory diagnostics variables
 * These variables hold the current state of RAM usage and configuration, and are updated by the ramInfoRefresh() function.
 * They are placed in specific sections in RAM for easy access and organization.
 * @{
 */						
														   
static uint16_t ramDiagnosticsGeneral_total_size SECTION(".ramDiagnostics.uint16_t"); //!<  Total size of all RAM in kB
static uint8_t ramDiagnosticsRAM1_total_size SECTION(".ramDiagnostics.uint8_t"); //!< Total size of bank RAM1 in kB
static uint8_t ramDiagnosticsRAM2_total_size SECTION(".ramDiagnostics.uint8_t"); //!< Total size of bank RAM2 in kB
static uint8_t ramDiagnosticsCCSRAM_total_size SECTION(".ramDiagnostics.uint8_t"); //!< Total size of bank CCSRAM in kB

static uint8_t ramDiagnosticsRAM2_ramDiagnostics_size SECTION(".ramDiagnostics.uint8_t"); //!< Size of .ramDiagnostics section in RAM2
static uint8_t ramDiagnosticsCCSRAM_total_size SECTION(".ramDiagnostics.uint8_t"); //!< Total size of bank CCSRAM in kB
	
static uint16_t ramDiagnosticsGeneral_used SECTION(".ramDiagnostics.uint16_t"); //!< Total amount of used RAM memory kB
static uint8_t ramDiagnosticsRAM1_used SECTION(".ramDiagnostics.uint8_t"); //!< Amount of used memory in bank RAM1 kB
static uint8_t ramDiagnosticsRAM2_used SECTION(".ramDiagnostics.uint8_t"); //!< Amount of used memory in bank RAM2 kB
static uint8_t ramDiagnosticsCCSRAM_used SECTION(".ramDiagnostics.uint8_t"); //!< Amount of used memory in bank CCSRAM kB

static uint32_t ramDiagnosticsRAM1_lastMSP SECTION(".ramDiagnostics.uint32_t");  //!< Last value of Main Stack Pointer in RAM1
static uint32_t ramDiagnosticsRAM1_lastHeapEnd SECTION(".ramDiagnostics.uint32_t"); //!<  Last value of heap end pointer in RAM1
static uint8_t ramDiagnosticsRAM1_data_size SECTION(".ramDiagnostics.uint8_t"); //!< Size of .data section in RAM1
static uint8_t ramDiagnosticsRAM1_bss_size SECTION(".ramDiagnostics.uint8_t"); //!< Size of .bss section in RAM1
static uint8_t ramDiagnosticsRAM1_taskHandles_size SECTION(".ramDiagnostics.uint8_t"); //!< Size of .taskHandles section in RAM1
static uint8_t ramDiagnosticsRAM1_taskStacks_size SECTION(".ramDiagnostics.uint8_t"); //!< Size of .taskStacks section in RAM1
static uint8_t ramDiagnosticsRAM1_heap_size SECTION(".ramDiagnostics.uint8_t"); //!< Size of .heap section in RAM1
static uint8_t ramDiagnosticsRAM1_stack_size SECTION(".ramDiagnostics.uint8_t"); //!< Size of .stack section in RAM1

static uint8_t ramDiagnosticsRAM2_ramDiagnostics_size SECTION(".ramDiagnostics.uint8_t"); //!< Size of .ramDiagnostics section in RAM2
static uint8_t ramDiagnosticsRAM2_sysDiagnostics_size SECTION(".ramDiagnostics.uint8_t"); //!< Size of .sysDiagnostics section in RAM2

static uint8_t ramDiagnosticsCCSRAM_crit_size SECTION(".ramDiagnostics.uint8_t"); //!< Size of .crit section in CCSRAM
static uint8_t ramDiagnosticsCCSRAM_dmaBuff_size SECTION(".ramDiagnostics.uint8_t"); //!< Size of .dmaBuff section in CCSRAM

/** @} */

void ramInfoInit(void){
	ramDiagnosticsGeneral_total_size=0;
	ramDiagnosticsRAM1_total_size=0;
	ramDiagnosticsRAM2_total_size=0;
	ramDiagnosticsCCSRAM_total_size=0;

	ramDiagnosticsGeneral_used=0;
	ramDiagnosticsRAM1_used=0;
	ramDiagnosticsRAM2_used=0;
	ramDiagnosticsCCSRAM_used=0;

	ramDiagnosticsRAM1_lastMSP=0;
	ramDiagnosticsRAM1_lastHeapEnd=0;
	ramDiagnosticsRAM1_data_size=0;
	ramDiagnosticsRAM1_bss_size=0;
	ramDiagnosticsRAM1_taskHandles_size=0;
	ramDiagnosticsRAM1_taskStacks_size=0;
	ramDiagnosticsRAM1_heap_size=0;
	ramDiagnosticsRAM1_stack_size=0;

	ramDiagnosticsRAM2_ramDiagnostics_size=0;
	ramDiagnosticsRAM2_sysDiagnostics_size=0;

	ramDiagnosticsCCSRAM_crit_size=0;
	ramDiagnosticsCCSRAM_dmaBuff_size=0;

	ramDiagnosticsRAM1_total_size=((uint32_t)&__RAM1_end__-(uint32_t)&__RAM1_start__)/1024;
	ramDiagnosticsRAM2_total_size=((uint32_t)&__RAM2_end__-(uint32_t)&__RAM2_start__)/1024;
	ramDiagnosticsCCSRAM_total_size=((uint32_t)&__CCSRAM_end__-(uint32_t)&__CCSRAM_start__)/1024;
	ramDiagnosticsGeneral_total_size=ramDiagnosticsRAM1_total_size+ramDiagnosticsRAM2_total_size+ramDiagnosticsCCSRAM_total_size;

	ramDiagnosticsRAM1_data_size=(((uint32_t)&_edata-(uint32_t)&__RAM1_start__)/1024);
	ramDiagnosticsRAM1_bss_size=(((uint32_t)&__bss_end__-(uint32_t)&__bss_start__)/1024);
	ramDiagnosticsRAM1_taskHandles_size=(((uint32_t)&__TASK_HANDLES_END__-(uint32_t)&__TASK_HANDLES_START__)/1024);
	ramDiagnosticsRAM1_taskStacks_size=(((uint32_t)&__TASK_STACKS_END__-(uint32_t)&__TASK_STACKS_START__)/1024);

	ramDiagnosticsRAM2_ramDiagnostics_size=((uint32_t)&__RAM_DIAGNOSTICS_END__-(uint32_t)&__RAM_DIAGNOSTICS_START__)/1024;
	ramDiagnosticsRAM2_sysDiagnostics_size=((uint32_t)&__SYS_DIAGNOSTICS_END__-(uint32_t)&__SYS_DIAGNOSTICS_END__)/1024;

	ramDiagnosticsCCSRAM_crit_size=((uint32_t)&__CRIT_END__-(uint32_t)&__CRIT_START__)/1024;
	ramDiagnosticsCCSRAM_dmaBuff_size=((uint32_t)&__DMA_END__-(uint32_t)&__DMA_START__)/1024;

	ramInfoRefresh();
}
void ramInfoRefresh(){
// RAM1 heap usage
	if(__sbrk_heap_end==NULL){
		ramDiagnosticsRAM1_lastHeapEnd=(uint32_t)&_end;
	}
	else{
		ramDiagnosticsRAM1_lastHeapEnd=(uint32_t)__sbrk_heap_end;
	}

// RAM1 usage
	ramDiagnosticsRAM1_lastMSP=__get_MSP();
	ramDiagnosticsRAM1_used=(((uint32_t)&__RAM1_end__-ramDiagnosticsRAM1_lastMSP)+(ramDiagnosticsRAM1_lastHeapEnd-(uint32_t)&__RAM1_start__))/1024;
// RAM2 usage
	ramDiagnosticsRAM2_used=((uint32_t)&__RAM_DIAGNOSTICS_END__-(uint32_t)&__RAM2_start__)/1024;
// CCSRAM usage
	ramDiagnosticsCCSRAM_used=((uint32_t)&__DMA_END__-(uint32_t)&__CCSRAM_start__)/1024;
// General RAM usage
	ramDiagnosticsGeneral_used=ramDiagnosticsRAM1_used+ramDiagnosticsRAM2_used+ramDiagnosticsCCSRAM_used;
// RAM1 .heap section usage
	ramDiagnosticsRAM1_heap_size=((uint32_t)ramDiagnosticsRAM1_lastHeapEnd-(uint32_t)&_end)/1024;
// RAM1 .stack section usage
	ramDiagnosticsRAM1_stack_size=((uint32_t)&__RAM1_end__-ramDiagnosticsRAM1_lastMSP)/1024;
// CCSRAM .crit section usage
	ramDiagnosticsCCSRAM_crit_size=((uint32_t)&__CRIT_END__-(uint32_t)&__CRIT_START__)/1024;
// CCSRAM .dmaBuff section usage
	ramDiagnosticsCCSRAM_crit_size=((uint32_t)&__DMA_END__-(uint32_t)&__DMA_END__)/1024;
}
														
void ramInfoGeneral(UINT(*pSleepFn)(ULONG timeout),ULONG sleepTimeout){
	char buffer[MEMINFO_LINE_BUFFER_SIZE]={0};
	char bar_buffer[MEMINFO_BAR_BUFFER_SIZE]={0};

	uint8_t usage_percent=0; // Used for bar graph calculation
// Send General RAM diagnostics headers 1-3
	while(usart1_dma_enq_data((uint8_t*)msg_ramDiagnosticsGeneral_header1,strlen(msg_ramDiagnosticsGeneral_header1))!=true){
		pSleepFn(sleepTimeout);
	}
	while(usart1_dma_enq_data((uint8_t*)msg_ramDiagnosticsGeneral_header2,strlen(msg_ramDiagnosticsGeneral_header2))!=true){
		pSleepFn(sleepTimeout);
	}
	while(usart1_dma_enq_data((uint8_t*)msg_ramDiagnosticsGeneral_header3,strlen(msg_ramDiagnosticsGeneral_header3))!=true){
		pSleepFn(sleepTimeout);
	}
// Send RAM1 info
	usage_percent=((uint16_t)ramDiagnosticsRAM1_used*100)/ramDiagnosticsRAM1_total_size;
	memset(bar_buffer,'-',MEMINFO_BAR_BUFFER_SIZE-1);
	bar_buffer[MEMINFO_BAR_BUFFER_SIZE-1]='\0';

	if(usage_percent>100){
		g_SystemErrors.system_error=ERROR_MEMINFO_RAM1_USAGE_OVERFLOW;
		return;
	}

	memset(bar_buffer,'#',(usage_percent*MEMINFO_BAR_BUFFER_SIZE)/100);
	bar_buffer[MEMINFO_BAR_BUFFER_SIZE-1]='\0';
	snprintf(buffer,MEMINFO_LINE_BUFFER_SIZE,msg_ramDiagnosticsGeneral_formatStringRAM1,
		(uint32_t)&__RAM1_start__,	   // RAM1 start
		(uint32_t)&__RAM1_end__,       // RAM1 end
		ramDiagnosticsRAM1_total_size, // RAM1 size in KB
		bar_buffer,                    // RAM1 usage bar
		usage_percent                  // RAM1 usage percent
	);
	while(usart1_dma_enq_data((uint8_t*)buffer,strlen(buffer))!=true){
		pSleepFn(sleepTimeout);
	}
// Send RAM2 info
	usage_percent=((uint16_t)ramDiagnosticsRAM2_used*100)/ramDiagnosticsRAM2_total_size;
	memset(bar_buffer,'-',MEMINFO_BAR_BUFFER_SIZE-1);
	bar_buffer[MEMINFO_BAR_BUFFER_SIZE-1]='\0';

	if(usage_percent>100){
		g_SystemErrors.system_error=ERROR_MEMINFO_RAM2_USAGE_OVERFLOW;
		return;
	}

	memset(bar_buffer,'#',(usage_percent*MEMINFO_BAR_BUFFER_SIZE)/100);
	bar_buffer[MEMINFO_BAR_BUFFER_SIZE-1]='\0';
	snprintf(buffer,MEMINFO_LINE_BUFFER_SIZE,msg_ramDiagnosticsGeneral_formatStringRAM2,
		(uint32_t)&__RAM2_start__,	   // RAM2 start
		(uint32_t)&__RAM2_end__,       // RAM2 end
		ramDiagnosticsRAM2_total_size, // RAM2 size in KB
		bar_buffer,                    // RAM2 usage bar
		usage_percent                  // RAM2 usage percent
	);
	while(usart1_dma_enq_data((uint8_t*)buffer,strlen(buffer))!=true){
		pSleepFn(sleepTimeout);
	}
// Send CCSRAM info
	usage_percent=((uint16_t)ramDiagnosticsCCSRAM_used*100)/ramDiagnosticsCCSRAM_total_size;
	memset(bar_buffer,'-',MEMINFO_BAR_BUFFER_SIZE-1);
	bar_buffer[MEMINFO_BAR_BUFFER_SIZE-1]='\0';
	if(usage_percent>100){
		g_SystemErrors.system_error=ERROR_MEMINFO_CCSRAM_USAGE_OVERFLOW;
		return;
	}
	memset(bar_buffer,'#',(usage_percent*MEMINFO_BAR_BUFFER_SIZE)/100);
	bar_buffer[MEMINFO_BAR_BUFFER_SIZE-1]='\0';
	snprintf(buffer,MEMINFO_LINE_BUFFER_SIZE,msg_ramDiagnosticsGeneral_formatStringCCSRAM,
		(uint32_t)&__CCSRAM_start__,	   // CCSRAM start
		(uint32_t)&__CCSRAM_end__,       // CCSRAM end
		ramDiagnosticsCCSRAM_total_size, // CCSRAM size in KB
		bar_buffer,                      // CCSRAM usage bar
		usage_percent                    // CCSRAM usage percent
	);
	while(usart1_dma_enq_data((uint8_t*)buffer,strlen(buffer))!=true){
		pSleepFn(sleepTimeout);
	}
// Send RAM diagnostics header 4
	while(usart1_dma_enq_data((uint8_t*)msg_ramDiagnosticsGeneral_header3,strlen(msg_ramDiagnosticsGeneral_header3))!=true){
		pSleepFn(sleepTimeout);
	}
// Send Free RAM total
	snprintf(buffer,MEMINFO_LINE_BUFFER_SIZE,msg_ramDiagnosticsGeneral_formatStringFreeRAM,ramDiagnosticsGeneral_total_size-ramDiagnosticsGeneral_used, ramDiagnosticsGeneral_total_size);
	while(usart1_dma_enq_data((uint8_t*)buffer,strlen(buffer))!=true){
		pSleepFn(sleepTimeout);
	}
// Send RAM diagnostics footers
	while(usart1_dma_enq_data((uint8_t*)msg_ramDiagnosticsGeneral_footer1,strlen(msg_ramDiagnosticsGeneral_footer1))!=true){
		pSleepFn(sleepTimeout);
	}
	while(usart1_dma_enq_data((uint8_t*)msg_ramDiagnosticsGeneral_footer2,strlen(msg_ramDiagnosticsGeneral_footer2))!=true){
		pSleepFn(sleepTimeout);
	}
// Send empty line
	while(usart1_dma_enq_data((uint8_t*)"\r\n",2)!=true){
		pSleepFn(sleepTimeout);
	}
}

void ramInfoRAM1(UINT(*pSleepFn)(ULONG timeout),ULONG sleepTimeout){
	char buffer[MEMINFO_LINE_BUFFER_SIZE]={0};
// Send RAM1 diagnostics headers 1-3
	while(usart1_dma_enq_data((uint8_t*)msg_ramDiagnosticsRAM1_header1,strlen(msg_ramDiagnosticsRAM1_header1))!=true){
		pSleepFn(sleepTimeout);
	}
	while(usart1_dma_enq_data((uint8_t*)msg_ramDiagnosticsRAM1_header2,strlen(msg_ramDiagnosticsRAM1_header2))!=true){
		pSleepFn(sleepTimeout);
	}
	while(usart1_dma_enq_data((uint8_t*)msg_ramDiagnosticsRAM1_header3,strlen(msg_ramDiagnosticsRAM1_header3))!=true){
		pSleepFn(sleepTimeout);
	}
// Send .data section info
	snprintf(buffer,MEMINFO_LINE_BUFFER_SIZE,msg_ramDiagnosticsRAM1_formatStringData,
		(uint32_t)&__RAM1_start__,	    // .data start
		(uint32_t)&_edata,              // .data end
		ramDiagnosticsRAM1_data_size,   // .data size in KB
		ramDiagnosticsRAM1_data_size    // .data used size in KB
	);
	while(usart1_dma_enq_data((uint8_t*)buffer,strlen(buffer))!=true){
		pSleepFn(sleepTimeout);
	}
// Send .bss section info
	snprintf(buffer,MEMINFO_LINE_BUFFER_SIZE,msg_ramDiagnosticsRAM1_formatStringBSS,
		(uint32_t)&__bss_start__,		 // .bss start
		(uint32_t)&__bss_end__,          // .bss end
		ramDiagnosticsRAM1_bss_size,     // .bss size in KB
		ramDiagnosticsRAM1_bss_size      // .bss used size in KB
	);
	while(usart1_dma_enq_data((uint8_t*)buffer,strlen(buffer))!=true){
		pSleepFn(sleepTimeout);
	}
// Send .taskHandles section info
	snprintf(buffer,MEMINFO_LINE_BUFFER_SIZE,msg_ramDiagnosticsRAM1_formatStringTaskHandles,
		(uint32_t)&__TASK_HANDLES_START__,	     // .taskHandles start
		(uint32_t)&__TASK_HANDLES_END__,          // .taskHandles end
		ramDiagnosticsRAM1_taskHandles_size,     // .taskHandles size in KB
		ramDiagnosticsRAM1_taskHandles_size      // .taskHandles used size in KB
	);
	while(usart1_dma_enq_data((uint8_t*)buffer,strlen(buffer))!=true){
		pSleepFn(sleepTimeout);
	}
// Send .taskStacks section info
	snprintf(buffer,MEMINFO_LINE_BUFFER_SIZE,msg_ramDiagnosticsRAM1_formatStringTaskStacks,
		(uint32_t)&__TASK_STACKS_START__,	     // .taskStacks start
		(uint32_t)&__TASK_STACKS_END__,          // .taskStacks end
		ramDiagnosticsRAM1_taskStacks_size,      // .taskStacks size in KB
		ramDiagnosticsRAM1_taskStacks_size       // .taskStacks used size in KB
	);
	while(usart1_dma_enq_data((uint8_t*)buffer,strlen(buffer))!=true){
		pSleepFn(sleepTimeout);
	}
// Send .heap section info
	snprintf(buffer,MEMINFO_LINE_BUFFER_SIZE,msg_ramDiagnosticsRAM1_formatStringHeap,
		(uint32_t)&_end,                          // .heap start
		(uint32_t)ramDiagnosticsRAM1_lastHeapEnd, // .heap end
		ramDiagnosticsRAM1_heap_size,             // .heap size in KB
		ramDiagnosticsRAM1_heap_size              // .heap used size in KB
	);
	while(usart1_dma_enq_data((uint8_t*)buffer,strlen(buffer))!=true){
		pSleepFn(sleepTimeout);
	}
// Send .stack section info
	snprintf(buffer,MEMINFO_LINE_BUFFER_SIZE,msg_ramDiagnosticsRAM1_formatStringStack,
		(uint32_t)&__RAM1_end__,              // .stack start
		(uint32_t)ramDiagnosticsRAM1_lastMSP, // .stack end
		ramDiagnosticsRAM1_stack_size,        // .stack size in KB
		ramDiagnosticsRAM1_stack_size         // .stack used size in KB
	);
	while(usart1_dma_enq_data((uint8_t*)buffer,strlen(buffer))!=true){
		pSleepFn(sleepTimeout);
	}
// Send RAM1 diagnostics footers
	while(usart1_dma_enq_data((uint8_t*)msg_ramDiagnosticsRAM1_header3,strlen(msg_ramDiagnosticsRAM1_header3))!=true){
		pSleepFn(sleepTimeout);
	}
// Send Free RAM1 total
	snprintf(buffer,MEMINFO_LINE_BUFFER_SIZE,msg_ramDiagnosticsRAM1_formatStringFreeRAM,ramDiagnosticsRAM1_total_size-ramDiagnosticsRAM1_used);
	while(usart1_dma_enq_data((uint8_t*)buffer,strlen(buffer))!=true){
		pSleepFn(sleepTimeout);
	}
// Send RAM diagnostics footers
	while(usart1_dma_enq_data((uint8_t*)msg_ramDiagnosticsGeneral_footer1,strlen(msg_ramDiagnosticsGeneral_footer1))!=true){
		pSleepFn(sleepTimeout);
	}
	while(usart1_dma_enq_data((uint8_t*)msg_ramDiagnosticsGeneral_footer2,strlen(msg_ramDiagnosticsGeneral_footer2))!=true){
		pSleepFn(sleepTimeout);
	}
}

void ramInfoRAM2(UINT(*pSleepFn)(ULONG timeout),ULONG sleepTimeout){
	char buffer[MEMINFO_LINE_BUFFER_SIZE]={0};
// Send RAM2 diagnostics headers 1-3
	while(usart1_dma_enq_data((uint8_t*)msg_ramDiagnosticsRAM2_header1,strlen(msg_ramDiagnosticsRAM2_header1))!=true){
		pSleepFn(sleepTimeout);
	}
	while(usart1_dma_enq_data((uint8_t*)msg_ramDiagnosticsRAM1_header2,strlen(msg_ramDiagnosticsRAM1_header2))!=true){
		pSleepFn(sleepTimeout);
	}
	while(usart1_dma_enq_data((uint8_t*)msg_ramDiagnosticsRAM1_header3,strlen(msg_ramDiagnosticsRAM1_header3))!=true){
		pSleepFn(sleepTimeout);
	}
// Send .ramDiagnostics section info
	snprintf(buffer,MEMINFO_LINE_BUFFER_SIZE,msg_ramDiagnosticsRAM2_formatStringRamDia,
		(uint32_t)&__RAM_DIAGNOSTICS_START__,	     // .ramDiagnostics start
		(uint32_t)&__RAM_DIAGNOSTICS_END__,          // .ramDiagnostics end
		ramDiagnosticsRAM2_ramDiagnostics_size,      // .ramDiagnostics size in KB
		ramDiagnosticsRAM2_ramDiagnostics_size       // .ramDiagnostics used size in KB
	);
	while(usart1_dma_enq_data((uint8_t*)buffer,strlen(buffer))!=true){
		pSleepFn(sleepTimeout);
	}
// Send .sysDiag section info
	snprintf(buffer,MEMINFO_LINE_BUFFER_SIZE,msg_ramDiagnosticsRAM2_formatStringSysDia,
		(uint32_t)&__SYS_DIAGNOSTICS_START__,        // .sysDiag start
		(uint32_t)&__SYS_DIAGNOSTICS_END__,          // .sysDiag end
		ramDiagnosticsRAM2_sysDiagnostics_size,      // .sysDiag size in KB
		ramDiagnosticsRAM2_sysDiagnostics_size       // .sysDiag size in KB
	);
	while(usart1_dma_enq_data((uint8_t*)buffer,strlen(buffer))!=true){
		pSleepFn(sleepTimeout);
	}
// Send RAM2 diagnostics footers
	while(usart1_dma_enq_data((uint8_t*)msg_ramDiagnosticsRAM1_header3,strlen(msg_ramDiagnosticsRAM1_header3))!=true){
		pSleepFn(sleepTimeout);
	}
// Send Free RAM2 total
	snprintf(buffer,MEMINFO_LINE_BUFFER_SIZE,msg_ramDiagnosticsRAM1_formatStringFreeRAM,ramDiagnosticsRAM2_total_size-ramDiagnosticsRAM2_used);
	while(usart1_dma_enq_data((uint8_t*)buffer,strlen(buffer))!=true){
		pSleepFn(sleepTimeout);
	}
// Send RAM diagnostics footers
	while(usart1_dma_enq_data((uint8_t*)msg_ramDiagnosticsGeneral_footer1,strlen(msg_ramDiagnosticsGeneral_footer1))!=true){
		pSleepFn(sleepTimeout);
	}
	while(usart1_dma_enq_data((uint8_t*)msg_ramDiagnosticsGeneral_footer2,strlen(msg_ramDiagnosticsGeneral_footer2))!=true){
		pSleepFn(sleepTimeout);
	}
}

void ramInfoCCSRAM(UINT(*pSleepFn)(ULONG timeout),ULONG sleepTimeout){
	char buffer[MEMINFO_LINE_BUFFER_SIZE]={0};

// Send CCSRAM diagnostics headers 1-3
	while(usart1_dma_enq_data((uint8_t*)msg_ramDiagnosticsCCSRAM_header1,strlen(msg_ramDiagnosticsCCSRAM_header1))!=true){
		pSleepFn(sleepTimeout);
	}
	while(usart1_dma_enq_data((uint8_t*)msg_ramDiagnosticsRAM1_header2,strlen(msg_ramDiagnosticsRAM1_header2))!=true){
		pSleepFn(sleepTimeout);
	}
	while(usart1_dma_enq_data((uint8_t*)msg_ramDiagnosticsRAM1_header3,strlen(msg_ramDiagnosticsRAM1_header3))!=true){
		pSleepFn(sleepTimeout);
	}
// Send .crit section info
	snprintf(buffer,MEMINFO_LINE_BUFFER_SIZE,msg_ramDiagnosticsCCSRAM_formatStringCrit,
		(uint32_t)&__CRIT_START__,	      // .crit start
		(uint32_t)&__CRIT_END__,          // .crit end
		ramDiagnosticsCCSRAM_crit_size,   // .crit size in KB
		ramDiagnosticsCCSRAM_crit_size    // .crit used size in KB
	);
	while(usart1_dma_enq_data((uint8_t*)buffer,strlen(buffer))!=true){
		pSleepFn(sleepTimeout);
	}
// Send .dmaBuff section info
	snprintf(buffer,MEMINFO_LINE_BUFFER_SIZE,msg_ramDiagnosticsCCSRAM_formatStringDmaBuff,
		(uint32_t)&__DMA_START__,	      // .dmaBuff start
		(uint32_t)&__DMA_END__,           // .dmaBuff end
		ramDiagnosticsCCSRAM_dmaBuff_size,   // .dmaBuff size in KB
		ramDiagnosticsCCSRAM_dmaBuff_size    // .dmaBuff used size in KB
	);
	while(usart1_dma_enq_data((uint8_t*)buffer,strlen(buffer))!=true){
		pSleepFn(sleepTimeout);
	}
// Send CCSRAM diagnostics footers
	while(usart1_dma_enq_data((uint8_t*)msg_ramDiagnosticsRAM1_header3,strlen(msg_ramDiagnosticsRAM1_header3))!=true){
		pSleepFn(sleepTimeout);
	}
// Send Free CCSRAM total
	snprintf(buffer,MEMINFO_LINE_BUFFER_SIZE,msg_ramDiagnosticsRAM1_formatStringFreeRAM,ramDiagnosticsCCSRAM_total_size-ramDiagnosticsCCSRAM_used);
	while(usart1_dma_enq_data((uint8_t*)buffer,strlen(buffer))!=true){
		pSleepFn(sleepTimeout);
	}
// Send RAM diagnostics footers
	while(usart1_dma_enq_data((uint8_t*)msg_ramDiagnosticsGeneral_footer1,strlen(msg_ramDiagnosticsGeneral_footer1))!=true){
		pSleepFn(sleepTimeout);
	}
	while(usart1_dma_enq_data((uint8_t*)msg_ramDiagnosticsGeneral_footer2,strlen(msg_ramDiagnosticsGeneral_footer2))!=true){
		pSleepFn(sleepTimeout);
	}
}

/** @} */