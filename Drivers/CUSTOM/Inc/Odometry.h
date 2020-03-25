#ifndef ODOMETRY_H
#define ODOMETRY_H

#include "main.h"

struct pos_t
{
	pos_t(uint16_t x=0, uint16_t y=0, uint16_t angle=0) : x(x), y(y), angle(angle) {};

	uint16_t x;
	uint16_t y;
	uint16_t angle;
};


class odometry_t
{
public:
	odometry_t(uint16_t x=0, uint16_t y=0, uint16_t angle=0);

	pos_t get_pos();
	void send_pos();
private:
	pos_t pos;
};


#endif
