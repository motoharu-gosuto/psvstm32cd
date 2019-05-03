#include "gpio.h"

//GPIO Initialization Function
void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  // GPIO Ports Clock Enable
  __HAL_RCC_GPIOH_CLK_ENABLE(); //enabling clock for ? pins
  __HAL_RCC_GPIOA_CLK_ENABLE(); //enabling clock for A pins
  __HAL_RCC_GPIOD_CLK_ENABLE(); //enabling clock for D pins
  __HAL_RCC_GPIOC_CLK_ENABLE(); //enabling clock for C pins

  // Configure GPIO pin Output Level (clear LED pins)
  HAL_GPIO_WritePin(GPIOD, LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin, GPIO_PIN_RESET);

  // Configure GPIO pin : B1_Pin (button pin)
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  //Configure GPIO pins : LD4_Pin LD3_Pin LD5_Pin LD6_Pin (LED pins)
  GPIO_InitStruct.Pin = LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}
