/**
 * @file MCP23S17.cpp
 * @brief Implementation of MCP23S17.h
 * 
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.
 * Permission is hereby granted, free of charge, to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of this software,
 * subject to inclusion of this copyright notice in all copies or substantial portions of the software.
 */

#include <MCP23S17.h>

MCP23S17::MCP23S17(SPI_HandleTypeDef*hspi,GPIO_TypeDef*cs_port,uint16_t cs_pin,GPIO_TypeDef*rst_port,uint16_t rst_pin){
    _hspi=hspi;
    _cs_port=cs_port;
    _cs_pin=cs_pin;
    _rst_port=rst_port;
    _rst_pin=rst_pin;
}
MCP23S17::~MCP23S17(){

}
void MCP23S17::init(){
    // Initialization code and reset of the IC
}