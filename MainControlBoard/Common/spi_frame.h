/**
 * Common file shared by STM32G4 and XMC4200 SPI frame definitions.
 * This file is intended to be included in both subprojects to avoid code duplication and maintain consistency in SPI frame definitions.
 * The SPI frame definitions in this file should be designed to be compatible with both STM32G4 and XMC4200 microcontrollers,
 * ensuring that the same SPI frame structure can be used across both platforms without modification.
 * 
 * 
 */

#ifndef _SPI_FRAME_H_
    #define _SPI_FRAME_H_

#include <stdint.h>

#ifdef __DOXYGEN__
    #define SECTION(x)
    #define ALIGNED(x)
    #define PACKED
#else
    #define SECTION(x) __attribute((section(x)))
    #define ALIGNED(x) __attribute((aligned(x)))
    #define PACKED __attribute((packed))
#endif // __DOXYGEN__

#define SPI_FRAME_SIZE 64 //! Defines the size of the SPI frame in bytes. Can be adjusted as needed to accommodate the data being transmitted.



typedef struct PACKED{
    uint8_t seq; //! Sequence number to track the order of frames. This can be used for error checking and ensuring that frames are processed in the correct order.
    uint8_t flags; //! Flags to indicate the type of data being transmitted or any special conditions.
    uint16_t ctrl; //! Control information for the frame, which can include commands or status indicators.
    uint16_t ch1; //! Channel 1 data, which can represent a specific sensor reading or control signal.
    uint16_t ch2; //! Channel 2 data, which can represent a specific sensor reading or control signal.
    uint16_t ch3; //! Channel 3 data, which can represent a specific sensor reading or control signal.
    uint16_t ch4; //! Channel 4 data, which can represent a specific sensor reading or control signal.
    uint16_t ch5; //! Channel 5 data, which can represent a specific sensor reading or control signal.
    uint16_t ch6; //! Channel 6 data, which can represent a specific sensor reading or control signal.
    uint16_t crc16; //! CRC16 checksum for error detection. This can be calculated based on the data being transmitted and included in the frame to allow the receiving device to verify the integrity of the received data.
}master_spi_frame_t; //! Structure sent by the master device (STM32G4) to the slave device (XMC4200). 

typedef struct PACKED{
    uint8_t seq; //! Sequence number to track the order of frames. This can be used for error checking and ensuring that frames are processed in the correct order.
    
    uint16_t crc16; //! CRC16 checksum for error detection. This can be calculated based on the data being transmitted and included in the frame to allow the receiving device to verify the integrity of the received data.
}slave_spi_frame_t; //! Structure sent by the slave device (XMC4200) to the master device (STM32G4).

// Note frames should be the same size and should be packed to ensure no padding bytes are added by the compiler

#endif // _SPI_FRAME_H_ 