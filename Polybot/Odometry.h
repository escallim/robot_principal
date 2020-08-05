#ifndef ODOMETRY_H
#define ODOMETRY_H

#include "MCP233.h"
#include "main.h"


class pos_t
{
public:
	pos_t(uint16_t x=0, uint16_t y=0, uint16_t theta=0) : x{x}, y{y}, theta{theta} {};

	void set_xytheta(double x, double y, double theta) { x=x; y = y; theta = theta; }
	void set_x(double x) { x = x; }
	void set_y(double y) { y = y; }
	void set_t(double theta) { theta = theta; }

	double get_x() { return x; }
	double get_y() { return y; }
	double get_t() { return theta; }
private:
	uint16_t x;
	uint16_t y;
	uint16_t theta;
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


enum class odometry_event_t
{

};

enum signe_deplacement {
    NEGATIF,
    POSITIF
};

class odometry_t
{
public:
	odometry_t(MCP233 controller, double wheel_1_diameter, double wheel_2_diameter, double entraxe, uint16_t tick_par_tour, uint16_t x=0, uint16_t y=0, uint16_t angle=0);

	pos_t get_pos();
	void send_pos();


	void gotoXY(double x_goal, double y_goal);
	void gotoTheta(double theta_goal, int masque[]);
	void gotoDist(double distance, int masque[]);

	void update_odo(void);

	bool _busy;

	int32_t _objectif_1;
	int32_t _objectif_2;

private:
	MCP233 &mcp;

	int32_t _pulses_1;
	int32_t _pulses_2;

	pos_t pos;

	signe_deplacement signe;

	//meca_t config_meca;



	odometry_state_t state;

	const double _distance_par_tick_1;
	const double _distance_par_tick_2;
	const double _entraxe;

	const int ACCEL_LINEAIRE_1;
	const int VITESSE_LINEAIRE_1;
	const int DECCEL_LINEAIRE_1;
	const int ACCEL_ANGULAIRE_1;
	const int VITESSE_ANGULAIRE_1;
	const int DECCEL_ANGULAIRE_1;

	const int ACCEL_LINEAIRE_2;
	const int VITESSE_LINEAIRE_2;
	const int DECCEL_LINEAIRE_2;
	const int ACCEL_ANGULAIRE_2;
	const int VITESSE_ANGULAIRE_2;
	const int DECCEL_ANGULAIRE_2;
};


#endif
