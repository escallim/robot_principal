#include "main.h"
#include "cmsis_os.h"

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == demarrage_Pin)
	{
		//robot.event(robot_event_t::START);
	}

	if(GPIO_Pin == couleur_Pin)
	{
		//robot.event(robot_event_t::TEAM_COLOR_CHANGE);
	}
}
