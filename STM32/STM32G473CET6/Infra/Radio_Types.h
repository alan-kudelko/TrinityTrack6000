//Doxy

#ifndef RADIO_TYPES_H_
    #define RADIO_TYPES_H_

#ifdef __DOXYGEN__
    #define SECTION(x)
    #define ALIGNED(x)
#else
    #define SECTION(x) __attribute((section(x)))
    #define ALIGNED(x) __attribute((aligned(x)))
#endif // __DOXYGEN__

#include <stdint.h>

#include <TrinityTrack6000_RadioConfig.h>

typedef struct ALIGNED(4) NRF_SETTINGS{
    uint8_t config;
    uint8_t en_aa;
    uint8_t en_rxaddr;
    uint8_t setup_aw;
    uint8_t setup_retr;
    uint8_t rf_ch;
    uint8_t rf_setup;
    uint8_t rx_addr_p0[RADIO_DEFAULT_ADDRESS_LENGTH];
    uint8_t rx_addr_p1[RADIO_DEFAULT_ADDRESS_LENGTH];
    uint8_t rx_addr_p2;
    uint8_t rx_addr_p3;
    uint8_t rx_addr_p4;
    uint8_t rx_addr_p5;
    uint8_t tx_addr[RADIO_DEFAULT_ADDRESS_LENGTH];
    uint8_t rx_pw_p0;
    uint8_t rx_pw_p1;
    uint8_t rx_pw_p2;
    uint8_t rx_pw_p3;
    uint8_t rx_pw_p4;
    uint8_t rx_pw_p5;
    uint8_t dynpd;
    uint8_t feature;
}NRF_SETTINGS;

typedef struct ALIGNED(4) RADIO_RUNTIME_STATS{
    uint8_t status;
    uint8_t fifo_status;
    uint8_t observe_tx;
    uint8_t cd;
}RADIO_RUNTIME_STATS; // Check alignment

typedef struct ALIGNED(4) RADIO_STATS{
    uint32_t total_packages;
    uint32_t packages_received;
    uint32_t packages_dropped;
    uint32_t packages_duplicate;
    uint32_t packages_out_of_order;
    uint8_t expected_id;
    uint8_t last_id;
    uint8_t first_packet; // Flag indicating firsts received packet in the session
}RADIO_STATS; // Change alingnment


#endif // RADIO_TYPES_H_