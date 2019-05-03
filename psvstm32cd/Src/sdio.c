#include "sdio.h"

#include <string.h>

#include "stm32f4xx_ll_sdmmc.h"

MMC_HandleTypeDef hmmc;

// SDIO Initialization Function
HAL_StatusTypeDef MX_SDIO_MMC_Init(void)
{
   hmmc.Instance = SDIO;
   hmmc.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
   hmmc.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
   hmmc.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_ENABLE;
   hmmc.Init.BusWide = SDIO_BUS_WIDE_1B;
   hmmc.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
   hmmc.Init.ClockDiv = 118;
  
   if (HAL_MMC_Init(&hmmc) != HAL_OK)
   {
    Error_Handler();
   }
  
   if (HAL_MMC_ConfigWideBusOperation(&hmmc, SDIO_BUS_WIDE_4B) != HAL_OK)
   {
    Error_Handler();
   }
}