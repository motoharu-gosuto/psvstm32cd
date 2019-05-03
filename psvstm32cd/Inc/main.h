#ifndef __MAIN_H
   #define __MAIN_H

   #ifdef __cplusplus
   extern "C" {
   #endif

   #include "stm32f4xx_hal.h"
   
   #define B1_Pin GPIO_PIN_0
   #define B1_GPIO_Port GPIOA
   #define LD4_Pin GPIO_PIN_12
   #define LD4_GPIO_Port GPIOD
   #define LD3_Pin GPIO_PIN_13
   #define LD3_GPIO_Port GPIOD
   #define LD5_Pin GPIO_PIN_14
   #define LD5_GPIO_Port GPIOD
   #define LD6_Pin GPIO_PIN_15
   #define LD6_GPIO_Port GPIOD

   #ifdef __cplusplus
   }
   #endif

#endif
