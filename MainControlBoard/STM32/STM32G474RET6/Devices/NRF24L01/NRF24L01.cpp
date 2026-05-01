/**
 * @addtogroup NRF24L01
 * @{
 */

#include <NRF24L01.hpp>

#include <string.h>


void NRF24L01::initialize_hspi_data(){
    _transaction_data.callbackFn=nullptr;
    _transaction_data.flags=HSPI_FLAG_FULL_DUPLEX; // Only option since it's full duplex communication
    _transaction_data.gpio_pin=_csnPin.gpio_pin; // Chip select pin and port for SPI transport layer
    _transaction_data.gpio_port=_csnPin.gpio_port;
    _transaction_data.rxBuffer=nullptr;
    _transaction_data.rxLength=0;
    _transaction_data.txBuffer=nullptr;
    _transaction_data.txLength=0;
    _transaction_data.callbackEvent=0;
}

NRF24L01::NRF24L01(DEVICE_IO ce,DEVICE_IO csn, DEVICE_IO irq){
    _cePin=ce;
    _csnPin=csn;
    _irqPin=irq;

    this->initialize_hspi_data();
}

NRF24L01::NRF24L01(GPIO_TypeDef*cePort,uint16_t cePin,GPIO_TypeDef*csnPort,uint16_t csnPin,GPIO_TypeDef*irqPort,uint16_t irqPin){
    _cePin.gpio_port=cePort;
    _cePin.gpio_pin=cePin;
    _csnPin.gpio_port=csnPort;
    _csnPin.gpio_pin=csnPin;
    _irqPin.gpio_port=irqPort;
    _irqPin.gpio_pin=irqPin;

    this->initialize_hspi_data();
}

NRF24L01::~NRF24L01(){
    // Nothing to do
}

void NRF24L01::attach_tx_buffer(uint8_t*txBuffer){
    _transaction_data.txBuffer=txBuffer;
}

void NRF24L01::attach_rx_buffer(uint8_t*rxBuffer){
    _transaction_data.rxBuffer=rxBuffer;
}

void NRF24L01::attach_callback_function(void(*callbackFn)(uint8_t event),uint8_t event){
    _transaction_data.callbackFn=callbackFn;
    _transaction_data.callbackEvent=event;
}

