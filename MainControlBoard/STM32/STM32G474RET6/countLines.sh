wc -l\
    Utils/*.c*\
    Utils/*.h*\
    Core/Src/main.c*\
    Include/*.h*\
    Devices/*/*.c*\
    Devices/*/*.h*\
    Tasks/*.c*\
    Tasks/*.h*\
    Infra/*.c*\
    Infra/*.h*\
    Platform/Board/*.c*\
    Platform/Board/*.h*\
    Platform/Callbacks/*.c*\
    Platform/Callbacks/*.h*\
    Platform/LL/SPI1_Dma/*.c*\
    Platform/LL/SPI1_Dma/*.h*\
    Platform/LL/USART3_Dma/*.c*\
    Platform/LL/USART3_Dma/*.h*\
    Platform/Startup/*.c*\
    Platform/Startup/*.h*\
| sort -n | nl
