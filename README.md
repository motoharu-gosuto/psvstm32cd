# psvstm32cd
PSVita game card dumper based on STM32F407G Discovery board

How to setup, compile etc:

Copy 'psvstm32cd\psvstm32cd\Drivers\CMSIS' drivers from your local repo

Copy 'psvstm32cd\psvstm32cd\Drivers\STM32F4xx_HAL_Driver' drivers from your local repo

Copy 'psvstm32cd\psvstm32cd\Middlewares\ST\STM32_USB_Device_Library' library from your local repo

Apply patches to 'psvstm32cd\psvstm32cd\Drivers\STM32F4xx_HAL_Driver'

Make some generated, driver, library files readonly (for your own sake, so that you do not break them)

- all files in 'psvstm32cd\psvstm32cd\Drivers\STM32F4xx_HAL_Driver'
- 'psvstm32cd\psvstm32cd\MDK-ARM\startup_stm32f407xx.s'
- 'psvstm32cd\psvstm32cd\Src\system_stm32f4xx.c'
- 'psvstm32cd\psvstm32cd\Middlewares\Third_Party\mbedTLS'
- 'psvstm32cd\psvstm32cd\Middlewares\ST\STM32_USB_Device_Library'
- 'psvstm32cd\psvstm32cd\Src\stm32f4xx_it.c'
- 'psvstm32cd\psvstm32cd\Inc\stm32f4xx_it.h'
- 'psvstm32cd\psvstm32cd\Src\stm32f4xx_hal_msp.c'
- 'psvstm32cd\psvstm32cd\Inc\stm32f4xx_hal_conf.h'
- N:\psvstm32cd\psvstm32cd\Src\usbd_conf.c
- N:\psvstm32cd\psvstm32cd\Src\usbd_conf.h
- N:\psvstm32cd\psvstm32cd\Src\usbd_desc.c
- N:\psvstm32cd\psvstm32cd\Src\usbd_desc.h
