#include "main.h"
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

// This function is executed in case of error occurrence.
void Error_Handler(void)
{
}

#ifdef  USE_FULL_ASSERT
// Reports the name of the source file and the source line number where the assert_param error has occurred.
void assert_failed(uint8_t *file, uint32_t line)
{ 
}
#endif
