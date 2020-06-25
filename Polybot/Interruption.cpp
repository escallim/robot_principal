#include "main.h"

#include <stdio.h>

#include "Robot.h"
#include "commandes.h"
#include "Odometry.h"

extern UART_HandleTypeDef huart2;
extern robot_t robot;
extern odometry_t odometry;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == demarrage_Pin)
	{
		robot.event(robot_event_t::START);
	}

	if(GPIO_Pin == couleur_Pin)
	{
		robot.event(robot_event_t::TEAM_COLOR_CHANGE);
	}
}

#define RX_SIZE 2
uint8_t rx_buffer[RX_SIZE];

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
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
}


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	__NOP();
}
