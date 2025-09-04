#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stm32l4xx_hal.h>

#include <TrinityTrack6000_MemInfo.h>

extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t __bss_start__;
extern uint32_t __bss_end__;
extern uint32_t __sbrk_heap_end; // Defined by me
extern uint32_t _end;

extern UART_HandleTypeDef uart;

const char msg_ramDump_header1[]      ="[######]==============[RAM STATUS]=============[######]\r\n";
const char msg_ramDump_header2[]      ="[      ]    START    |     END     |   SIZE    [      ]\r\n";
const char msg_ramDump_data_header[]  ="[.DATA ] 0x";
const char msg_ramDump_data_footer[]  =" B [.DATA ]\r\n";
const char msg_ramDump_bss_header[]   ="[.BSS  ] 0x";
const char msg_ramDump_bss_footer[]   =" B [.BSS  ]\r\n";
const char msg_ramDump_tdat_header[]  ="[.TDAT] 0x";
const char msg_ramDump_tdat_footer[]  =" B[.TDAT]\r\n";
const char msg_ramDump_heap_header[]  ="[HEAP  ] 0x";
const char msg_ramDump_heap_footer[]  =" B[HEAP  ]\r\n";
const char msg_ramDump_stack_header[] ="[STACK ] 0x";
const char msg_ramDump_stack_footer[] =" B [STACK ]\r\n";
const char msg_ramDump_free_header[]  ="[FREE  ]---------------------------| ";
const char msg_ramDump_free_footer[]  =" B[FREE  ]\r\n";
const char msg_ramDump_addr_header[]  ="| 0x";
const char msg_ramDump_seperator[]    ="| ";

