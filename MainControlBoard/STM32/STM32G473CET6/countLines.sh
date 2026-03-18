wc -l Utils/*.c* Utils/*.h*\
 Src/*.c* Include/*.h*\
 Devices/*/*.c* Devices/*/*.h\
 Tasks/*.c* Tasks/*h\
 Infra/Utils/*.c* Infra/Utils/*.h\
 Platform/Board/*.c* Platform/Board/*.h\
 Platform/Callbacks/*.c* Platform/Callbacks/*.h\
 Platform/LL/SPI1_Dma/*.c* Platform/LL/SPI1_Dma/*.h\
 Platform/LL/USART1_Dma/*.c* Platform/LL/USART1_Dma/*.h\
 Platform/Startup/*.c* Platform/Startup/*.h\
 Modules/nrf_radio_api/*.c* Modules/nrf_radio_api/*.h\
| sort -n | nl
