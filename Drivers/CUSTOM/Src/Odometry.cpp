#include "Odometry.h"

#include <stdarg.h>
#include <stdio.h>

odometry_t::odometry_t(MCP233 controller, double wheel_1_diameter, double wheel_2_diameter, double entraxe, uint16_t x, uint16_t y, uint16_t angle): mcp{controller}, config_meca{wheel_1_diameter, wheel_2_diameter, entraxe}, pos(x,y,angle)
{
}

pos_t odometry_t::get_pos()
{
	return pos;
}

void odometry_t::send_pos()
{
	printf("1 %d %d\n", pos.x, pos.y);
}
