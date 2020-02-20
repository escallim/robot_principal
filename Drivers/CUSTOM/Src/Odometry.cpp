#include "Odometry.h"

#include <stdarg.h>
#include <stdio.h>

robot_t::robot_t(uint16_t x, uint16_t y, uint16_t angle): pos(x,y,angle)
{
	mode = MANUAL;
	etat = WAITING;
}

pos_t robot_t::get_pos()
{
	return pos;
}

void robot_t::send_pos()
{
	//printf("1 %d %d %d\n", pos.x, pos.y, pos.angle);
}