bool NRF24L01::write_reg_config(uint8_t flags){
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_CONFIG;
    _transaction_data.txBuffer[1]=flags;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_config(){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_CONFIG;
    _transaction_data.txBuffer[1]=NRF_CMD_NOP;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_reg_en_aa(uint8_t flags){
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_EN_AA;
    _transaction_data.txBuffer[1]=flags;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_en_aa(){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_EN_AA;
    _transaction_data.txBuffer[1]=NRF_CMD_NOP;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_reg_en_rxAddr(uint8_t flags){
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_EN_RXADDR;
    _transaction_data.txBuffer[1]=flags;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_en_rxAddr(){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_EN_RXADDR;
    _transaction_data.txBuffer[1]=NRF_CMD_NOP;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_reg_setup_aw(uint8_t flags){
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_SETUP_AW;
    _transaction_data.txBuffer[1]=flags;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_setup_aw(){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_SETUP_AW;
    _transaction_data.txBuffer[1]=NRF_CMD_NOP;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_reg_setup_retr(uint8_t flags){
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_SETUP_RETR;
    _transaction_data.txBuffer[1]=flags;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_setup_retr(){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_SETUP_RETR;
    _transaction_data.txBuffer[1]=NRF_CMD_NOP;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_reg_rf_ch(uint8_t flags){
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_RF_CH;
    _transaction_data.txBuffer[1]=flags;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_rf_ch(){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_RF_CH;
    _transaction_data.txBuffer[1]=NRF_CMD_NOP;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_reg_rf_setup(uint8_t flags){
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_RF_SETUP;
    _transaction_data.txBuffer[1]=flags;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_rf_setup(){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_RF_SETUP;
    _transaction_data.txBuffer[1]=NRF_CMD_NOP;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_reg_status(uint8_t flags){
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_STATUS;
    _transaction_data.txBuffer[1]=flags;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_status(){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_STATUS;
    _transaction_data.txBuffer[1]=NRF_CMD_NOP;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_observe_tx(){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_OBSERVE_TX;
    _transaction_data.txBuffer[1]=NRF_CMD_NOP;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_cd(){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_CD;
    _transaction_data.txBuffer[1]=NRF_CMD_NOP;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_reg_rx_addr_p0(const uint8_t*addr,uint8_t length){
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_RX_ADDR_P0;
    memcpy(_transaction_data.txBuffer+1,addr,length);
    _transaction_data.txLength=length+1;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_rx_addr_p0(uint8_t length){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_RX_ADDR_P0;
    memset(_transaction_data.txBuffer+1,NRF_CMD_NOP,length);
    _transaction_data.txLength=length+1;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_reg_rx_addr_p1(const uint8_t*addr,uint8_t length){
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_RX_ADDR_P1;
    memcpy(_transaction_data.txBuffer+1,addr,length);
    _transaction_data.txLength=length+1;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_rx_addr_p1(uint8_t length){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_RX_ADDR_P1;
    memset(_transaction_data.txBuffer+1,NRF_CMD_NOP,length);
    _transaction_data.txLength=length+1;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_reg_rx_addr_p2(uint8_t addr){
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_RX_ADDR_P2;
    _transaction_data.txBuffer[1]=addr;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_rx_addr_p2(){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_RX_ADDR_P2;
    _transaction_data.txBuffer[1]=NRF_CMD_NOP;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_reg_rx_addr_p3(uint8_t addr){
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_RX_ADDR_P3;
    _transaction_data.txBuffer[1]=addr;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_rx_addr_p3(){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_RX_ADDR_P3;
    _transaction_data.txBuffer[1]=NRF_CMD_NOP;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_reg_rx_addr_p4(uint8_t addr){
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_RX_ADDR_P4;
    _transaction_data.txBuffer[1]=addr;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_rx_addr_p4(){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_RX_ADDR_P4;
    _transaction_data.txBuffer[1]=NRF_CMD_NOP;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_reg_rx_addr_p5(uint8_t addr){
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_RX_ADDR_P5;
    _transaction_data.txBuffer[1]=addr;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_rx_addr_p5(){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_RX_ADDR_P5;
    _transaction_data.txBuffer[1]=NRF_CMD_NOP;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_reg_tx_addr(const uint8_t*addr,uint8_t length){
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_TX_ADDR;
    memcpy(_transaction_data.txBuffer+1,addr,length);
    _transaction_data.txLength=length+1;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_tx_addr(uint8_t length){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_TX_ADDR;
    memset(_transaction_data.txBuffer+1,NRF_CMD_NOP,length);
    _transaction_data.txLength=length+1;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_reg_rx_pw_p0(uint8_t flags){
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_RX_PW_P0;
    _transaction_data.txBuffer[1]=flags;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_rx_pw_p0(){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_RX_PW_P0;
    _transaction_data.txBuffer[1]=NRF_CMD_NOP;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_reg_rx_pw_p1(uint8_t flags){
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_RX_PW_P1;
    _transaction_data.txBuffer[1]=flags;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_rx_pw_p1(){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_RX_PW_P1;
    _transaction_data.txBuffer[1]=NRF_CMD_NOP;
    _transaction_data.txLength=2;
    
    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_reg_rx_pw_p2(uint8_t flags){
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_RX_PW_P2;
    _transaction_data.txBuffer[1]=flags;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_rx_pw_p2(){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_RX_PW_P2;
    _transaction_data.txBuffer[1]=NRF_CMD_NOP;
    _transaction_data.txLength=2;
    
    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_reg_rx_pw_p3(uint8_t flags){
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_RX_PW_P3;
    _transaction_data.txBuffer[1]=flags;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_rx_pw_p3(){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_RX_PW_P3;
    _transaction_data.txBuffer[1]=NRF_CMD_NOP;
    _transaction_data.txLength=2;
    
    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_reg_rx_pw_p4(uint8_t flags){
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_RX_PW_P4;
    _transaction_data.txBuffer[1]=flags;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_rx_pw_p4(){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_RX_PW_P4;
    _transaction_data.txBuffer[1]=NRF_CMD_NOP;
    _transaction_data.txLength=2;
    
    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_reg_rx_pw_p5(uint8_t flags){
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_RX_PW_P5;
    _transaction_data.txBuffer[1]=flags;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_rx_pw_p5(){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_RX_PW_P5;
    _transaction_data.txBuffer[1]=NRF_CMD_NOP;
    _transaction_data.txLength=2;
    
    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_reg_fifo_status(uint8_t flags){
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_FIFO_STATUS;
    _transaction_data.txBuffer[1]=flags;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_fifo_status(){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_FIFO_STATUS;
    _transaction_data.txBuffer[1]=NRF_CMD_NOP;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_reg_dynpd(uint8_t flags){
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_DYNPD;
    _transaction_data.txBuffer[1]=flags;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_dynpd(){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_DYNPD;
    _transaction_data.txBuffer[1]=NRF_CMD_NOP;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_reg_feature(uint8_t flags){
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_FEATURE;
    _transaction_data.txBuffer[1]=flags;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_feature(){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_FEATURE;
    _transaction_data.txBuffer[1]=NRF_CMD_NOP;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_rx_payload(uint8_t length){
    _transaction_data.txBuffer[0]=NRF_CMD_R_RX_PAYLOAD;
    memset(_transaction_data.txBuffer+1,NRF_CMD_NOP,length);
    _transaction_data.txLength=length+1;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_tx_payload(uint8_t length){
    _transaction_data.txBuffer[0]=NRF_CMD_W_TX_PAYLOAD;
    _transaction_data.txLength=length+1;
    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::flush_tx(){
    _transaction_data.txBuffer[0]=NRF_CMD_FLUSH_TX;
    _transaction_data.txLength=1;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::flush_rx(){
    _transaction_data.txBuffer[0]=NRF_CMD_FLUSH_RX;
    _transaction_data.txLength=1;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::reuse_tx_pl(){
    _transaction_data.txBuffer[0]=NRF_CMD_REUSE_TX_PL;
    _transaction_data.txLength=1;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::activate(){
    _transaction_data.txBuffer[0]=NRF_CMD_ACTIVATE;
    _transaction_data.txBuffer[1]=NRF_CMD_ACTIVATE_DATA;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_ack_payload(uint8_t length,uint8_t rxPipe){
    _transaction_data.txBuffer[0]=NRF_CMD_W_ACK_PAYLOAD|rxPipe;
    _transaction_data.txLength=length+1;

    return spi1_dma_enq_data(&_transaction_data);
}

/**@} */