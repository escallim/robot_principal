#include "Odometry.h"

#include <stdarg.h>
#include <stdio.h>

#define MCP_DEFAULT 1024
#define PI 3.14159265368979323846264338327950288419716939937510

int seuil[] = {275,275,275,275,275,275,275,275};

odometry_t::odometry_t(MCP233 controller, double wheel_1_diameter, double wheel_2_diameter, double entraxe, uint16_t tick_par_tour, uint16_t x, uint16_t y, uint16_t angle):
		mcp{controller},
		pos(x,y,angle),

		_distance_par_tick_1(wheel_1_diameter * PI / tick_par_tour),
		_distance_par_tick_2(wheel_2_diameter * PI / tick_par_tour),
		_entraxe{entraxe},

		ACCEL_LINEAIRE_1(MCP_DEFAULT),
		VITESSE_LINEAIRE_1(MCP_DEFAULT),
		DECCEL_LINEAIRE_1(MCP_DEFAULT),
		ACCEL_ANGULAIRE_1(MCP_DEFAULT),
		VITESSE_ANGULAIRE_1(MCP_DEFAULT),
		DECCEL_ANGULAIRE_1(MCP_DEFAULT),

		ACCEL_LINEAIRE_2(MCP_DEFAULT*(wheel_1_diameter/wheel_2_diameter)),
		VITESSE_LINEAIRE_2(MCP_DEFAULT*(wheel_1_diameter/wheel_2_diameter)),
		DECCEL_LINEAIRE_2(MCP_DEFAULT*(wheel_1_diameter/wheel_2_diameter)),
		ACCEL_ANGULAIRE_2(MCP_DEFAULT*(wheel_1_diameter/wheel_2_diameter)),
		VITESSE_ANGULAIRE_2(MCP_DEFAULT*(wheel_1_diameter/wheel_2_diameter)),
		DECCEL_ANGULAIRE_2(MCP_DEFAULT*(wheel_1_diameter/wheel_2_diameter))
{
	//mcp.drive_forward(0);
	//mcp.reset_encoder_counts();

	_busy=0;

	_objectif_1 = 0;
	_objectif_2 = 0;

	state = odometry_state_t::IDLE;
}

pos_t odometry_t::get_pos()
{
	return pos;
}

void odometry_t::send_pos()
{
	printf("1 %f %f\n", pos.get_x(), pos.get_y());
}


void odometry_t::gotoDist(double distance, int masque[])
{
	//odometry_state_t etat = odometry_state_t::IDLE;

	// attend que le robot soit dispo
    while( state != odometry_state_t::IDLE);

    // robot occupé
    state = odometry_state_t::BUSY;

    // calcul objectif
    //TODO MUTEX
    while(_busy);
    _busy=1;
    int32_t pos_initiale_1 = mcp.read_encoder_count_M1();
    int32_t pos_initiale_2 = mcp.read_encoder_count_M2();
    _busy=0;

    _objectif_1 = (int32_t) distance/_distance_par_tick_1 + pos_initiale_1;
    _objectif_2 = (int32_t) distance/_distance_par_tick_2 + pos_initiale_2;

    // envoie commande
    while(_busy);
    _busy=1;
    if (distance >= 0) {
        mcp.drive_speed_accel_deccel_position(ACCEL_LINEAIRE_1, ACCEL_LINEAIRE_2, VITESSE_LINEAIRE_1, VITESSE_LINEAIRE_2, DECCEL_LINEAIRE_1, DECCEL_LINEAIRE_2, _objectif_1, _objectif_2, 1);
    } else {
        mcp.drive_speed_accel_deccel_position(ACCEL_LINEAIRE_1, ACCEL_LINEAIRE_2, -VITESSE_LINEAIRE_1, -VITESSE_LINEAIRE_2, DECCEL_LINEAIRE_1, DECCEL_LINEAIRE_2, _objectif_1, _objectif_2, 1);
    }
    _busy=0;


/*

    while(state != odometry_state_t::REACHED){
    	odometry_state_t etat_actuel = odometry_state_t::IDLE;

        for(int i=0 ; i<NB_CAPTEUR_TOF ; i++){
            if(ToF_Mesure[i]<seuil[i] && masque[i]){
                //printf("ACHTUNG GRENADA\n\r");
                mcp.drive_forward(0);
                etat = odometry_state_t::BLOCKED;
            }
        }

        if(etat_actuel != odometry_state_t::BLOCKED && state == odometry_state_t::BLOCKED){
            state = odometry_state_t::UNBLOCKED;
        } else if(etat==odometry_state_t::BLOCKED){
            state = odometry_state_t::BLOCKED;
        }

        if(state == odometry_state_t::BLOCKED){
            mcp.drive_forward(0);
        } else if(state == odometry_state_t::UNBLOCKED){
            state = odometry_state_t::BUSY;
            while(_busy);
            _busy=1;
            if (distance >= 0) {
                mcp.drive_speed_accel_deccel_position(ACCEL_LINEAIRE_1, ACCEL_LINEAIRE_2, VITESSE_LINEAIRE_1, VITESSE_LINEAIRE_2, DECCEL_LINEAIRE_1, DECCEL_LINEAIRE_2, _objectif_1, _objectif_2, 1);
            } else {
                mcp.drive_speed_accel_deccel_position(ACCEL_LINEAIRE_1, ACCEL_LINEAIRE_2, -VITESSE_LINEAIRE_1, -VITESSE_LINEAIRE_2, DECCEL_LINEAIRE_1, DECCEL_LINEAIRE_2, _objectif_1, _objectif_2, 1);
            }
            _busy=0;
        }
    }
*/
    state = odometry_state_t::IDLE;
}

