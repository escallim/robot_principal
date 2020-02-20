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

enum r_etat_t
{
	WAITING,
	BUSY,
	BLOCKED
};

enum r_mode_t
{
	MANUAL,
	AUTOMATIC
};

class robot_t
{
public:
	robot_t(uint16_t x=0, uint16_t y=0, uint16_t angle=0);

	pos_t get_pos();
	void send_pos();
private:
	pos_t pos;
	r_etat_t etat;
	r_mode_t mode;
};


#endif
