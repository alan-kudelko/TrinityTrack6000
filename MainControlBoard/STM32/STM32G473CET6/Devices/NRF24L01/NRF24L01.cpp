/**
 * @addtogroup NRF24L01
 * @{
 */

#include <NRF24L01.h>

#include <string.h>

#include <SPI1_Dma.h>

#include <stm32g4xx_hal.h> // Delete later
#include <USART1_Dma.h> // Delete later after testing

void NRF24L01::initialize_hspi_data(){
    _transaction_data.callbackFn=nullptr;
    _transaction_data.flags=HSPI_FLAG_FULL_DUPLEX; // Only option since it's full duplex communication
    _transaction_data.gpio_pin=_csnPin.gpio_pin; // Chip select pin and port for SPI transport layer
    _transaction_data.gpio_port=_csnPin.gpio_port;
    _transaction_data.rxBuffer=nullptr;
    _transaction_data.rxLength=0;
    _transaction_data.txBuffer=nullptr;
    _transaction_data.txLength=0;
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

void NRF24L01::attach_callback_function(void(*callbackFn)(void)){
    // Probably should be inline since it's only 1 line
    _transaction_data.callbackFn=callbackFn;
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
    // SETUP_AW is important here since it defines address length
    // Add error checking in the future //Fix
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
    // SETUP_AW is important here since it defines address length
    // Add error checking in the future //Fix
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

bool NRF24L01::write_reg_rx_addr_p2(const uint8_t*addr,uint8_t length){
    // SETUP_AW is important here since it defines address length
    // Add error checking in the future //Fix
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_RX_ADDR_P2;
    memcpy(_transaction_data.txBuffer+1,addr,length);
    _transaction_data.txLength=length+1;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_rx_addr_p2(uint8_t length){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_RX_ADDR_P2;
    memset(_transaction_data.txBuffer+1,NRF_CMD_NOP,length);
    _transaction_data.txLength=length+1;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_reg_rx_addr_p3(const uint8_t*addr,uint8_t length){
    // SETUP_AW is important here since it defines address length
    // Add error checking in the future //Fix
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_RX_ADDR_P3;
    memcpy(_transaction_data.txBuffer+1,addr,length);
    _transaction_data.txLength=length+1;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_rx_addr_p3(uint8_t length){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_RX_ADDR_P3;
    memset(_transaction_data.txBuffer+1,NRF_CMD_NOP,length);
    _transaction_data.txLength=length+1;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_reg_rx_addr_p4(const uint8_t*addr,uint8_t length){
    // SETUP_AW is important here since it defines address length
    // Add error checking in the future //Fix
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_RX_ADDR_P4;
    memcpy(_transaction_data.txBuffer+1,addr,length);
    _transaction_data.txLength=length+1;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_rx_addr_p4(uint8_t length){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_RX_ADDR_P4;
    memset(_transaction_data.txBuffer+1,NRF_CMD_NOP,length);
    _transaction_data.txLength=length+1;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_reg_rx_addr_p5(const uint8_t*addr,uint8_t length){
    // SETUP_AW is important here since it defines address length
    // Add error checking in the future //Fix
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_RX_ADDR_P5;
    memcpy(_transaction_data.txBuffer+1,addr,length);
    _transaction_data.txLength=length+1;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_rx_addr_p5(uint8_t length){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_RX_ADDR_P5;
    memset(_transaction_data.txBuffer+1,NRF_CMD_NOP,length);
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
    memset(_transaction_data.txBuffer+1,NRF_CMD_NOP,32);
    _transaction_data.txLength=length+1;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_tx_payload(uint8_t length){
    return true;
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
    // For now pipe 0
bool NRF24L01::write_ack_payload(uint8_t length,uint8_t rxPipe){
    _transaction_data.txBuffer[0]=NRF_CMD_W_ACK_PAYLOAD|rxPipe;
    memset(_transaction_data.txBuffer+1,0xAA,length); // Temporary
    _transaction_data.txLength=length+1;

    return spi1_dma_enq_data(&_transaction_data);
}

// Test functions

extern SPI_HandleTypeDef hspi1;

extern "C" void nrf24l01_display_all_registers();

extern UART_HandleTypeDef huart1;

extern "C" void nrf24l01_display_all_registers(){
    // Function to display all register values to serial terminal
    // Used only during testing of the wireless communication
    typedef struct NRF24L01_REG{
        uint8_t config;
        uint8_t en_aa;
        uint8_t en_rxaddr;
        uint8_t setup_aw;
        uint8_t setup_retr;
        uint8_t rf_ch;
        uint8_t rf_setup;
        uint8_t status;
        uint8_t observe_tx;
        uint8_t cd;
        uint8_t rx_addr_p0[5];
        uint8_t rx_addr_p1[5];
        uint8_t rx_addr_p2[5];
        uint8_t rx_addr_p3[5];
        uint8_t rx_addr_p4[5];
        uint8_t rx_addr_p5[5];
        uint8_t tx_addr[5];
        uint8_t rx_pw_p0;
        uint8_t rx_pw_p1;
        uint8_t rx_pw_p2;
        uint8_t rx_pw_p3;
        uint8_t rx_pw_p4;
        uint8_t rx_pw_p5;
        uint8_t fifo_status;
        uint8_t dynpd;
        uint8_t feature;
    }NRF24L01_REG;

    NRF24L01_REG nrf24l01_reg;

    UNUSED(nrf24l01_reg);

    uint8_t operationTx[33]={0};
    uint8_t operationRx[33]={0};

    char buffer[33]{0};
    UNUSED(buffer);

    NRF24L01 nrf24l01(nullptr,0,GPIOB,GPIO_PIN_0,nullptr,0);
    nrf24l01.attach_rx_buffer(operationRx);
    nrf24l01.attach_tx_buffer(operationTx);
// Reading data from NRF24L01 and assigning values to the struct
// CONFIG
    nrf24l01.read_reg_config();
    HAL_Delay(50);
    nrf24l01_reg.config=operationRx[1];
// ENAA
    nrf24l01.read_reg_en_aa();
    HAL_Delay(50);
    nrf24l01_reg.en_aa=operationRx[1];
// EN_RXADDR
    nrf24l01.read_reg_en_rxAddr();
    HAL_Delay(50);
    nrf24l01_reg.en_rxaddr=operationRx[1];
// SETUP_AW
    nrf24l01.read_reg_setup_aw();
    HAL_Delay(50);
    nrf24l01_reg.setup_aw=operationRx[1];
// SETUP_RETR
    nrf24l01.read_reg_setup_retr();
    HAL_Delay(50);
    nrf24l01_reg.setup_retr=operationRx[1];
// RF_CH
    nrf24l01.read_reg_rf_ch();
    HAL_Delay(50);
    nrf24l01_reg.rf_ch=operationRx[1];
// RF_SETUP
    nrf24l01.read_reg_rf_setup();
    HAL_Delay(50);
    nrf24l01_reg.rf_setup=operationRx[1];
// STATUS
    nrf24l01.read_reg_status();
    HAL_Delay(50);
    nrf24l01_reg.status=operationRx[1];
// OBSERVE_TX
    nrf24l01.read_reg_observe_tx();
    HAL_Delay(50);
    nrf24l01_reg.observe_tx=operationRx[1];
// CD
    nrf24l01.read_reg_cd();
    HAL_Delay(50);
    nrf24l01_reg.cd=operationRx[1];
// RX_ADDR_P0
    nrf24l01.read_reg_rx_addr_p0(5);
    HAL_Delay(50);
    memcpy(nrf24l01_reg.rx_addr_p0,operationRx+1,5);
// RX_ADDR_P1
    nrf24l01.read_reg_rx_addr_p1(5);
    HAL_Delay(50);
    memcpy(nrf24l01_reg.rx_addr_p1,operationRx+1,5);
// RX_ADDR_P2
    nrf24l01.read_reg_rx_addr_p2(5);
    HAL_Delay(50);
    memcpy(nrf24l01_reg.rx_addr_p2,operationRx+1,5);
// RX_ADDR_P3
    nrf24l01.read_reg_rx_addr_p3(5);
    HAL_Delay(50);
    memcpy(nrf24l01_reg.rx_addr_p3,operationRx+1,5);
// RX_ADDR_P4
    nrf24l01.read_reg_rx_addr_p4(5);
    HAL_Delay(50);
    memcpy(nrf24l01_reg.rx_addr_p4,operationRx+1,5);
// RX_ADDR_P5
    nrf24l01.read_reg_rx_addr_p5(5);
    HAL_Delay(50);
    memcpy(nrf24l01_reg.rx_addr_p5,operationRx+1,5);
// TX_ADDR
    nrf24l01.read_reg_tx_addr(5);
    HAL_Delay(50);
    memcpy(nrf24l01_reg.tx_addr,operationRx+1,5);
// RX_PW_P0
    nrf24l01.read_reg_rx_pw_p0();
    HAL_Delay(50);
    nrf24l01_reg.rx_pw_p0=operationRx[1];
// RX_PW_P1
    nrf24l01.read_reg_rx_pw_p1();
    HAL_Delay(50);
    nrf24l01_reg.rx_pw_p1=operationRx[1];
// RX_PW_P2
    nrf24l01.read_reg_rx_pw_p2();
    HAL_Delay(50);
    nrf24l01_reg.rx_pw_p2=operationRx[1];
// RX_PW_P3
    nrf24l01.read_reg_rx_pw_p3();
    HAL_Delay(50);
    nrf24l01_reg.rx_pw_p3=operationRx[1];
// RX_PW_P4
    nrf24l01.read_reg_rx_pw_p4();
    HAL_Delay(50);
    nrf24l01_reg.rx_pw_p4=operationRx[1];
// RX_PW_P5
    nrf24l01.read_reg_rx_pw_p5();
    HAL_Delay(50);
    nrf24l01_reg.rx_pw_p5=operationRx[1];
// FIFO_STATUS
    nrf24l01.read_reg_fifo_status();
    HAL_Delay(50);
    nrf24l01_reg.fifo_status=operationRx[1];
// DYNPD
    nrf24l01.read_reg_dynpd();
    HAL_Delay(50);
    nrf24l01_reg.dynpd=operationRx[1];
// FEATURE
    nrf24l01.read_reg_feature();
    HAL_Delay(50);
    nrf24l01_reg.feature=operationRx[1];
    //HAL_UART_Transmit(&huart1,(uint8_t*)buffer,8,HAL_MAX_DELAY);

}

extern "C" void nrf24l01_test_ack_payload(void){
    while(1){
        HAL_Delay(1000);
        nrf24l01_display_all_registers();
    }
}

/**@} */