#include <avr/io.h>
#include <SPI.h>

#define CE_PIN 7
#define CSN_PIN 6
#define IRQ_PIN 5

#define NRF_REG_CONFIG        0x00
#define NRF_REG_EN_AA         0x01
#define NRF_REG_EN_RXADDR     0x02
#define NRF_REG_SETUP_AW      0x03
#define NRF_REG_SETUP_RETR    0x04
#define NRF_REG_RF_CH         0x05
#define NRF_REG_RF_SETUP      0x06
#define NRF_REG_STATUS        0x07
#define NRF_REG_OBSERVE_TX    0x08
#define NRF_REG_CD            0x09

#define NRF_REG_RX_ADDR_P0    0x0A
#define NRF_REG_RX_ADDR_P1    0x0B
#define NRF_REG_RX_ADDR_P2    0x0C
#define NRF_REG_RX_ADDR_P3    0x0D
#define NRF_REG_RX_ADDR_P4    0x0E
#define NRF_REG_RX_ADDR_P5    0x0F

#define NRF_REG_TX_ADDR       0x10
#define NRF_REG_RX_PW_P0      0x11
#define NRF_REG_RX_PW_P1      0x12
#define NRF_REG_RX_PW_P2      0x13
#define NRF_REG_RX_PW_P3      0x14
#define NRF_REG_RX_PW_P4      0x15
#define NRF_REG_RX_PW_P5      0x16

#define NRF_REG_FIFO_STATUS   0x17
#define NRF_REG_DYNPD         0x1C
#define NRF_REG_FEATURE       0x1D

/** @} */


/**
 * @defgroup NRF24L01_Commands Commands
 * @{
 */

#define NRF_CMD_R_REGISTER         0x00
#define NRF_CMD_W_REGISTER         0x20
#define NRF_CMD_R_RX_PAYLOAD       0x61
#define NRF_CMD_W_TX_PAYLOAD       0xA0
#define NRF_CMD_FLUSH_TX           0xE1
#define NRF_CMD_FLUSH_RX           0xE2
#define NRF_CMD_REUSE_TX_PL        0xE3
#define NRF_CMD_ACTIVATE           0x50
#define NRF_CMD_ACTIVATE_DATA      0x73
#define NRF_CMD_R_RX_PL_WID        0x60
#define NRF_CMD_W_ACK_PAYLOAD      0xA8
#define NRF_CMD_W_TX_PAYLOAD_NOACK 0xB0
#define NRF_CMD_NOP                0xFF

/** @} */

/**
 * @defgroup NRF24L01_BitMasks Bit Masks
 * @{
 */

#define NRF_BIT_MASK_RX_DR         (1<<6)
#define NRF_BIT_MASK_TX_DS         (1<<5)
#define NRF_BIT_MASK_MAX_RT        (1<<4)
#define NRF_BIT_EN_CRC             (1<<3)
#define NRF_BIT_CRCO               (1<<2)
#define NRF_BIT_PWR_UP             (1<<1)
#define NRF_BIT_PRIM_RX            (1<<0)

#define NRF_BIT_ENAA_P5            (1<<5)
#define NRF_BIT_ENAA_P4            (1<<4)
#define NRF_BIT_ENAA_P3            (1<<3)
#define NRF_BIT_ENAA_P2            (1<<2)
#define NRF_BIT_ENAA_P1            (1<<1)
#define NRF_BIT_ENAA_P0            (1<<0)

#define NRF_BIT_ERX_P5             (1<<5)
#define NRF_BIT_ERX_P4             (1<<4)
#define NRF_BIT_ERX_P3             (1<<3)
#define NRF_BIT_ERX_P2             (1<<2)
#define NRF_BIT_ERX_P1             (1<<1)
#define NRF_BIT_ERX_P0             (1<<0)

#define NRF_BIT_AW1                (1<<1)
#define NRF_BIT_AW0                (1<<0)

#define NRF_BIT_ARD7               (1<<7)
#define NRF_BIT_ARD6               (1<<6)
#define NRF_BIT_ARD5               (1<<5)
#define NRF_BIT_ARD4               (1<<4)

#define NRF_BIT_ARC3               (1<<3)
#define NRF_BIT_ARC2               (1<<2)
#define NRF_BIT_ARC1               (1<<1)
#define NRF_BIT_ARC0               (1<<0)

