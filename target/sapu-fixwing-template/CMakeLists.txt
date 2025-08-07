SET(TARGET_NAME sapu-fixwing)
SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_PROCESSOR cortex-m7)
#SET(CMAKE_VERBOSE_MAKEFILE ON)

SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

SET(CMAKE_C_FLAGS " -mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=hard -ffunction-sections -fdata-sections -Dgcc -O0 -gdwarf-2 -g -std=c99")
SET(CMAKE_ASM_FLAGS " -c -mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=hard -ffunction-sections -fdata-sections -x assembler-with-cpp -Wa,-mimplicit-it=thumb  -gdwarf-2")
SET(CMAKE_C_COMPILER_WORKS TRUE)

SET(CMAKE_CXX_FLAGS " -mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=hard -ffunction-sections -fdata-sections -Dgcc -O0 -gdwarf-2 -g")
SET(CMAKE_CXX_COMPILER_WORKS TRUE)

SET(CMAKE_OBJCOPY "E:/Project/Study/Autopilot/.env/tools/bin/../../tools/gnu_gcc/arm_gcc/mingw/bin/arm-none-eabi-objcopy.exe")
SET(CMAKE_SIZE "E:/Project/Study/Autopilot/.env/tools/bin/../../tools/gnu_gcc/arm_gcc/mingw/bin/arm-none-eabi-size.exe")

SET(CMAKE_EXE_LINKER_FLAGS " -mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=hard -ffunction-sections -fdata-sections -Wl,--gc-sections,-Map=rtthread.map,-cref,-u,Reset_Handler -T ${CMAKE_CURRENT_SOURCE_DIR}/board/linker_scripts/link.lds -Wl,--gc-sections,--print-memory-usage")


ADD_EXECUTABLE(${TARGET_NAME}.elf)

SET(COMMON_INCLUDE_DIR	.
	applications
	board
	board/CubeMX_Config/Core/Inc
	packages/optparse-latest
	../../rt-thread/include
	../../rt-thread/libcpu/arm/common
	../../rt-thread/libcpu/arm/cortex-m7
	../../rt-thread/components/dfs/dfs_v1/include
	../../rt-thread/components/dfs/dfs_v1/filesystems/elmfat
	../../rt-thread/components/drivers/include
	../../rt-thread/components/drivers/ktime
	../../rt-thread/components/drivers/ktime/inc
	../../rt-thread/components/drivers/phy
	../../rt-thread/components/drivers/sdio/sdhci/include
	../../rt-thread/components/drivers/smp_call
	../../rt-thread/components/drivers/spi
	../../rt-thread/components/finsh
	../../rt-thread/components/libc/compilers/common/include
	../../rt-thread/components/libc/compilers/newlib
	../../rt-thread/components/libc/posix/delay
	../../rt-thread/components/libc/posix/io/epoll
	../../rt-thread/components/libc/posix/io/eventfd
	../../rt-thread/components/libc/posix/io/poll
	../../rt-thread/components/libc/posix/ipc
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Inc
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/CMSIS/Device/ST/STM32H7xx/Include
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Inc/Legacy
	../../rt-thread/bsp/stm32/libraries/HAL_Drivers/CMSIS/Include
)

target_include_directories(${TARGET_NAME}.elf PUBLIC ${COMMON_INCLUDE_DIR})
SET(COMMON_DEFINITIONS 
	-DSTM32H743xx
	-DDATA_IN_D2_SRAM
	-D__RTTHREAD__
	-DRT_USING_NEWLIBC
	-DRT_USING_LIBC
	-D_POSIX_C_SOURCE=1
	-DUSE_HAL_DRIVER
	-DUSE_FULL_LL_DRIVER
)

target_compile_definitions(${TARGET_NAME}.elf PRIVATE ${COMMON_DEFINITIONS})
# Library source files
SET(RT_APPLICATIONS_SOURCES
	applications/sd_hotplug.c
	applications/rgb.c
)

SET(RT_BOARD_DRIVERS_SOURCES
	board/drv_usart_v2.c
	board/drv_systick.c
	board/drv_can.c
	board/CubeMX_Config/Core/Src/stm32h7xx_hal_msp.c
	board/CubeMX_Config/Core/Src/main.c
	board/drv_gpio.c
	board/drv_pwm.c
	board/drv_soft_i2c.c
	board/drv_sdmmc.c
	board/drv_spi.c
	board/CubeMX_Config/Core/Src/system_stm32h7xx.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/CMSIS/Device/ST/STM32H7xx/Source/Templates/gcc/startup_stm32h743xx.s
	board/board.c
	board/drv_adc.c
)

