#ifndef ROBOT_H
#define ROBOT_H

#include "main.h"

#include "MCP233.h"
#include "Odometry.h"


enum class color_t
{
	ERROR = -1,
	BLUE = 0,
	YELLOW = 1
};

enum class robot_state_t
{
	IDLE,
	AUTOMATIC,
	MANUAL,
	DEAD
};


enum class robot_event_t
{
	START,
	STOP,

	TEAM_COLOR_CHANGE,

	SUPERVISOR_CMD_AUTO,
	SUPERVISOR_CMD_MANU
};

enum class robot_cmd_t
{
	HALT,
	GO_FORWARD,
	GO_BACKWARD,
	TURN_LEFT,
	TURN_RIGHT
};


class robot_t
{
public:
	robot_t(MCP233 controller, odometry_t odometry);


	robot_state_t get_state();

	color_t get_color();

	void event(robot_event_t event);
	void cmd(robot_cmd_t cmd);

	void reset_timer(){ timer = 0; }
	uint8_t get_timer(){ return timer; }
	void inc_timer(){ timer++; }

private:
	MCP233 &mcp;
	odometry_t &odo;

	robot_state_t current_state;

	uint8_t timer;

	const char robot_state_str[4][20] = {"IDLE","AUTOMATIC","MANUAL","DEAD"};
	const char robot_event_str[5][20] = {"START","STOP","TEAM_COLOR_CHANGE","SUPERVISOR_CMD_AUTO","SUPERVISOR_CMD_MANU"};
	const char robot_cmd_str[5][20] = {"HALT","GO_FORWARD","GO_BACKWARD","TURN_LEFT","TURN_RIGHT"};


};


#endif
