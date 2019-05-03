#include "error_handler.h"

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
