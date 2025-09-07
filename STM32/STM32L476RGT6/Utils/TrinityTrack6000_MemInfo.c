#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stm32l4xx_hal.h>
#include <core_cm4.h>

#include <TrinityTrack6000_MemInfo.h>

extern uint32_t __RAM1_start__; // Defined in the linker script by me for RAM1 start
extern uint32_t __RAM1_end__;   // Defined in the linker script by me for RAM1 end

extern uint32_t _edata;
extern uint32_t __bss_start__;
extern uint32_t __bss_end__;
extern uint32_t _end; // Start of heap as heap grows upwards
extern uint32_t _heap_start; // Defined in the linker script by me for heap start

extern uint32_t __RAM2_start__; // Defined in the linker script by me for RAM2 start
extern uint32_t __RAM2_end__; // Defined in the linker script by me for RAM2 end

extern uint32_t __RAM_DIAGNOSTICS_END__; // Defined in the linker script by me for end of ramDiagnostics section in RAM2

extern uint8_t* __sbrk_heap_end; // Defined in sysmem.c

extern UART_HandleTypeDef uart;

extern void Error_Handler(void);

const char msg_ramDiagnosticsGeneral_header1[]            ="+-------------------------[ RAM DIAGNOSTICS ]--------------------------+\r\n";
const char msg_ramDiagnosticsGeneral_header2[]            ="| Bank   | Start      | End        | Size    | Usage      | Used       |\r\n"; 
const char msg_ramDiagnosticsGeneral_header3[]            ="+--------+------------+------------+---------+------------+------------+\r\n";
                                                        //  | RAM1   | 0x20000000 | 0x2001FFFF | 128 KB  | ########## | 80%        |
const char msg_ramDiagnosticsGeneral_formatStringRAM1[]   ="│ RAM1   │ 0x%08lX │ 0x%08lX │ %3u  KB │%11s │ %3u%%       │\r\n";			  
								                        //  | RAM2   | 0x20020000 | 0x2003FFFF |  64 KB  | ####------| 40%         |
const char msg_ramDiagnosticsGeneral_formatStringRAM2[]   ="│ RAM2   │ 0x%08lX │ 0x%08lX │ %3u  KB │%11s │ %3u%%       │\r\n";
								                        //  | CCSRAM | 0x10000000 | 0x10003FFF |  16 KB  | ##--------| 20%         |
const char msg_ramDiagnosticsGeneral_formatStringCCSRAM[] ="│ CCSRAM │ 0x%08lX │ 0x%08lX │ %3u  KB │%11s │ %3u%%       │\r\n";
                                                        //  +--------+------------+------------+---------+-----------+-------------+
                                                        //  | FREE RAM TOTAL: 600 KB                                               |
const char msg_ramDiagnosticsGeneral_formatStringFreeRAM[]="│ FREE RAM TOTAL: %3u KB                                               │\r\n";
const char msg_ramDiagnosticsGeneral_footer1[]            ="| Commands: s(snapshot) b(bank) q(quit)                                |\r\n"; 
const char msg_ramDiagnosticsGeneral_footer2[]            ="+----------------------------------------------------------------------+\r\n";       	

const char msg_ramDiagnosticsRAM1_header1[]               ="+------------------------[ BANK RAM1 DETAILS ]-------------------------+\r\n";
const char msg_ramDiagnosticsRAM1_header2[]               ="| Section | Start      | End        | Size    | Usage     |            |\r\n"; 
const char msg_ramDiagnosticsRAM1_header3[]               ="+---------+------------+------------+---------+-----------+------------+\r\n";
                                                        //  | .DATA   | 0x20000000 | 0x20007FFF | 32 KB   | 32 KB     |            |
const char msg_ramDiagnosticsRAM1_formatStringData[]      ="| .DATA   | 0x%08lX | 0x%08lX | %3u  KB | %3u  KB   |            |\r\n";			  
												        //  | .BSS    | 0x20008000 | 0x2000DFFF | 24 KB   | 18 KB     |            |
const char msg_ramDiagnosticsRAM1_formatStringBSS[]       ="| .BSS    | 0x%08lX | 0x%08lX | %3u  KB | %3u  KB   |            |\r\n";
												        //  | .TDAT   |   4 KB |   4 KB   | ##-------- | 20%  | N/A                |