void odometry_t::gotoTheta(double theta, int masque[])
{
	odometry_state_t etat = odometry_state_t::IDLE;

    while(state != odometry_state_t::IDLE);
    state = odometry_state_t::BUSY;

    while(_busy);
    _busy=1;
    int32_t pos_initiale_1 = mcp.read_encoder_count_M1();
    int32_t pos_initiale_2 = mcp.read_encoder_count_M2();
    _busy=0;

    double erreur_theta = theta - pos.get_t();

    while(erreur_theta >= PI) erreur_theta -= 2*PI;
    while(erreur_theta < -PI) erreur_theta += 2*PI;

    _objectif_1 = (int32_t) pos_initiale_1 + (erreur_theta*_entraxe/2)/_distance_par_tick_1;
    _objectif_2 = (int32_t) pos_initiale_2 - (erreur_theta*_entraxe/2)/_distance_par_tick_2;

    while(_busy);
    _busy=1;
    mcp.drive_speed_accel_deccel_position(ACCEL_ANGULAIRE_1, ACCEL_ANGULAIRE_2, VITESSE_ANGULAIRE_1,VITESSE_ANGULAIRE_2, DECCEL_ANGULAIRE_1, DECCEL_ANGULAIRE_2, _objectif_1, _objectif_2, 1);
    _busy=0;

    while(state != odometry_state_t::REACHED){
    	odometry_state_t etat_actuel = odometry_state_t::IDLE;
    	/*
        for(int i=0 ; i<NB_CAPTEUR_TOF ; i++){
            if(ToF_Mesure[i]<seuil[i] && !masque[i]){
                mcp.drive_forward(0);
                etat = odometry_state_t::BLOCKED;
            }
        }
		*/

        if(etat_actuel != odometry_state_t::BLOCKED && state == odometry_state_t::BLOCKED){
            state = odometry_state_t::UNBLOCKED;
        } else if(etat == odometry_state_t::BLOCKED){
            state = odometry_state_t::BLOCKED;
        }

        if(state == odometry_state_t::BLOCKED){
            mcp.drive_forward(0);
        } else if(state == odometry_state_t::UNBLOCKED){
            state = odometry_state_t::BUSY;
            while(_busy);
            _busy=1;
            mcp.drive_speed_accel_deccel_position(ACCEL_ANGULAIRE_1, ACCEL_ANGULAIRE_2, VITESSE_ANGULAIRE_1,VITESSE_ANGULAIRE_2, DECCEL_ANGULAIRE_1, DECCEL_ANGULAIRE_2, _objectif_1, _objectif_2, 1);
            _busy=0;
        }
    }
    state = odometry_state_t::IDLE;
}

/*
void odometry_t::update_odo(void)
{
    while(busy) {
        printf("Jsuis pas dispo gros\n");
    }
    busy=1;
    int32_t mcpENCM1 = mcp.read_encoder_count_M1();
    int32_t mcpENCM2 = mcp.read_encoder_count_M2();
    busy=0;

    int32_t delta_1 = mcpENCM2 - _pulses_1;
    int32_t delta_2 = mcpENCM1 - _pulses_2;

    _pulses_1 = mcpENCM2;
    _pulses_2 = mcpENCM1;

    double deltaDistance = (_distance_par_tick_1*delta_1 + _distance_par_tick_2*delta_2) / 2.0f;
    double deltaTheta = (_distance_par_tick_1*delta_1 - _distance_par_tick_2*delta_2) / _entraxe;

    _posX += deltaDistance*cos(_posTheta+deltaTheta/2);
    _posY += deltaDistance*sin(_posTheta+deltaTheta/2);

    _posTheta += deltaTheta;
    while(_posTheta > PI) _posTheta -= 2*PI;
    while(_posTheta <= -PI) _posTheta += 2*PI;
}


void odometry_t::GotoXY(double x_goal, double y_goal)
{
    double theta_ = atan2(y_goal-_posY, x_goal-_posX);
    double dist_ = sqrt(carre(x_goal-_posX)+carre(y_goal-_posY));
    GotoTheta(theta_);
    GotoDist(dist_);
}
*/
