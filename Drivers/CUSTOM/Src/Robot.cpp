#include "Robot.h"

robot_t::robot_t() : current_state{IDLE}
{
}

robot_state_t robot_t::get_state()
{
	return current_state;
}

void robot_t::transition(robot_event_t event)
{
	switch(current_state)
	{
	case IDLE:
		switch(event)
		{
		case START:
			current_state = AUTOMATIC;
			break;
		case TEAM_COLOR_CHANGE:
			break;
		case SUPERVISOR_CMD_AUTO:
			break;
		case SUPERVISOR_CMD_MANU:
			current_state = MANUAL;
			break;
		case STOP:
			break;
		}
		break;
	case AUTOMATIC:
		switch(event)
		{
		case START:
			break;
		case TEAM_COLOR_CHANGE:
			current_state = IDLE;
			break;
		case SUPERVISOR_CMD_AUTO:
			break;
		case SUPERVISOR_CMD_MANU:
			current_state = MANUAL;
			break;
		case STOP:
			break;
		}
		break;
	case MANUAL:
		switch(event)
		{
		case START:
			break;
		case TEAM_COLOR_CHANGE:
			break;
		case SUPERVISOR_CMD_AUTO:
			current_state = IDLE;
			break;
		case SUPERVISOR_CMD_MANU:
			break;
		case STOP:
			break;
		}
		break;
	}
}