const char msg_ramDiagnosticsRAM1_formatStringTData[]     ="| .TDAT   | 0x%08lX | 0x%08lX | %3u  KB | %3u  KB   |            |\r\n";
                                                        //  | .HEAP   | 0x2000E000 | 0x2000FFFF | 16 KB   | 8 KB      |            |
const char msg_ramDiagnosticsRAM1_formatStringHeap[]      ="| .HEAP   | 0x%08lX | 0x%08lX | %3u  KB | %3u  KB   |            |\r\n";
												        //  | .STACK  | 0x20010000 | 0x20013FFF | 16 KB   | 4 KB      |            |
const char msg_ramDiagnosticsRAM1_formatStringStack[]     ="| .STACK  | 0x%08lX | 0x%08lX | %3u  KB | %3u  KB   |            |\r\n";
												        //  +---------+--------+----------+------------+------+--------------------+
                                                        //  | FREE RAM TOTAL: 60 KB                                                |                                             
const char msg_ramDiagnosticsRAM1_formatStringFreeRAM[]   ="| FREE RAM TOTAL: %3u KB                                               |\r\n";      			
                                                        //  | Commands: s(snapshot) b(bank) q(quit)                                |
														//  +----------------------------------------------------------------------+

const char msg_ramDiagnosticsRAM2_header1[]               ="+------------------------[ BANK RAM2 DETAILS ]-------------------------+\r\n";
                                                        //  | Section | Start      | End        | Size    | Usage     |            |
                                                        //  +---------+------------+------------+---------+-----------+------------+
                                                        //  | .ramDia | 0x10000000 | 0x10004000 |  4 KB   |  4 KB     |            |
const char msg_ramDiagnosticsRAM2_formatStringRamDia[]    ="| .ramDia | 0x%08lX | 0x%08lX | %3u  KB | %3u  KB   |            |\r\n";			  
                                                        //  +---------+--------+----------+------------+------+--------------------+
                                                        //  | FREE RAM TOTAL: 60 KB                                                |                                      
                                                        //  | Commands: s(snapshot) b(bank) q(quit)                                |
														//  +----------------------------------------------------------------------+

uint16_t ramDiagnosticsGeneral_total_size=0;
uint8_t ramDiagnosticsRAM1_total_size=0;
uint8_t ramDiagnosticsRAM2_total_size=0;
uint8_t ramDiagnosticsCCSRAM_total_size=0;
	
uint16_t ramDiagnosticsGeneral_used=0;
uint8_t ramDiagnosticsRAM1_used=0;
uint8_t ramDiagnosticsRAM2_used=0;
uint8_t ramDiagnosticsCCSRAM_used=0;

uint32_t ramDiagnosticsRAM1_lastMSP=0;
uint32_t ramDiagnosticsRAM1_lastHeapEnd=0;
uint8_t ramDiagnosticsRAM1_data_size=0;
uint8_t ramDiagnosticsRAM1_bss_size=0;
uint8_t ramDiagnosticsRAM1_tdat_size=0;
uint8_t ramDiagnosticsRAM1_heap_size=0;
uint8_t ramDiagnosticsRAM1_stack_size=0;

uint8_t ramDiagnosticsRAM2_ramDiagnostics_size=0;

