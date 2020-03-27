#ifndef ODOMETRY_H
#define ODOMETRY_H

#include "main.h"

#include "MCP233.h"

struct pos_t
{
	pos_t(uint16_t x=0, uint16_t y=0, uint16_t angle=0) : x{x}, y{y}, angle{angle} {};

	uint16_t x;
	uint16_t y;
	uint16_t angle;
};

struct meca_t
{
	meca_t(double wheel_1_diameter, double wheel_2_diameter, double entraxe) : wheel_1_diameter{wheel_1_diameter}, wheel_2_diameter{wheel_2_diameter}, entraxe{entraxe} {};

	double wheel_1_diameter;
	double wheel_2_diameter;
	double entraxe;
};

enum class odometry_state_t
{
	IDLE,
	BUSY,
	REACHED,
	BLOCKED,
	UNBLOCKED
};


enum odometry_event_t
{

};

class odometry_t
{
public:
	odometry_t(MCP233 controller, double wheel_1_diameter, double wheel_2_diameter, double entraxe, uint16_t x=0, uint16_t y=0, uint16_t angle=0);

	pos_t get_pos();
	void send_pos();

private:
	MCP233 &mcp;

	meca_t config_meca;

	pos_t pos;
};


#endif
