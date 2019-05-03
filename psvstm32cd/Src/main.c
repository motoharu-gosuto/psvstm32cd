#include "main.h"
#include "error_handler.h"
#include "usb_device.h"
#include "sdio.h"
#include "gpio.h"
#include "sys_clk.h"
#include "cmd56.h"

int button_hold_counter = 0;

HAL_StatusTypeDef StartGameCardInit()
{
   // first - perform basic MMC initialization
   if(MX_SDIO_MMC_Init() != HAL_OK)
   {
      // toggle red pin if MMC initialization has failed
      HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
      return HAL_ERROR;
   }
   else
   {
      // second - perform CMD56 initialization
      if(MX_SDIO_MMC_CMD56_Init() != HAL_OK)
      {
         // toggle red pin if CMD56 initialization has failed
         HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
         return HAL_ERROR;
      }
      else
      {
         uint8_t sector0[0x200];
         
         // perform read of first sector
         // it seems that timeout has to be large enough (2000) to be able to send first cmd17
         if(HAL_MMC_ReadBlocks(&hmmc, sector0, 0, 1, 2000) != HAL_OK)
         {
            // toggle red pin if read operation has failed
            HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
            return HAL_ERROR;
         }
         else
         {
            // toggle green pin if everything is ok
            HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
            return HAL_OK;
         }
      }
   }
}

int main(void)
{
   // reset peripherals, initialize
   HAL_Init();

   // configure system clock
   SystemClock_Config();

   // initialize gpio
   MX_GPIO_Init();

   while (1)
   {
      //toggle orange pin (show that device is ready by blinking the led)
      HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
      
      // read button input
      GPIO_PinState ps = HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin);
      
      // write to blue pin (led will be on till button is pressed)
      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, ps);
      
      // check if blue button was pressed
      if(ps == GPIO_PIN_SET)
      {
         // handle button press only once
         if(button_hold_counter == 0)
         {
            if(StartGameCardInit() == HAL_OK)
            {
               // initialize usb
               //MX_USB_DEVICE_Init();
            }
            
            button_hold_counter++;
         }
      }
      
      HAL_Delay(100);
   }
}