void ramDiagnositcsInit(void){
	ramDiagnosticsRAM1_total_size=((uint32_t)&__RAM1_end__-(uint32_t)&__RAM1_start__)/1024;
	ramDiagnosticsRAM2_total_size=((uint32_t)&__RAM2_end__-(uint32_t)&__RAM2_start__)/1024;
	ramDiagnosticsCCSRAM_total_size=0; // Not applicable in this MCU
	ramDiagnosticsGeneral_total_size=ramDiagnosticsRAM1_total_size+ramDiagnosticsRAM2_total_size+ramDiagnosticsCCSRAM_total_size;

	ramDiagnosticsRAM1_data_size=((&_edata-(uint32_t*)&__RAM1_start__)/1024);
	ramDiagnosticsRAM1_bss_size=((&__bss_end__-(uint32_t*)&__bss_start__)/1024);
	ramDiagnosticsRAM1_tdat_size=0; // Will be implemented after adding ThreadX to the project

	ramDiagnosticsRAM2_ramDiagnostics_size=((uint32_t)&__RAM_DIAGNOSTICS_END__-(uint32_t)&__RAM2_start__)/1024;

	ramDiagnosticsRefresh();
}
void ramDiagnosticsRefresh(){
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
	// Not applicable in this MCU
// General RAM usage
	ramDiagnosticsGeneral_used=ramDiagnosticsRAM1_used+ramDiagnosticsRAM2_used+ramDiagnosticsCCSRAM_used;
// RAM1 .heap section usage
	ramDiagnosticsRAM1_heap_size=((uint32_t)ramDiagnosticsRAM1_lastHeapEnd-(uint32_t)&_end)/1024;
// RAM1 .stack section usage
	ramDiagnosticsRAM1_stack_size=((uint32_t)&__RAM1_end__-ramDiagnosticsRAM1_lastMSP)/1024;
//
}
														
