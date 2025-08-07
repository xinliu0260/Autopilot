cmake_minimum_required(VERSION 3.22)

project(stm32cubemx)
add_library(stm32cubemx INTERFACE)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

target_compile_definitions(stm32cubemx INTERFACE 
	USE_PWR_LDO_SUPPLY 
	USE_HAL_DRIVER 
	STM32H743xx
    $<$<CONFIG:Debug>:DEBUG>
)

target_include_directories(stm32cubemx INTERFACE
    ../../Core/Inc
    C:/EmbedDevTool/STM32CubeMX/Repository/STM32Cube_FW_H7_V1.10.0/Drivers/STM32H7xx_HAL_Driver/Inc
    C:/EmbedDevTool/STM32CubeMX/Repository/STM32Cube_FW_H7_V1.10.0/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy
    C:/EmbedDevTool/STM32CubeMX/Repository/STM32Cube_FW_H7_V1.10.0/Drivers/CMSIS/Device/ST/STM32H7xx/Include
    C:/EmbedDevTool/STM32CubeMX/Repository/STM32Cube_FW_H7_V1.10.0/Drivers/CMSIS/Include
)

target_sources(stm32cubemx INTERFACE
    ../../Core/Src/main.c
    ../../Core/Src/stm32h7xx_it.c
    ../../Core/Src/stm32h7xx_hal_msp.c
    C:/EmbedDevTool/STM32CubeMX/Repository/STM32Cube_FW_H7_V1.10.0/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_adc.c
    C:/EmbedDevTool/STM32CubeMX/Repository/STM32Cube_FW_H7_V1.10.0/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_adc_ex.c
    C:/EmbedDevTool/STM32CubeMX/Repository/STM32Cube_FW_H7_V1.10.0/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.c
    C:/EmbedDevTool/STM32CubeMX/Repository/STM32Cube_FW_H7_V1.10.0/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex.c
    C:/EmbedDevTool/STM32CubeMX/Repository/STM32Cube_FW_H7_V1.10.0/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash.c
    C:/EmbedDevTool/STM32CubeMX/Repository/STM32Cube_FW_H7_V1.10.0/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash_ex.c
    C:/EmbedDevTool/STM32CubeMX/Repository/STM32Cube_FW_H7_V1.10.0/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.c
    C:/EmbedDevTool/STM32CubeMX/Repository/STM32Cube_FW_H7_V1.10.0/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hsem.c
    C:/EmbedDevTool/STM32CubeMX/Repository/STM32Cube_FW_H7_V1.10.0/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma.c
    C:/EmbedDevTool/STM32CubeMX/Repository/STM32Cube_FW_H7_V1.10.0/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma_ex.c
    C:/EmbedDevTool/STM32CubeMX/Repository/STM32Cube_FW_H7_V1.10.0/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mdma.c
    C:/EmbedDevTool/STM32CubeMX/Repository/STM32Cube_FW_H7_V1.10.0/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr.c
    C:/EmbedDevTool/STM32CubeMX/Repository/STM32Cube_FW_H7_V1.10.0/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.c
    C:/EmbedDevTool/STM32CubeMX/Repository/STM32Cube_FW_H7_V1.10.0/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex.c
    C:/EmbedDevTool/STM32CubeMX/Repository/STM32Cube_FW_H7_V1.10.0/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.c
    C:/EmbedDevTool/STM32CubeMX/Repository/STM32Cube_FW_H7_V1.10.0/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c.c
    C:/EmbedDevTool/STM32CubeMX/Repository/STM32Cube_FW_H7_V1.10.0/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c_ex.c
    C:/EmbedDevTool/STM32CubeMX/Repository/STM32Cube_FW_H7_V1.10.0/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_exti.c
    C:/EmbedDevTool/STM32CubeMX/Repository/STM32Cube_FW_H7_V1.10.0/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_fdcan.c
    C:/EmbedDevTool/STM32CubeMX/Repository/STM32Cube_FW_H7_V1.10.0/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_sdmmc.c
    C:/EmbedDevTool/STM32CubeMX/Repository/STM32Cube_FW_H7_V1.10.0/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_delayblock.c
    C:/EmbedDevTool/STM32CubeMX/Repository/STM32Cube_FW_H7_V1.10.0/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sd.c
    C:/EmbedDevTool/STM32CubeMX/Repository/STM32Cube_FW_H7_V1.10.0/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sd_ex.c
    C:/EmbedDevTool/STM32CubeMX/Repository/STM32Cube_FW_H7_V1.10.0/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mmc.c
    C:/EmbedDevTool/STM32CubeMX/Repository/STM32Cube_FW_H7_V1.10.0/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mmc_ex.c
    C:/EmbedDevTool/STM32CubeMX/Repository/STM32Cube_FW_H7_V1.10.0/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_spi.c
    C:/EmbedDevTool/STM32CubeMX/Repository/STM32Cube_FW_H7_V1.10.0/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_spi_ex.c
    C:/EmbedDevTool/STM32CubeMX/Repository/STM32Cube_FW_H7_V1.10.0/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim.c
    C:/EmbedDevTool/STM32CubeMX/Repository/STM32Cube_FW_H7_V1.10.0/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim_ex.c
    C:/EmbedDevTool/STM32CubeMX/Repository/STM32Cube_FW_H7_V1.10.0/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart.c
    C:/EmbedDevTool/STM32CubeMX/Repository/STM32Cube_FW_H7_V1.10.0/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart_ex.c
    ../../Core/Src/system_stm32h7xx.c
    ../../Core/Src/sysmem.c
    ../../Core/Src/syscalls.c
    ../../startup_stm32h743xx.s
)

target_link_directories(stm32cubemx INTERFACE
)

target_link_libraries(stm32cubemx INTERFACE
)

# Validate that STM32CubeMX code is compatible with C standard
if(CMAKE_C_STANDARD LESS 11)
    message(ERROR "Generated code requires C11 or higher")
endif()