void ram_dump(){
	char buffer[MEMINFO_HEX_BUFFER_SIZE]={0};
	uint32_t section_size=123456;
// Send RAM dump header
	HAL_UART_Transmit(&uart,(uint8_t*)msg_ramDump_header1,strlen(msg_ramDump_header1),MEMINFO_UART_TIMEOUT);
	HAL_UART_Transmit(&uart,(uint8_t*)msg_ramDump_header2,strlen(msg_ramDump_header1),MEMINFO_UART_TIMEOUT);
// .data section
	HAL_UART_Transmit(&uart,(uint8_t*)msg_ramDump_data_header,strlen(msg_ramDump_data_header),MEMINFO_UART_TIMEOUT);
	snprintf(buffer,MEMINFO_HEX_BUFFER_SIZE,"%04lX_%04lX ",(uint32_t)&_sdata>>16,(uint32_t)&_sdata&0xFFFF);
	HAL_UART_Transmit(&uart,(uint8_t*)buffer,strlen(buffer),MEMINFO_UART_TIMEOUT);
	HAL_UART_Transmit(&uart,(uint8_t*)msg_ramDump_seperator,strlen(msg_ramDump_seperator),MEMINFO_UART_TIMEOUT);
	snprintf(buffer,MEMINFO_HEX_BUFFER_SIZE,"0x%04lX_%04lX ",(uint32_t)&_edata>>16,(uint32_t)&_edata&0xFFFF);
	HAL_UART_Transmit(&uart,(uint8_t*)buffer,strlen(buffer),MEMINFO_UART_TIMEOUT);
	HAL_UART_Transmit(&uart,(uint8_t*)msg_ramDump_seperator,strlen(msg_ramDump_seperator),MEMINFO_UART_TIMEOUT);
	section_size=(uint32_t)&_edata-(uint32_t)&_sdata;
	snprintf(buffer,MEMINFO_HEX_BUFFER_SIZE,"%3lu %3lu",section_size/1000,(section_size%1000));
	HAL_UART_Transmit(&uart,(uint8_t*)buffer,strlen(buffer),MEMINFO_UART_TIMEOUT);
	HAL_UART_Transmit(&uart,(uint8_t*)msg_ramDump_data_footer,strlen(msg_ramDump_data_footer),MEMINFO_UART_TIMEOUT);
// .bss section
	HAL_UART_Transmit(&uart,(uint8_t*)msg_ramDump_bss_header,strlen(msg_ramDump_bss_header),MEMINFO_UART_TIMEOUT);
	snprintf(buffer,MEMINFO_HEX_BUFFER_SIZE,"%04lX_%04lX ",(uint32_t)&__bss_start__>>16,(uint32_t)&__bss_start__&0xFFFF);
	HAL_UART_Transmit(&uart,(uint8_t*)buffer,strlen(buffer),MEMINFO_UART_TIMEOUT);
	HAL_UART_Transmit(&uart,(uint8_t*)msg_ramDump_seperator,strlen(msg_ramDump_seperator),MEMINFO_UART_TIMEOUT);
	snprintf(buffer,MEMINFO_HEX_BUFFER_SIZE,"0x%04lX_%04lX ",(uint32_t)&__bss_end__>>16,(uint32_t)&__bss_end__&0xFFFF);
	HAL_UART_Transmit(&uart,(uint8_t*)buffer,strlen(buffer),MEMINFO_UART_TIMEOUT);
	HAL_UART_Transmit(&uart,(uint8_t*)msg_ramDump_seperator,strlen(msg_ramDump_seperator),MEMINFO_UART_TIMEOUT);
	section_size=(uint32_t)&__bss_end__-(uint32_t)&__bss_start__;
	snprintf(buffer,MEMINFO_HEX_BUFFER_SIZE,"%3lu %3lu",section_size/1000,(section_size%1000));
	HAL_UART_Transmit(&uart,(uint8_t*)buffer,strlen(buffer),MEMINFO_UART_TIMEOUT);
	HAL_UART_Transmit(&uart,(uint8_t*)msg_ramDump_bss_footer,strlen(msg_ramDump_bss_footer),MEMINFO_UART_TIMEOUT);

// .tdat section
	//HAL_UART_Transmit(&uart,(uint8_t*)msg_ramDump_bss_header,strlen(msg_ramDump_bss_header),MEMINFO_UART_TIMEOUT);

// heap section
	HAL_UART_Transmit(&uart,(uint8_t*)msg_ramDump_heap_header,strlen(msg_ramDump_heap_header),MEMINFO_UART_TIMEOUT);
	snprintf(buffer,MEMINFO_HEX_BUFFER_SIZE,"%04lX_%04lX ",(uint32_t)&__sbrk_heap_end>>16,(uint32_t)&__sbrk_heap_end&0xFFFF);
	HAL_UART_Transmit(&uart,(uint8_t*)buffer,strlen(buffer),MEMINFO_UART_TIMEOUT);
	HAL_UART_Transmit(&uart,(uint8_t*)msg_ramDump_seperator,strlen(msg_ramDump_seperator),MEMINFO_UART_TIMEOUT);
	snprintf(buffer,MEMINFO_HEX_BUFFER_SIZE,"0x%04lX_%04lX ",(uint32_t)&_end>>16,(uint32_t)&_end&0xFFFF);
	HAL_UART_Transmit(&uart,(uint8_t*)buffer,strlen(buffer),MEMINFO_UART_TIMEOUT);
	HAL_UART_Transmit(&uart,(uint8_t*)msg_ramDump_seperator,strlen(msg_ramDump_seperator),MEMINFO_UART_TIMEOUT);
	section_size=(uint32_t)&_end-(uint32_t)&__sbrk_heap_end;
	snprintf(buffer,MEMINFO_HEX_BUFFER_SIZE,"%3lu %3lu",section_size/1000,(section_size%1000));
	HAL_UART_Transmit(&uart,(uint8_t*)buffer,strlen(buffer),MEMINFO_UART_TIMEOUT);
	HAL_UART_Transmit(&uart,(uint8_t*)msg_ramDump_heap_footer,strlen(msg_ramDump_heap_footer),MEMINFO_UART_TIMEOUT);

// stack section
	//HAL_UART_Transmit(&uart,(uint8_t*)msg_ramDump_tdat_header,strlen(msg_ramDump_tdat_header),MEMINFO_UART_TIMEOUT);

// Send RAM dump footer
	//HAL_UART_Transmit(&uart,(uint8_t*)msg_ramDump_header1,strlen(msg_ramDump_header1),MEMINFO_UART_TIMEOUT);
}

void ram2_dump(){
	// To be implemented
}

void flash_dump(){

}