#define NRF_BIT_RF_CH6             (1<<6)
#define NRF_BIT_RF_CH5             (1<<5)
#define NRF_BIT_RF_CH4             (1<<4)
#define NRF_BIT_RF_CH3             (1<<3)
#define NRF_BIT_RF_CH2             (1<<2)
#define NRF_BIT_RF_CH1             (1<<1)
#define NRF_BIT_RF_CH0             (1<<0)

#define NRF_BIT_PLL_LOCK           (1<<4)
#define NRF_BIT_RF_DR              (1<<3)
#define NRF_BIT_RF_PWR2            (1<<2)
#define NRF_BIT_RF_PWR1            (1<<1)
#define NRF_BIT_RF_LNA_HCURR       (1<<0)

#define NRF_BIT_RX_DR              (1<<6)
#define NRF_BIT_TX_DS              (1<<5)
#define NRF_BIT_MAX_RT             (1<<4)
#define NRF_BIT_P_NO3              (1<<3)
#define NRF_BIT_P_NO2              (1<<2)
#define NRF_BIT_P_NO1              (1<<1)
#define NRF_BIT_TX_FULL            (1<<0)

#define NRF_BIT_PLOS_CNT7          (1<<7)
#define NRF_BIT_PLOS_CNT6          (1<<6)
#define NRF_BIT_PLOS_CNT5          (1<<5)
#define NRF_BIT_PLOS_CNT4          (1<<4)
#define NRF_BIT_ARC_CNT3           (1<<3)
#define NRF_BIT_ARC_CNT2           (1<<2)
#define NRF_BIT_ARC_CNT1           (1<<1)
#define NRF_BIT_ARC_CNT0           (1<<0)

#define NRF_BIT_CD                 (1<<0)
// RX_ADDR_P0-P5 39:0

#define NRF_BIT_TX_REUSE           (1<<6)
#define NRF_BIT_RX_FULL            (1<<1)
#define NRF_BIT_RX_EMPTY           (1<<0)

#define NRF_BIT_DPL_P5             (1<<5)
#define NRF_BIT_DPL_P4             (1<<4)
#define NRF_BIT_DPL_P3             (1<<3)
#define NRF_BIT_DPL_P2             (1<<2)
#define NRF_BIT_DPL_P1             (1<<1)
#define NRF_BIT_DPL_P0             (1<<0)

#define NRF_BIT_EN_DPL             (1<<2)
#define NRF_BIT_EN_ACK_PAY         (1<<1)
#define NRF_BIT_EN_DYN_ACK         (1<<0)

/** @} */

#define RADIO_DEFAULT_ADDRESS          {0xFE,0xFE,0xFE,0xFE,0xFE}
#define RADIO_DEFAULT_PAYLOAD_SIZE_32  (1<<5)
#define RADIO_DEFAULT_ADDRESS_LENGTH   5

// MACRO DEF

#define NRF_READ_REG(reg) \
{ \
  op[0] = NRF_CMD_R_REGISTER | (reg); \
  op[1] = NRF_CMD_NOP; \
  set_CSN_low(); \
  SPI.transfer(op,2); \
  set_CSN_high(); \
}

#define NRF_READ_BUF(reg,len) \
{ \
  op[0] = NRF_CMD_R_REGISTER | (reg); \
  for(int i=1;i<=len;i++) op[i]=NRF_CMD_NOP; \
  set_CSN_low(); \
  SPI.transfer(op,len+1); \
  set_CSN_high(); \
}

#define NRF_WRITE(reg,val) \
{ op[0]=(NRF_CMD_W_REGISTER|(reg)); op[1]=(val); set_CSN_low(); SPI.transfer(op,2); set_CSN_high(); }

#define NRF_READ(reg) \
{ op[0]=(NRF_CMD_R_REGISTER|(reg)); op[1]=NRF_CMD_NOP; set_CSN_low(); SPI.transfer(op,2); set_CSN_high(); }

#define NRF_CMD(cmd,val) \
{ op[0]=(cmd); op[1]=(val); set_CSN_low(); SPI.transfer(op,2); set_CSN_high(); }

#define NRF_WRITE_BUF(reg,buf,len) \
{ set_CSN_low(); SPI.transfer(NRF_CMD_W_REGISTER|(reg)); SPI.transfer(buf,len); set_CSN_high(); }

#define NRF_CMD1(cmd) \
{ set_CSN_low(); SPI.transfer(cmd); set_CSN_high(); }



