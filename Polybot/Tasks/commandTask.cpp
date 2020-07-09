
/*
printf("%s",rx_buffer);

	switch( rx_buffer[0] )
	{
	case CMD_START:
		robot.event(robot_event_t::START);
		break;

	case CMD_STOP:
		robot.event(robot_event_t::STOP);
		break;

	case MODE_AUTO:
		robot.event(robot_event_t::SUPERVISOR_CMD_AUTO);
		break;

	case MODE_MANUAL:
		robot.event(robot_event_t::SUPERVISOR_CMD_MANU);
		break;

	case MOVE_HALT:
		robot.cmd(robot_cmd_t::HALT);
		break;

	case MOVE_GO_FORWARD:
		robot.cmd(robot_cmd_t::GO_FORWARD);
		break;

	case MOVE_GO_BACKWARD:
		robot.cmd(robot_cmd_t::GO_BACKWARD);
		break;

	case MOVE_TURN_LEFT:
		robot.cmd(robot_cmd_t::TURN_LEFT);
		break;

	case MOVE_TURN_RIGHT:
		robot.cmd(robot_cmd_t::TURN_RIGHT);

		break;
	case POS_ASK:
		printf("POS_ASK\n");
		odometry.send_pos();
		break;
	default:
		printf("GROSSE_MERDE\n");
		break;
	}
	HAL_UART_Receive_IT(&huart2, rx_buffer, RX_SIZE);
}*/

#include "main.h"
#include "cmsis_os.h"

#include "commandTask.h"

extern UART_HandleTypeDef huart2;

extern osThreadId commandTaskHandle;

extern uint8_t received_char;

#define SIZE_BUFFER 32
uint8_t bufferReception[SIZE_BUFFER];
uint8_t bufferResponse[SIZE_BUFFER];
uint8_t index = 0;

#define STX		0x02
#define ETX		0x03


uint8_t CRC8(const uint8_t *data, int length)
{
	uint8_t crc = 0x00;
	uint8_t extract;
	uint8_t sum;

	for(uint8_t i=0;i<length;i++)
	{
		extract = *data;
		for (char tempI = 8; tempI; tempI--)
		{
			sum = (crc ^ extract) & 0x01;
			crc >>= 1;
			if (sum)
				crc ^= 0x8C;
         extract >>= 1;
		}
		data++;
   }
   return crc;
}

typedef struct
{
	uint8_t commande;
} commandRequest_t;

typedef struct
{
	uint8_t commande;
	uint8_t size;
} commandResponse_t;

void decodeCommand(commandRequest_t *request, uint8_t *buffer, uint8_t length)
{

}

void processCommand(commandRequest_t *request, commandResponse_t *response)
{

}

void responseCommand(commandResponse_t *response)
{

}
/*
void StartCommandTask(void const * argument)
{
	commandTaskState_t state = INIT;
	commandRequest_t request;
	commandResponse_t response;

	while(1)
	{
		switch(state)
		{
		case INIT:
			HAL_UART_Receive_IT(&huart2, &received_char, 1);
			state = WAITING;
			break;

		case WAITING:
			ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
			state = PROCESS;
			break;

		case PROCESS:
			// calcul crc

			if(0)
			{
				state = INIT;
				break;
			}

			decodeCommand(&request, bufferReception, index);
			processCommand(&request, &response);
			responseCommand(&response, bufferResponse);

			state = SEND_RESPONSE;
			break;
		case SEND_RESPONSE:
			HAL_UART_Transmit(&huart2,bufferResponse,response->size);
			break
		}
	}
}
*/
void GetCharFromIT(uint8_t c)
{
	if(c == STX || index >= SIZE_BUFFER)
		index=0;

	bufferReception[index++] = c;

	if(c == ETX)
	{
		vTaskNotifyGiveFromISR(commandTaskHandle, NULL );
	}
	else
	{
		HAL_UART_Receive_IT(&huart2, &received_char, 1);
	}
}
