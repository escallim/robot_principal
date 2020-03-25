#include "Odometry.h"

#include <stdarg.h>
#include <stdio.h>

odometry_t::odometry_t(uint16_t x, uint16_t y, uint16_t angle): pos(x,y,angle)
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