SET(RT_COMPILER_SOURCES
	../../rt-thread/components/libc/compilers/common/cctype.c
	../../rt-thread/components/libc/compilers/common/cstdlib.c
	../../rt-thread/components/libc/compilers/common/cstring.c
	../../rt-thread/components/libc/compilers/common/ctime.c
	../../rt-thread/components/libc/compilers/common/cunistd.c
	../../rt-thread/components/libc/compilers/common/cwchar.c
	../../rt-thread/components/libc/compilers/newlib/syscalls.c
)

SET(RT_DEVICEDRIVERS_SOURCES
	../../rt-thread/components/drivers/block/blk.c
	../../rt-thread/components/drivers/block/blk_dev.c
	../../rt-thread/components/drivers/block/blk_dfs.c
	../../rt-thread/components/drivers/block/blk_partition.c
	../../rt-thread/components/drivers/can/dev_can.c
	../../rt-thread/components/drivers/core/device.c
	../../rt-thread/components/drivers/i2c/dev_i2c_bit_ops.c
	../../rt-thread/components/drivers/i2c/dev_i2c_core.c
	../../rt-thread/components/drivers/i2c/dev_i2c_dev.c
	../../rt-thread/components/drivers/ipc/completion_comm.c
	../../rt-thread/components/drivers/ipc/completion_up.c
	../../rt-thread/components/drivers/ipc/condvar.c
	../../rt-thread/components/drivers/ipc/dataqueue.c
	../../rt-thread/components/drivers/ipc/pipe.c
	../../rt-thread/components/drivers/ipc/ringblk_buf.c
	../../rt-thread/components/drivers/ipc/ringbuffer.c
	../../rt-thread/components/drivers/ipc/waitqueue.c
	../../rt-thread/components/drivers/ipc/workqueue.c
	../../rt-thread/components/drivers/misc/adc.c
	../../rt-thread/components/drivers/pin/dev_pin.c
	../../rt-thread/components/drivers/rtc/dev_rtc.c
	../../rt-thread/components/drivers/rtc/dev_soft_rtc.c
	../../rt-thread/components/drivers/sdio/dev_block.c
	../../rt-thread/components/drivers/sdio/dev_mmc.c
	../../rt-thread/components/drivers/sdio/dev_mmcsd_core.c
	../../rt-thread/components/drivers/sdio/dev_sd.c
	../../rt-thread/components/drivers/sdio/dev_sdio.c
	../../rt-thread/components/drivers/serial/dev_serial_v2.c
	../../rt-thread/components/drivers/spi/dev_spi.c
	../../rt-thread/components/drivers/spi/dev_spi_core.c
)

SET(RT_DRIVERS_SOURCES
)

SET(RT_FILESYSTEM_SOURCES
	../../rt-thread/components/dfs/dfs_v1/filesystems/elmfat/dfs_elm.c
	../../rt-thread/components/dfs/dfs_v1/filesystems/elmfat/ff.c
	../../rt-thread/components/dfs/dfs_v1/filesystems/elmfat/ffunicode.c
	../../rt-thread/components/dfs/dfs_v1/src/dfs.c
	../../rt-thread/components/dfs/dfs_v1/src/dfs_file.c
	../../rt-thread/components/dfs/dfs_v1/src/dfs_fs.c
	../../rt-thread/components/dfs/dfs_v1/src/dfs_posix.c
)

SET(RT_FINSH_SOURCES
	../../rt-thread/components/finsh/shell.c
	../../rt-thread/components/finsh/cmd.c
	../../rt-thread/components/finsh/msh_parse.c
	../../rt-thread/components/finsh/msh_file.c
	../../rt-thread/components/finsh/msh.c
)

SET(RT_KERNEL_SOURCES
	../../rt-thread/src/clock.c
	../../rt-thread/src/components.c
	../../rt-thread/src/cpu_up.c
	../../rt-thread/src/defunct.c
	../../rt-thread/src/idle.c
	../../rt-thread/src/ipc.c
	../../rt-thread/src/irq.c
	../../rt-thread/src/kservice.c
	../../rt-thread/src/memheap.c
	../../rt-thread/src/object.c
	../../rt-thread/src/scheduler_comm.c
	../../rt-thread/src/scheduler_up.c
	../../rt-thread/src/thread.c
	../../rt-thread/src/timer.c
)

SET(RT_KLIBC_SOURCES
	../../rt-thread/src/klibc/rt_vsscanf.c
	../../rt-thread/src/klibc/kstdio.c
	../../rt-thread/src/klibc/kerrno.c
	../../rt-thread/src/klibc/rt_vsnprintf_tiny.c
	../../rt-thread/src/klibc/kstring.c
)

SET(RT_KTIME_SOURCES
	../../rt-thread/components/drivers/ktime/src/cputimer.c
	../../rt-thread/components/drivers/ktime/src/boottime.c
	../../rt-thread/components/drivers/ktime/src/hrtimer.c
)