void ramDiagnosticsGeneral(){
	char buffer[MEMINFO_LINE_BUFFER_SIZE]={0};
	char bar_buffer[MEMINFO_BAR_BUFFER_SIZE]={0};

	uint8_t usage_percent=0; // Used for bar graph calculation
// Send General RAM diagnostics headers 1-3
	HAL_UART_Transmit(&uart,(uint8_t*)msg_ramDiagnosticsGeneral_header1,strlen(msg_ramDiagnosticsGeneral_header1),MEMINFO_UART_TIMEOUT);
	HAL_UART_Transmit(&uart,(uint8_t*)msg_ramDiagnosticsGeneral_header2,strlen(msg_ramDiagnosticsGeneral_header2),MEMINFO_UART_TIMEOUT);
	HAL_UART_Transmit(&uart,(uint8_t*)msg_ramDiagnosticsGeneral_header3,strlen(msg_ramDiagnosticsGeneral_header3),MEMINFO_UART_TIMEOUT);
// Send RAM1 info
	usage_percent=((uint16_t)ramDiagnosticsRAM1_used*100)/ramDiagnosticsRAM1_total_size;
	memset(bar_buffer,'-',MEMINFO_BAR_BUFFER_SIZE-1);
	bar_buffer[MEMINFO_BAR_BUFFER_SIZE-1]='\0';

	if(usage_percent>100){
		// Add error code here
		Error_Handler(); // this should never happen
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
	HAL_UART_Transmit(&uart,(uint8_t*)buffer,strlen(buffer),MEMINFO_UART_TIMEOUT);
// Send RAM2 info
	usage_percent=((uint16_t)ramDiagnosticsRAM2_used*100)/ramDiagnosticsRAM2_total_size;
	memset(bar_buffer,'-',MEMINFO_BAR_BUFFER_SIZE-1);
	bar_buffer[MEMINFO_BAR_BUFFER_SIZE-1]='\0';

	if(usage_percent>100){
		// Add error code here
		Error_Handler(); // this should never happen
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
	HAL_UART_Transmit(&uart,(uint8_t*)buffer,strlen(buffer),MEMINFO_UART_TIMEOUT);
// Send CCSRAM info
	// Not applicable in this MCU
	// Will be added in production project with STM32G473
// Send RAM diagnostics header 4
	HAL_UART_Transmit(&uart,(uint8_t*)msg_ramDiagnosticsGeneral_header3,strlen(msg_ramDiagnosticsGeneral_header3),MEMINFO_UART_TIMEOUT);
// Send Free RAM total
	snprintf(buffer,MEMINFO_LINE_BUFFER_SIZE,msg_ramDiagnosticsGeneral_formatStringFreeRAM,ramDiagnosticsGeneral_total_size-ramDiagnosticsGeneral_used);
	HAL_UART_Transmit(&uart,(uint8_t*)buffer,strlen(buffer),MEMINFO_UART_TIMEOUT);
// Send RAM diagnostics footers
	HAL_UART_Transmit(&uart,(uint8_t*)msg_ramDiagnosticsGeneral_footer1,strlen(msg_ramDiagnosticsGeneral_footer1),MEMINFO_UART_TIMEOUT);
	HAL_UART_Transmit(&uart,(uint8_t*)msg_ramDiagnosticsGeneral_footer2,strlen(msg_ramDiagnosticsGeneral_footer2),MEMINFO_UART_TIMEOUT);
// Send empty line
	HAL_UART_Transmit(&uart,(uint8_t*)"\r\n",2,MEMINFO_UART_TIMEOUT);	
}

void ramDiagnosticsRAM1(){
	char buffer[MEMINFO_LINE_BUFFER_SIZE]={0};

// Send RAM1 diagnostics headers 1-3	
	HAL_UART_Transmit(&uart,(uint8_t*)msg_ramDiagnosticsRAM1_header1,strlen(msg_ramDiagnosticsRAM1_header1),MEMINFO_UART_TIMEOUT);
	HAL_UART_Transmit(&uart,(uint8_t*)msg_ramDiagnosticsRAM1_header2,strlen(msg_ramDiagnosticsRAM1_header2),MEMINFO_UART_TIMEOUT);
	HAL_UART_Transmit(&uart,(uint8_t*)msg_ramDiagnosticsRAM1_header3,strlen(msg_ramDiagnosticsRAM1_header3),MEMINFO_UART_TIMEOUT);
// Send .data section info
	snprintf(buffer,MEMINFO_LINE_BUFFER_SIZE,msg_ramDiagnosticsRAM1_formatStringData,
		(uint32_t)&__RAM1_start__,	    // .data start
		(uint32_t)&_edata,              // .data end
		ramDiagnosticsRAM1_data_size,   // .data size in KB
		ramDiagnosticsRAM1_data_size    // .data used size in KB
	);
	HAL_UART_Transmit(&uart,(uint8_t*)buffer,strlen(buffer),MEMINFO_UART_TIMEOUT);
// Send .bss section info
	snprintf(buffer,MEMINFO_LINE_BUFFER_SIZE,msg_ramDiagnosticsRAM1_formatStringBSS,
		(uint32_t)&__bss_start__,		 // .bss start
		(uint32_t)&__bss_end__,          // .bss end
		ramDiagnosticsRAM1_bss_size,     // .bss size in KB
		ramDiagnosticsRAM1_bss_size      // .bss used size in KB
	);
	HAL_UART_Transmit(&uart,(uint8_t*)buffer,strlen(buffer),MEMINFO_UART_TIMEOUT);
// Send .tdat section info
	snprintf(buffer,MEMINFO_LINE_BUFFER_SIZE,msg_ramDiagnosticsRAM1_formatStringTData,
		0UL,                             // .tdat start
		0UL,							 // .tdat end
		ramDiagnosticsRAM1_tdat_size,    // .tdat size in KB
		ramDiagnosticsRAM1_tdat_size     // .tdat used size in KB
	);
	HAL_UART_Transmit(&uart,(uint8_t*)buffer,strlen(buffer),MEMINFO_UART_TIMEOUT);
// Send .heap section info
	snprintf(buffer,MEMINFO_LINE_BUFFER_SIZE,msg_ramDiagnosticsRAM1_formatStringHeap,
		(uint32_t)&_end,                          // .heap start
		(uint32_t)ramDiagnosticsRAM1_lastHeapEnd, // .heap end
		ramDiagnosticsRAM1_heap_size,             // .heap size in KB
		ramDiagnosticsRAM1_heap_size              // .heap used size in KB
	);
	HAL_UART_Transmit(&uart,(uint8_t*)buffer,strlen(buffer),MEMINFO_UART_TIMEOUT);
// Send .stack section info
	snprintf(buffer,MEMINFO_LINE_BUFFER_SIZE,msg_ramDiagnosticsRAM1_formatStringStack,
		(uint32_t)&__RAM1_end__,              // .stack start
		(uint32_t)ramDiagnosticsRAM1_lastMSP, // .stack end
		ramDiagnosticsRAM1_stack_size,        // .stack size in KB
		ramDiagnosticsRAM1_stack_size         // .stack used size in KB
	);
	HAL_UART_Transmit(&uart,(uint8_t*)buffer,strlen(buffer),MEMINFO_UART_TIMEOUT);
// Send RAM1 diagnostics footers
	HAL_UART_Transmit(&uart,(uint8_t*)msg_ramDiagnosticsRAM1_header3,strlen(msg_ramDiagnosticsRAM1_header3),MEMINFO_UART_TIMEOUT);
	
// Send Free RAM total
	snprintf(buffer,MEMINFO_LINE_BUFFER_SIZE,msg_ramDiagnosticsRAM1_formatStringFreeRAM,ramDiagnosticsRAM1_total_size-ramDiagnosticsRAM1_used);
	HAL_UART_Transmit(&uart,(uint8_t*)buffer,strlen(buffer),MEMINFO_UART_TIMEOUT);

// Send RAM diagnostics footers
	HAL_UART_Transmit(&uart,(uint8_t*)msg_ramDiagnosticsGeneral_footer1,strlen(msg_ramDiagnosticsGeneral_footer1),MEMINFO_UART_TIMEOUT);
	HAL_UART_Transmit(&uart,(uint8_t*)msg_ramDiagnosticsGeneral_footer2,strlen(msg_ramDiagnosticsGeneral_footer2),MEMINFO_UART_TIMEOUT);
}

void ramDiagnosticsRAM2(){
	char buffer[MEMINFO_LINE_BUFFER_SIZE]={0};
// Send RAM2 diagnostics headers 1-3
	HAL_UART_Transmit(&uart,(uint8_t*)msg_ramDiagnosticsRAM2_header1,strlen(msg_ramDiagnosticsRAM2_header1),MEMINFO_UART_TIMEOUT);
	HAL_UART_Transmit(&uart,(uint8_t*)msg_ramDiagnosticsRAM1_header2,strlen(msg_ramDiagnosticsRAM1_header2),MEMINFO_UART_TIMEOUT);
	HAL_UART_Transmit(&uart,(uint8_t*)msg_ramDiagnosticsRAM1_header3,strlen(msg_ramDiagnosticsRAM1_header3),MEMINFO_UART_TIMEOUT);
// Send .ramDiagnostics section info
	snprintf(buffer,MEMINFO_LINE_BUFFER_SIZE,msg_ramDiagnosticsRAM2_formatStringRamDia,
		(uint32_t)&__RAM2_start__,	                 // .ramDiagnostics start
		(uint32_t)&__RAM_DIAGNOSTICS_END__,          // .ramDiagnostics end
		ramDiagnosticsRAM2_ramDiagnostics_size,      // .ramDiagnostics size in KB
		ramDiagnosticsRAM2_ramDiagnostics_size       // .ramDiagnostics used size in KB
	);
	HAL_UART_Transmit(&uart,(uint8_t*)buffer,strlen(buffer),MEMINFO_UART_TIMEOUT);
// Send RAM2 diagnostics footers
	HAL_UART_Transmit(&uart,(uint8_t*)msg_ramDiagnosticsRAM1_header3,strlen(msg_ramDiagnosticsRAM1_header3),MEMINFO_UART_TIMEOUT);
// Send Free RAM total
	snprintf(buffer,MEMINFO_LINE_BUFFER_SIZE,msg_ramDiagnosticsRAM1_formatStringFreeRAM,ramDiagnosticsRAM2_total_size-ramDiagnosticsRAM2_used);
	HAL_UART_Transmit(&uart,(uint8_t*)buffer,strlen(buffer),MEMINFO_UART_TIMEOUT);
// Send RAM diagnostics footers
	HAL_UART_Transmit(&uart,(uint8_t*)msg_ramDiagnosticsGeneral_footer1,strlen(msg_ramDiagnosticsGeneral_footer1),MEMINFO_UART_TIMEOUT);
	HAL_UART_Transmit(&uart,(uint8_t*)msg_ramDiagnosticsGeneral_footer2,strlen(msg_ramDiagnosticsGeneral_footer2),MEMINFO_UART_TIMEOUT);
}

void ramDiagnosticsCCSRAM(){
	// Not applicable in this MCU
	// Will be added in production project with STM32G473
}

