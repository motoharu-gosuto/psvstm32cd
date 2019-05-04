#include "sdio.h"

#include <string.h>

#include "stm32f4xx_ll_sdmmc.h"
#include "error_handler.h"

MMC_HandleTypeDef hmmc;

HAL_StatusTypeDef HAL_MMC_Init_Custom(MMC_HandleTypeDef *hmmc)
{
  // Check the MMC handle allocation
  if(hmmc == NULL)
  {
    return HAL_ERROR;
  }

  // Check the parameters
  assert_param(IS_SDIO_ALL_INSTANCE(hmmc->Instance));
  assert_param(IS_SDIO_CLOCK_EDGE(hmmc->Init.ClockEdge));
  assert_param(IS_SDIO_CLOCK_BYPASS(hmmc->Init.ClockBypass));
  assert_param(IS_SDIO_CLOCK_POWER_SAVE(hmmc->Init.ClockPowerSave));
  assert_param(IS_SDIO_BUS_WIDE(hmmc->Init.BusWide));
  assert_param(IS_SDIO_HARDWARE_FLOW_CONTROL(hmmc->Init.HardwareFlowControl));
  assert_param(IS_SDIO_CLKDIV(hmmc->Init.ClockDiv));

  // Call user defined init on reset
  if(hmmc->State == HAL_MMC_STATE_RESET)
  {
    // Allocate lock resource and initialize it
    hmmc->Lock = HAL_UNLOCKED;
    // Init the low level hardware : GPIO, CLOCK, CORTEX...etc 
    HAL_MMC_MspInit(hmmc);
  }

  hmmc->State = HAL_MMC_STATE_BUSY;

  // Initialize the Card parameters
  HAL_StatusTypeDef internal_error = HAL_MMC_InitCard(hmmc);
  if(internal_error != HAL_OK)
     return internal_error;

  // Initialize the error code
  hmmc->ErrorCode = HAL_DMA_ERROR_NONE;
  
  // Initialize the MMC operation
  hmmc->Context = MMC_CONTEXT_NONE;

  // Initialize the MMC state
  hmmc->State = HAL_MMC_STATE_READY;

  return HAL_OK;
}

// SDIO Initialization Function
HAL_StatusTypeDef MX_SDIO_MMC_Init(void)
{
   hmmc.Instance = SDIO;
   hmmc.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
   hmmc.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
   hmmc.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_ENABLE;
   hmmc.Init.BusWide = SDIO_BUS_WIDE_1B;
   hmmc.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
   hmmc.Init.ClockDiv = SDIO_INIT_CLK_DIV;

   // call CUSTOM low level initialization
   // (because usual initialization does not return an error properly)
   if (HAL_MMC_Init_Custom(&hmmc) != HAL_OK)
   {
      Error_Handler();
      return HAL_ERROR;
   }
   else
   {
      // switch to high speed mode
      if(SDMMC_CmdSwitch(hmmc.Instance, 0x03B90100U) != HAL_MMC_ERROR_NONE)
      {
         Error_Handler();
         return HAL_ERROR;
      }
      else
      {
         if (HAL_MMC_ConfigWideBusOperation(&hmmc, SDIO_BUS_WIDE_4B) != HAL_OK) //SDIO_BUS_WIDE_1B 
         {
            Error_Handler();
            return HAL_ERROR;
         }
         else
         {
            //switch to lower freqency for debug
            /*
            hmmc.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
            hmmc.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
            hmmc.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_ENABLE;
            hmmc.Init.BusWide = SDIO_BUS_WIDE_4B;
            hmmc.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
            hmmc.Init.ClockDiv = SDIO_INIT_CLK_DIV;

            SDIO_Init(hmmc.Instance, hmmc.Init);
            */
            
            return HAL_OK;
         }
      }
   }
}

// MMC MSP Initialization
void HAL_MMC_MspInit(MMC_HandleTypeDef* hmmc)
{
   GPIO_InitTypeDef GPIO_InitStruct = {0};

   if(hmmc->Instance==SDIO)
   {
      __HAL_RCC_SDIO_CLK_ENABLE(); // enable SDIO device clock

      __HAL_RCC_GPIOC_CLK_ENABLE(); //enabling clock for C pins
      __HAL_RCC_GPIOD_CLK_ENABLE(); //enabling clock for D pins

      // SDIO GPIO Configuration    
      // PC8     ------> SDIO_D0
      // PC9     ------> SDIO_D1
      // PC10     ------> SDIO_D2
      // PC11     ------> SDIO_D3
      // PC12     ------> SDIO_CK
      // PD2     ------> SDIO_CMD 

      //configure data pins
      GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull = GPIO_PULLUP; // use pullup (required by MMC standard)
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
      GPIO_InitStruct.Alternate = GPIO_AF12_SDIO; // enable SDIO function for pins
      HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

      //configure ck pin
      GPIO_InitStruct.Pin = GPIO_PIN_12;
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
      GPIO_InitStruct.Alternate = GPIO_AF12_SDIO; // enable SDIO function for pins
      HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

      //configure cmd pin
      GPIO_InitStruct.Pin = GPIO_PIN_2;
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull = GPIO_PULLUP; // use pullup (required by MMC standard)
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
      GPIO_InitStruct.Alternate = GPIO_AF12_SDIO; // enable SDIO function for pins
      HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
   }
}

void HAL_MMC_MspDeInit(MMC_HandleTypeDef* hmmc)
{
   if(hmmc->Instance==SDIO)
   {
      __HAL_RCC_SDIO_CLK_DISABLE(); // disable SDIO device clock

      // SDIO GPIO Configuration    
      // PC8     ------> SDIO_D0
      // PC9     ------> SDIO_D1
      // PC10     ------> SDIO_D2
      // PC11     ------> SDIO_D3
      // PC12     ------> SDIO_CK
      // PD2     ------> SDIO_CMD 

      HAL_GPIO_DeInit(GPIOC, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11 
                       |GPIO_PIN_12);

      HAL_GPIO_DeInit(GPIOD, GPIO_PIN_2);
   }
}
