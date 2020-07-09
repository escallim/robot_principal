/**
 *  TCA9548A library
 *
 *  @author Akash Vibhute
 *  @author < akash . roboticist [at] gmail . com >
 *  @version 0.1
 *  @date May/24/2016
 */

#include <Drivers/TCA9548A/TCA9548A.h>

TCA9548A::TCA9548A( I2C_HandleTypeDef &hi2c, uint8_t i2c_address) : hi2c(hi2c), i2c_addr(i2c_address)
{
}


void TCA9548A::select( uint8_t channel )
{
    uint8_t data; //create temporary char buffer

    if(channel >= 8) return;

    data = 1 << channel;

    //i2c_.write( i2c_addr << 1, &data, 1);
    HAL_I2C_Master_Transmit(&hi2c, i2c_addr << 1, &data, 1, 10);
}

void TCA9548A::reset( )
{
    //reset_pin = 0;
    HAL_Delay(1);
    //reset_pin = 1;
}
