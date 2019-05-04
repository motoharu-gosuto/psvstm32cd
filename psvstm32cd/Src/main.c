#include "main.h"
#include "error_handler.h"
#include "usb_device.h"

#include "gpio.h"
#include "sys_clk.h"

int button_hold_counter = 0;

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
            // initialize usb
            if(MX_USB_DEVICE_Init() == USBD_OK)
            {
               // toggle green pin if everything is ok
               HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);  
            }
            else
            {
               // toggle red pin if MMC initialization has failed
               HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
            }
            
            button_hold_counter++;
         }
      }
      
      HAL_Delay(100);
   }
}