SET(RT_LIBCPU_SOURCES
	../../rt-thread/libcpu/arm/common/atomic_arm.c
	../../rt-thread/libcpu/arm/common/div0.c
	../../rt-thread/libcpu/arm/common/showmem.c
	../../rt-thread/libcpu/arm/cortex-m7/context_gcc.S
	../../rt-thread/libcpu/arm/cortex-m7/cpu_cache.c
	../../rt-thread/libcpu/arm/cortex-m7/cpuport.c
)

SET(RT_OPTPARSE_SOURCES
	packages/optparse-latest/optparse.c
)

SET(RT_POSIX_SOURCES
	../../rt-thread/components/libc/posix/delay/delay.c
)

SET(RT_SMP_SOURCES
)

SET(RT_STM32_HAL_SOURCES
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_eth_ex.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hcd.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rng.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sdram.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart_ex.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dts.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_delayblock.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cryp_ex.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hrtim.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sd_ex.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_rcc.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_rtc.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_exti.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rng_ex.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_gpio.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dcmi.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gfxmmu.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pcd_ex.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_smbus_ex.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_usart.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_utils.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dac_ex.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pcd.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_hrtim.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_crc_ex.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hash_ex.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_opamp.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_spdifrx.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_wwdg.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_adc_ex.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_lptim.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_i2c.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_swpmi.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dfsdm_ex.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c_ex.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_spi.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_spi_ex.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_dma.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cec.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mdma.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_qspi.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_lptim.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_rng.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma_ex.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_smartcard_ex.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sram.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_adc.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_comp.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mdios.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_smbus.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_dac.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_tim.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_fdcan.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_ltdc.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_nor.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_swpmi.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_usart_ex.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mmc_ex.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_ospi.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_dma2d.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_usb.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hash.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_comp.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hsem.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_ramecc.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_exti.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cryp.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2s_ex.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rtc.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_fmac.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cordic.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2s.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_opamp_ex.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_lpuart.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_eth.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mmc.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_fmc.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_ltdc_ex.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sai_ex.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_mdma.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash_ex.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rtc_ex.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim_ex.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_crc.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dac.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_jpeg.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_smartcard.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_opamp.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dfsdm.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_nand.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_cordic.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_spi.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_iwdg.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sai.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_crc.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_fmac.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_irda.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_otfdec.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_crs.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_sdmmc.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_adc.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dsi.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sd.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_pwr.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_usart.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma2d.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pssi.c
	../../rt-thread/bsp/stm32/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_bdma.c
)

SET(RT_UTESTCASES_SOURCES
)

# Library search paths
# Library local macro definitions
SET(RT_DEVICEDRIVERS_DEFINES
	__RT_IPC_SOURCE__
)

SET(RT_KERNEL_DEFINES
	__RT_KERNEL_SOURCE__
)

# Library dependencies
SET(RT_COMPILER_LIBS
	c
	m
)

# Libraries
ADD_LIBRARY(sapu-fixwing-Board_Drivers OBJECT ${RT_BOARD_DRIVERS_SOURCES})
target_include_directories(sapu-fixwing-Board_Drivers PUBLIC ${COMMON_INCLUDE_DIR})
target_compile_definitions(sapu-fixwing-Board_Drivers PUBLIC ${COMMON_DEFINITIONS})

ADD_LIBRARY(sapu-fixwing-Compiler OBJECT ${RT_COMPILER_SOURCES})
target_include_directories(sapu-fixwing-Compiler PUBLIC ${COMMON_INCLUDE_DIR})
target_compile_definitions(sapu-fixwing-Compiler PUBLIC ${COMMON_DEFINITIONS})

ADD_LIBRARY(sapu-fixwing-DeviceDrivers OBJECT ${RT_DEVICEDRIVERS_SOURCES})
target_include_directories(sapu-fixwing-DeviceDrivers PUBLIC ${COMMON_INCLUDE_DIR})
target_compile_definitions(sapu-fixwing-DeviceDrivers PUBLIC ${COMMON_DEFINITIONS})

ADD_LIBRARY(sapu-fixwing-Filesystem OBJECT ${RT_FILESYSTEM_SOURCES})
target_include_directories(sapu-fixwing-Filesystem PUBLIC ${COMMON_INCLUDE_DIR})
target_compile_definitions(sapu-fixwing-Filesystem PUBLIC ${COMMON_DEFINITIONS})

ADD_LIBRARY(sapu-fixwing-Finsh OBJECT ${RT_FINSH_SOURCES})
target_include_directories(sapu-fixwing-Finsh PUBLIC ${COMMON_INCLUDE_DIR})
target_compile_definitions(sapu-fixwing-Finsh PUBLIC ${COMMON_DEFINITIONS})

ADD_LIBRARY(sapu-fixwing-Kernel OBJECT ${RT_KERNEL_SOURCES})
target_include_directories(sapu-fixwing-Kernel PUBLIC ${COMMON_INCLUDE_DIR})
target_compile_definitions(sapu-fixwing-Kernel PUBLIC ${COMMON_DEFINITIONS})

