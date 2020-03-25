#ifndef ROBOT_H
#define ROBOT_H

#include "main.h"


enum robot_state_t
{
	IDLE,
	AUTOMATIC,
	MANUAL
};

enum robot_event_t
{
	START,
	TEAM_COLOR_CHANGE,
	SUPERVISOR_CMD_AUTO,
	SUPERVISOR_CMD_MANU,
	STOP
};


struct robot_t
{
public:
	robot_t();

	robot_state_t get_state();


	void transition(robot_event_t event);

private:
	robot_state_t current_state;
};


#endif
