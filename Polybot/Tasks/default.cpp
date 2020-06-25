#include "main.h"
#include "cmsis_os.h"

#include <stdio.h>

#include "MCP233.h"
#include "VL53L0X.h"
#include "TCA9548A.h"

#include "Odometry.h"

#include "Robot.h"


#define NB_CAPTEUR_TOF 2
#define MATCH_DURATION 100

#define WHEEL_1_DIAMETER 87.5
#define WHEEL_2_DIAMETER 89.0
#define ENTRAXE 276.0

// entre les roue 126.5 encodeur 259.3



#define TICK_PAR_TOUR 4096


#define LOG(f_, ...) printf((f_), ##__VA_ARGS__)


extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart6;

MCP233 controller(128, huart6);

odometry_t odometry(controller, WHEEL_1_DIAMETER, WHEEL_2_DIAMETER, TICK_PAR_TOUR, ENTRAXE);

robot_t robot(controller,odometry);

void robot_inc_timer()
{
	if(robot.get_state() == robot_state_t::AUTOMATIC)
	{
		robot.inc_timer();
		if(robot.get_timer() == MATCH_DURATION)
		{
			robot.event(robot_event_t::STOP);
		}
	}
}
#define RX_SIZE 2
extern uint8_t rx_buffer[];

void StartDefaultTask(void const * argument)
{


	HAL_UART_Receive_IT(&huart2, rx_buffer, RX_SIZE);

	/*
		TCA9548A mux(hi2c1);

		VL53L0X *tof_sensor[NB_CAPTEUR_TOF];
		uint16_t tof_dist[NB_CAPTEUR_TOF];

		// Initialisation
		for (int i = 0; i < NB_CAPTEUR_TOF; i++) {
			mux.select(i);
			tof_sensor[i] = new VL53L0X(hi2c1);
			tof_sensor[i]->init();
			tof_sensor[i]->startContinuous();
		}
	*/
		printf("debut\n");

		printf("j'attend\n");
		while(robot.get_state() == robot_state_t::IDLE)
		{
			HAL_Delay(100);
		}

		HAL_Delay(100);

		printf("c'est partit\n");

		controller.reset_encoder_counts();
		printf("reset enc\n");

		//controller.drive_forward(0);
		//printf("reset moteur\n");

		HAL_Delay(400);

		controller.drive_speed_accel_deccel_position(1024,1024, 1024,1024, 1024, 1024, 4096, 4096, 0);
		printf("commande\n");

		/* USER CODE END 2 */

		/* Infinite loop */
		/* USER CODE BEGIN WHILE */
		while (1) {
			/* USER CODE END WHILE */

			/* USER CODE BEGIN 3 */
			/*
			// Mesure
			for (int i = 0; i < NB_CAPTEUR_TOF; i++) {
				mux.select(i);
				tof_dist[i] = tof_sensor[i]->readRangeContinuousMillimeters();
			}

			// Affichage
			printf("\n");
			for (int i = 0; i < NB_CAPTEUR_TOF; i++) {
				printf("S%i: %4d\t", i, tof_dist[i]);
			}

			HAL_Delay(100);*/

			int32_t enc1 = controller.read_encoder_count_M1();
			int32_t enc2 = controller.read_encoder_count_M2();
			printf("\t%6ld | %6ld  \n", enc1, enc2);
			HAL_Delay(100);
		}
		/* USER CODE END 3 */
  for(;;)
  {
    osDelay(1);
  }
}
