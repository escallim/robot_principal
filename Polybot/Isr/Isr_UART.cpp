#include "main.h"
#include "cmsis_os.h"


extern UART_HandleTypeDef huart2;

uint8_t received_char;
extern void GetCharFromIT(uint8_t c);



void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart2)
	{
		GetCharFromIT(received_char);
	}
}



void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	__NOP();
}