void display_all_register()
{
  uint8_t op[33];

  Serial.println("");

  #define PRINT_REG(name,reg) \
  NRF_READ_REG(reg); \
  Serial.print(name); \
  Serial.print(" "); \
  Serial.println(op[1],HEX);

  PRINT_REG("CONFIG     ",NRF_REG_CONFIG);
  PRINT_REG("EN_AA      ",NRF_REG_EN_AA);
  PRINT_REG("EN_RXADDR  ",NRF_REG_EN_RXADDR);
  PRINT_REG("SETUP_AW   ",NRF_REG_SETUP_AW);
  PRINT_REG("SETUP_RETR ",NRF_REG_SETUP_RETR);
  PRINT_REG("RF_CH      ",NRF_REG_RF_CH);
  PRINT_REG("RF_SETUP   ",NRF_REG_RF_SETUP);
  PRINT_REG("STATUS     ",NRF_REG_STATUS);
  PRINT_REG("OBSERVE_TX ",NRF_REG_OBSERVE_TX);
  PRINT_REG("RPD/CD     ",NRF_REG_CD);

  PRINT_REG("RX_PW_P0   ",NRF_REG_RX_PW_P0);
  PRINT_REG("RX_PW_P1   ",NRF_REG_RX_PW_P1);
  PRINT_REG("RX_PW_P2   ",NRF_REG_RX_PW_P2);
  PRINT_REG("RX_PW_P3   ",NRF_REG_RX_PW_P3);
  PRINT_REG("RX_PW_P4   ",NRF_REG_RX_PW_P4);
  PRINT_REG("RX_PW_P5   ",NRF_REG_RX_PW_P5);

  PRINT_REG("FIFO_STAT  ",NRF_REG_FIFO_STATUS);
  PRINT_REG("DYNPD      ",NRF_REG_DYNPD);
  PRINT_REG("FEATURE    ",NRF_REG_FEATURE);

  #undef PRINT_REG

  // RX_ADDR_P0
  NRF_READ_BUF(NRF_REG_RX_ADDR_P0,5);
  Serial.print("RX_ADDR_P0 ");
  for(int i=1;i<=5;i++)
  {
    Serial.print(op[i],HEX);
    Serial.print(" ");
  }
  Serial.println("");

  // RX_ADDR_P1
  NRF_READ_BUF(NRF_REG_RX_ADDR_P1,5);
  Serial.print("RX_ADDR_P1 ");
  for(int i=1;i<=5;i++)
  {
    Serial.print(op[i],HEX);
    Serial.print(" ");
  }
  Serial.println("");
  delay(10);
  // TX_ADDR
  NRF_READ_BUF(NRF_REG_TX_ADDR,5);
  Serial.print("TX_ADDR    ");
  for(int i=1;i<=5;i++)
  {
    Serial.print(op[i],HEX);
    Serial.print(" ");
  }
  Serial.println("");

  Serial.println("");
}

void arduino_tx_2Mbps_init(void){
    uint8_t op[2];
    uint8_t addr[5]={0xFE,0xFE,0xFE,0xFE,0xFE};

    set_CE_low();

    // szybkie włączenie zasilania
    NRF_WRITE(NRF_REG_CONFIG, NRF_BIT_PWR_UP);
    delay(2);

    // spróbuj ustawić FEATURE
    NRF_WRITE(NRF_REG_FEATURE, NRF_BIT_EN_ACK_PAY);
    NRF_READ(NRF_REG_FEATURE);

    if(op[1] != NRF_BIT_EN_ACK_PAY)
    {
        NRF_CMD(NRF_CMD_ACTIVATE,0x73);
        NRF_WRITE(NRF_REG_FEATURE,NRF_BIT_EN_ACK_PAY);
    }

    // brak dynamic payload
    NRF_WRITE(NRF_REG_DYNPD,0x00);

    // CONFIG (TX mode)
    NRF_WRITE(NRF_REG_CONFIG,
        NRF_BIT_TX_DS|
        NRF_BIT_MAX_RT|
        NRF_BIT_EN_CRC|
        NRF_BIT_CRCO|
        NRF_BIT_PWR_UP);

    // auto ACK
    NRF_WRITE(NRF_REG_EN_AA,NRF_BIT_ENAA_P0);

    // enable pipe0
    NRF_WRITE(NRF_REG_EN_RXADDR,NRF_BIT_ERX_P0);

    // address width 5B
    NRF_WRITE(NRF_REG_SETUP_AW,NRF_BIT_AW1|NRF_BIT_AW0);

    // retry disabled (TX)
    NRF_WRITE(NRF_REG_SETUP_RETR,0x2F);

    // kanał
    NRF_WRITE(NRF_REG_RF_CH,NRF_BIT_RF_CH1|NRF_BIT_RF_CH5|NRF_BIT_RF_CH4);

    // 2 Mbps
    NRF_WRITE(NRF_REG_RF_SETUP,
        (NRF_BIT_RF_DR&0)|
        NRF_BIT_RF_PWR2|
        NRF_BIT_RF_PWR1|
        NRF_BIT_RF_LNA_HCURR);

    // adresy
    NRF_WRITE_BUF(NRF_REG_RX_ADDR_P0,addr,5);
    //NRF_WRITE_BUF(NRF_REG_TX_ADDR,addr,5);

set_CSN_low();

SPI.transfer(NRF_CMD_W_REGISTER | NRF_REG_TX_ADDR);

SPI.transfer(0xFE);
SPI.transfer(0xFE);
SPI.transfer(0xFE);
SPI.transfer(0xFE);
SPI.transfer(0xFE);

set_CSN_high();

    // payload size
    NRF_WRITE(NRF_REG_RX_PW_P0,0x20);

    // flush fifo
    NRF_CMD1(NRF_CMD_FLUSH_RX);
    NRF_CMD1(NRF_CMD_FLUSH_TX);

    // clear IRQ flags
    NRF_WRITE(NRF_REG_STATUS,
        NRF_BIT_RX_DR|
        NRF_BIT_TX_DS|
        NRF_BIT_MAX_RT);

    // start RX
    set_CE_high();
}

