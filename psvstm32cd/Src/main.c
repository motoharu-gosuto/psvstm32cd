#include "main.h"
#include "error_handler.h"
#include "usb_device.h"
#include "sdio.h"
#include "gpio.h"
#include "sys_clk.h"
#include "cmd56.h"

int main(void)
{
   // reset peripherals, initialize
   HAL_Init();

   // configure system clock
   SystemClock_Config();

   // initialize gpio
   MX_GPIO_Init();

   // initialize sdio
   MX_SDIO_MMC_Init();

   // initialize usb
   MX_USB_DEVICE_Init();

   while (1)
   {
   }
}
