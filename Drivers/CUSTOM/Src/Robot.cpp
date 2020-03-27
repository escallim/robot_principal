#include "Robot.h"

#include <stdio.h>

robot_t::robot_t(MCP233 controller, odometry_t odometry) : mcp{controller}, odo{odometry}, current_state{robot_state_t::IDLE}
{
	printf("Current state %s\n", robot_state_str[(int)current_state]);
}


robot_state_t robot_t::get_state()
{
	return current_state;
}

color_t robot_t::get_color()
{
	switch( HAL_GPIO_ReadPin(couleur_GPIO_Port, couleur_Pin) )
	{
	case GPIO_PIN_RESET:
		return color_t::YELLOW;
		break;
	case GPIO_PIN_SET:
		return color_t::BLUE;
		break;
	}
	return color_t::ERROR;
}

void robot_t::event(robot_event_t event)
{
	printf("Event %s received\n", robot_event_str[(int)event]);
	switch(current_state)
	{
	case robot_state_t::IDLE:
		switch(event)
		{
		case robot_event_t::START:
			current_state = robot_state_t::AUTOMATIC;
			reset_timer();
			break;
		case robot_event_t::STOP:
			break;

		case robot_event_t::TEAM_COLOR_CHANGE:
			printf("2 %i\n", (int)get_color());
			break;

		case robot_event_t::SUPERVISOR_CMD_AUTO:
			break;
		case robot_event_t::SUPERVISOR_CMD_MANU:
			current_state = robot_state_t::MANUAL;
			break;
		}
		break;

	case robot_state_t::AUTOMATIC:
		switch(event)
		{
		case robot_event_t::START:
			break;
		case robot_event_t::STOP:
			//TODO STOPPER ROBOT
			current_state = robot_state_t::DEAD;
			break;

		case robot_event_t::TEAM_COLOR_CHANGE:
			current_state = robot_state_t::IDLE;
			break;

		case robot_event_t::SUPERVISOR_CMD_AUTO:
			break;
		case robot_event_t::SUPERVISOR_CMD_MANU:
			current_state = robot_state_t::MANUAL;
			break;
		}
		break;

	case robot_state_t::MANUAL:
		switch(event)
		{
		case robot_event_t::START:
			break;
		case robot_event_t::STOP:
			break;

		case robot_event_t::TEAM_COLOR_CHANGE:
			printf("2 %i\n", (int)get_color());
			break;

		case robot_event_t::SUPERVISOR_CMD_AUTO:
			current_state = robot_state_t::IDLE;
			break;
		case robot_event_t::SUPERVISOR_CMD_MANU:
			break;
		}
		break;

	case robot_state_t::DEAD:
		switch(event)
		{
		case robot_event_t::START:
		case robot_event_t::STOP:
		case robot_event_t::TEAM_COLOR_CHANGE:
		case robot_event_t::SUPERVISOR_CMD_AUTO:
		case robot_event_t::SUPERVISOR_CMD_MANU:
			break;
		}
		break;
	}

	printf("Current state %s\n", robot_state_str[(int)current_state]);
}



void robot_t::cmd(robot_cmd_t cmd)
{
	printf("Command %s received\n", robot_cmd_str[(int)cmd]);
	if( get_state() == robot_state_t::MANUAL)
	{
		switch(cmd)
		{
		case robot_cmd_t::HALT:
			mcp.drive_forward_M1(0);
			mcp.drive_forward_M2(0);
			break;
		case robot_cmd_t::GO_FORWARD:
			mcp.drive_forward(24);
			break;
		case robot_cmd_t::GO_BACKWARD:
			mcp.drive_backward(24);
			break;
		case robot_cmd_t::TURN_LEFT:
			mcp.drive_forward_M1(8);
			mcp.drive_backward_M2(8);
			break;
		case robot_cmd_t::TURN_RIGHT:
			mcp.drive_backward_M1(8);
			mcp.drive_forward_M2(8);
			break;
		}
	}
}