void nrf_send_test_payload(uint8_t packageId)
{
    uint8_t payload[32];

    payload[0]=packageId;
    // Serial.print(payload[0]);
    // Serial.print(payload[1]);
    // Serial.println(" ");

    // wypełnij payload wzorcem testowym
    for(int i=2;i<32;i++)
    {
        payload[i] = 0xAA;
    }

    // załaduj payload do TX FIFO
    set_CSN_low();

    SPI.transfer(NRF_CMD_W_TX_PAYLOAD);

    for(int i=0;i<32;i++)
    {
        SPI.transfer(payload[i]);
    }

    set_CSN_high();
    delay(1);
    // rozpocznij transmisję
    //set_CE_high();
   // delayMicroseconds(100);
    //set_CE_low();

    // wyczyść wszystkie flagi TX


    // wyczyść TX FIFO (na wypadek MAX_RT)
    //set_CSN_low();
    //SPI.transfer(NRF_CMD_FLUSH_TX);
    //set_CSN_high();

    //display_all_register();

    // wyczyść TX FIFO (na wypadek MAX_RT)
    set_CSN_low();
    SPI.transfer(NRF_CMD_FLUSH_TX);
    set_CSN_high();

    // wyczyść wszystkie flagi TX
        set_CSN_low();
    SPI.transfer(NRF_CMD_W_REGISTER | NRF_REG_STATUS);
    SPI.transfer(NRF_BIT_TX_DS | NRF_BIT_MAX_RT);
    set_CSN_high();
    delayMicroseconds(800); // 504 Hz
    //delayMicroseconds(1600); // 350 Hz
    //delayMicroseconds(3000); // 250 Hz
    //delayMicroseconds(6000); // 140 Hz
    //delayMicroseconds(12000); // 75 Hz
    //delayMicroseconds(36000);   // 32 Hz
    //delayMicroseconds(72000);   // 16 Hz
    //delayMicroseconds(144000);
    //delayMicroseconds(288000);
    //delayMicroseconds(576000);
    //delay(500);
}

void arduino_tx_2Mbps(){

}

void set_CE_low(){
  digitalWrite(CE_PIN,LOW);
}

void set_CE_high(){
  digitalWrite(CE_PIN,HIGH);
}

void set_CSN_low(){
  digitalWrite(CSN_PIN,LOW);
}

void set_CSN_high(){
  digitalWrite(CSN_PIN,HIGH);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(CE_PIN,OUTPUT);
  pinMode(CSN_PIN,OUTPUT);

  set_CE_high();
  set_CSN_low();
  SPI.begin();
  Serial.begin(115200);
// Radio settings

  //communication_test_2Mbps();
  arduino_tx_2Mbps_init();
  display_all_register();
  set_CE_high();
  delay(10);
  //arduino_tx_2Mbps();
  volatile uint8_t counter=0;
  while(true){ 
    //Serial.print("Iteracja i=");
    //Serial.print(counter);
    //Serial.println("");
    nrf_send_test_payload(counter);
    counter++;
  }
}

// Dodac wyswietlacz LCD na którym będą się wyświetlać wyniki testu komunikacji


void loop() {
  // put your main code here, to run repeatedly:

}
