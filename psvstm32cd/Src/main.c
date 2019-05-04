#include "main.h"
#include "error_handler.h"
#include "usb_device.h"
#include "gpio.h"
#include "sys_clk.h"

#include "game_card.h"

int main(void)
{
   // reset peripherals, initialize
   HAL_Init();

   // configure system clock
   SystemClock_Config();

   // initialize gpio
   MX_GPIO_Init();
   
   HAL_Delay(100);
   
   // somehow initialization of card called from STORAGE_Init_FS does not work - hangs forever
   
   // write to blue pin - indicate initialization
   HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
   
   MBR mbr;

   if(StartGameCardInit(&mbr) != HAL_OK)
   {
      // toggle red pin if game card initialization has failed
      HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
   }
   else
   {
      // toggle once green pin if everything is ok
      HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
      HAL_Delay(500);
      HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
      HAL_Delay(500);
   }
   
   // write to blue pin - indicate that initialization is over
   HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
   
   HAL_Delay(100);
   
   // write to blue pin - indicate initialization
   HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
   
   // initialize usb
   if(MX_USB_DEVICE_Init() != USBD_OK)
   {
      // toggle red pin if MMC initialization has failed
      HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
   }
   else
   {
      // toggle twice green pin if everything is ok
      HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
      HAL_Delay(500);
      HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
      HAL_Delay(500);
      HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
      HAL_Delay(500);
      HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
      HAL_Delay(500);
   }
   
   // write to blue pin - indicate that initialization is over
   HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
   
   while (1)
   {
   }

   // code for debugging
   /*
   int button_hold_counter = 0;
   
   while (1)
   {
      //toggle orange pin (show that device is ready by blinking the led)
      //HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
      
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
            // initialize usb
            if(MX_USB_DEVICE_Init() != USBD_OK)
            {
               // toggle red pin if MMC initialization has failed
               HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
            }
            else
            {
               // toggle twice green pin if everything is ok
               HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
               HAL_Delay(500);
               HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
               HAL_Delay(500);
               HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
               HAL_Delay(500);
               HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
               HAL_Delay(500);
            }
            
            button_hold_counter++;
         }
      }
      
      HAL_Delay(100);
   }
   */
}