ADD_LIBRARY(sapu-fixwing-klibc OBJECT ${RT_KLIBC_SOURCES})
target_include_directories(sapu-fixwing-klibc PUBLIC ${COMMON_INCLUDE_DIR})
target_compile_definitions(sapu-fixwing-klibc PUBLIC ${COMMON_DEFINITIONS})

ADD_LIBRARY(sapu-fixwing-ktime OBJECT ${RT_KTIME_SOURCES})
target_include_directories(sapu-fixwing-ktime PUBLIC ${COMMON_INCLUDE_DIR})
target_compile_definitions(sapu-fixwing-ktime PUBLIC ${COMMON_DEFINITIONS})

ADD_LIBRARY(sapu-fixwing-libcpu OBJECT ${RT_LIBCPU_SOURCES})
target_include_directories(sapu-fixwing-libcpu PUBLIC ${COMMON_INCLUDE_DIR})
target_compile_definitions(sapu-fixwing-libcpu PUBLIC ${COMMON_DEFINITIONS})

ADD_LIBRARY(sapu-fixwing-optparse OBJECT ${RT_OPTPARSE_SOURCES})
target_include_directories(sapu-fixwing-optparse PUBLIC ${COMMON_INCLUDE_DIR})
target_compile_definitions(sapu-fixwing-optparse PUBLIC ${COMMON_DEFINITIONS})

ADD_LIBRARY(sapu-fixwing-POSIX OBJECT ${RT_POSIX_SOURCES})
target_include_directories(sapu-fixwing-POSIX PUBLIC ${COMMON_INCLUDE_DIR})
target_compile_definitions(sapu-fixwing-POSIX PUBLIC ${COMMON_DEFINITIONS})

ADD_LIBRARY(sapu-fixwing-STM32_HAL OBJECT ${RT_STM32_HAL_SOURCES})
target_include_directories(sapu-fixwing-STM32_HAL PUBLIC ${COMMON_INCLUDE_DIR})
target_compile_definitions(sapu-fixwing-STM32_HAL PUBLIC ${COMMON_DEFINITIONS})


# Interface libraries
ADD_LIBRARY(sapu-fixwing-Drivers INTERFACE)
target_include_directories(sapu-fixwing-Drivers INTERFACE ${COMMON_INCLUDE_DIR})
target_compile_definitions(sapu-fixwing-Drivers INTERFACE ${COMMON_DEFINITIONS})

ADD_LIBRARY(sapu-fixwing-smp INTERFACE)
target_include_directories(sapu-fixwing-smp INTERFACE ${COMMON_INCLUDE_DIR})
target_compile_definitions(sapu-fixwing-smp INTERFACE ${COMMON_DEFINITIONS})

ADD_LIBRARY(sapu-fixwing-utestcases INTERFACE)
target_include_directories(sapu-fixwing-utestcases INTERFACE ${COMMON_INCLUDE_DIR})
target_compile_definitions(sapu-fixwing-utestcases INTERFACE ${COMMON_DEFINITIONS})


# Private macros
TARGET_COMPILE_DEFINITIONS(sapu-fixwing-DeviceDrivers PRIVATE ${RT_DEVICEDRIVERS_DEFINES})
TARGET_COMPILE_DEFINITIONS(sapu-fixwing-Kernel PRIVATE ${RT_KERNEL_DEFINES})

# Interface library search paths
TARGET_LINK_LIBRARIES(sapu-fixwing-Compiler INTERFACE ${RT_COMPILER_LIBS})

target_sources(${TARGET_NAME}.elf PUBLIC ${RT_APPLICATIONS_SOURCES})
TARGET_LINK_LIBRARIES(${TARGET_NAME}.elf PUBLIC 
	sapu-fixwing-Board_Drivers
	sapu-fixwing-Compiler
	sapu-fixwing-DeviceDrivers
	sapu-fixwing-Filesystem
	sapu-fixwing-Finsh
	sapu-fixwing-Kernel
	sapu-fixwing-klibc
	sapu-fixwing-ktime
	sapu-fixwing-libcpu
	sapu-fixwing-optparse
	sapu-fixwing-POSIX
	sapu-fixwing-STM32_HAL
	sapu-fixwing-Drivers
	sapu-fixwing-smp
	sapu-fixwing-utestcases
)

ADD_CUSTOM_COMMAND(TARGET ${TARGET_NAME}.elf POST_BUILD 
	COMMAND ${CMAKE_OBJCOPY} -O binary ${TARGET_NAME}.elf sapu-fixwing.bin
	COMMAND ${CMAKE_SIZE} ${TARGET_NAME}.elf
)

# if custom.cmake is exist, add it
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/custom.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/custom.cmake)
endif()
