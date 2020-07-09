#include <Drivers/MCP233/MCP233.h>
#include <stdarg.h>
#include <stdio.h>

#define SetDWORDval(arg) (uint8_t)(arg>>24),(uint8_t)(arg>>16),(uint8_t)(arg>>8),(uint8_t)arg
#define SetWORDval(arg) (uint8_t)(arg>>8),(uint8_t)arg


MCP233::MCP233(uint8_t id, UART_HandleTypeDef &huart) : huart(huart), _ID(id)
{
}

void MCP233::crc_clear(){
    _crc = 0;
}

void MCP233::crc_update (uint8_t data){
    int i;
    _crc = _crc ^ ((uint16_t)data << 8);
    for (i=0; i<8; i++) {
        if (_crc & 0x8000)
            _crc = (_crc << 1) ^ 0x1021;
        else
            _crc <<= 1;
    }
}

uint16_t MCP233::crc_get(){
    return _crc;
}

void MCP233::send_packet(uint8_t cnt, ... ){
    crc_clear();
    va_list param;
    va_start( param, cnt );
    for(uint8_t index=0; index<cnt; index++) {
        uint8_t data = va_arg(param, unsigned int);
        crc_update(data);
        HAL_UART_Transmit(&huart, &data, 1, 1000);
    }
    va_end(param);

    uint16_t _crc = crc_get();

    uint8_t crc[2];
    crc[0] = _crc >>8;
    crc[1] = _crc;

    HAL_UART_Transmit(&huart, crc, 2, 1000);
}

void MCP233::write_(uint8_t command, uint8_t data, bool reading, bool crcon){
	HAL_UART_Transmit(&huart, &_ID, 1, 1000);
	HAL_UART_Transmit(&huart, &command, 1, 1000);

    if(reading == false) {
        if(crcon == true) {
            uint8_t packet[2] = {_ID, command};
            uint16_t checksum_tmp = crc16(packet,2);
            uint8_t checksum[2] = { (checksum_tmp && 0xFF00)>>8 , (checksum_tmp && 0x00FF) };
            HAL_UART_Transmit(&huart, checksum, 2, 1000);
        } else {
            HAL_UART_Transmit(&huart, &data, 1, 1000);

            uint8_t packet[3] = {_ID, command, data};
            uint16_t checksum_tmp = crc16(packet,3);
            uint8_t checksum[2] = { (checksum_tmp && 0xFF00)>>8 , (checksum_tmp && 0x00FF) };
            HAL_UART_Transmit(&huart, checksum, 2, 1000);
        }
    }
}

uint16_t MCP233::crc16(uint8_t *packet, int nBytes){
    uint16_t crc_;
    for (int byte = 0; byte < nBytes; byte++) {
        crc_ = crc_ ^ ((uint16_t)packet[byte] << 8);
        for (uint8_t bit = 0; bit < 8; bit++) {
            if (crc_ & 0x8000) {
                crc_ = (crc_ << 1) ^ 0x1021;
            } else {
                crc_ = crc_ << 1;
            }
        }
    }
    return crc_;
}

uint8_t MCP233::read_(void){
	uint8_t data;
	HAL_UART_Receive(&huart, &data, 1, 10);
    return data;
}



void MCP233::drive_forward_M1(int speed){
    do {
		send_packet(3,_ID,0,speed);
    } while( read_() != 0xFF);
}

void MCP233::drive_backward_M1(int speed){
    do {
        send_packet(3,_ID,1,speed);
    } while( read_() != 0xFF);
}

void MCP233::drive_forward_M2(int speed){
    do {
        send_packet(3,_ID,4,speed);
    } while( read_() != 0xFF);
}

void MCP233::drive_backward_M2(int speed){
    do {
    	send_packet(3,_ID,5,speed);
    } while( read_() != 0xFF);
}

void MCP233::drive_forward(int speed){
    do {
    	send_packet(3,_ID,8,speed);
    } while( read_() != 0xFF);
}

void MCP233::drive_backward(int speed){
    do {
    	send_packet(3,_ID,9,speed);
    } while( read_() != 0xFF);
}

void MCP233::read_firmware_version(){
    write_(21,0x00,true,false);
}

int32_t MCP233::read_encoder_count_M1(){
    uint16_t read_byte[7];
    int32_t count;

    //send_packet(2,_ID,16);
    write_(16,0x00, true,false);

    read_byte[0] = (uint16_t)read_();
    read_byte[1] = (uint16_t)read_();
    read_byte[2] = (uint16_t)read_();
    read_byte[3] = (uint16_t)read_();
    read_byte[4] = (uint16_t)read_();
    read_byte[5] = (uint16_t)read_();
    read_byte[6] = (uint16_t)read_();

    count = read_byte[0]<<24;
    count |= read_byte[1]<<16;
    count |= read_byte[2]<<8;
    count |= read_byte[3];

    return count;
}

int32_t MCP233::read_encoder_count_M2(){
    uint16_t read_byte[7];
    int32_t count;

    //send_packet(2,_ID,17);
    write_(17,0x00, true,false);

    read_byte[0] = (uint16_t)read_();
    read_byte[1] = (uint16_t)read_();
    read_byte[2] = (uint16_t)read_();
    read_byte[3] = (uint16_t)read_();
    read_byte[4] = (uint16_t)read_();
    read_byte[5] = (uint16_t)read_();
    read_byte[6] = (uint16_t)read_();

    count = read_byte[0]<<24;
    count |= read_byte[1]<<16;
    count |= read_byte[2]<<8;
    count |= read_byte[3];

    return count;
}

int32_t MCP233::read_encoder_speed_M1(){
    uint16_t read_byte[7];
    int32_t speed;

    send_packet(2,_ID,18);

    read_byte[0] = (uint16_t)read_();
    read_byte[1] = (uint16_t)read_();
    read_byte[2] = (uint16_t)read_();
    read_byte[3] = (uint16_t)read_();
    read_byte[4] = (uint16_t)read_();
    read_byte[5] = (uint16_t)read_();
    read_byte[6] = (uint16_t)read_();

    speed = read_byte[0]<<24;
    speed |= read_byte[1]<<16;
    speed |= read_byte[2]<<8;
    speed |= read_byte[3];

    return speed;
}

int32_t MCP233::read_encoder_speed_M2(){
    uint16_t read_byte[7];
    int32_t speed;

    send_packet(2,_ID,19);

    read_byte[0] = (uint16_t)read_();
    read_byte[1] = (uint16_t)read_();
    read_byte[2] = (uint16_t)read_();
    read_byte[3] = (uint16_t)read_();
    read_byte[4] = (uint16_t)read_();
    read_byte[5] = (uint16_t)read_();
    read_byte[6] = (uint16_t)read_();

    speed = read_byte[0]<<24;
    speed |= read_byte[1]<<16;
    speed |= read_byte[2]<<8;
    speed |= read_byte[3];

    return speed;
}

void MCP233::reset_encoder_counts(){
    do {
        send_packet(2,_ID,20);
    } while( read_() != 0xFF);
}

void MCP233::drive_speed_distance_M1(int32_t speed, int32_t distance, bool buffer){
    do {
        send_packet(11,_ID,41,SetDWORDval(speed),SetDWORDval(distance),buffer);
    } while( read_() != 0xFF);
}

void MCP233::drive_speed_distance_M2(int32_t speed, int32_t distance, bool buffer){
    do {
        send_packet(11,_ID,42,SetDWORDval(speed),SetDWORDval(distance),buffer);
    } while( read_() != 0xFF);
}

void MCP233::drive_speed_distance(int32_t speed_M1,int32_t speed_M2, int32_t distance_M1, int32_t distance_M2, bool buffer){
    do {
        send_packet(11,_ID,43,SetDWORDval(speed_M1),SetDWORDval(distance_M1),SetDWORDval(speed_M2),SetDWORDval(distance_M2),buffer);
    } while( read_() != 0xFF);
}

void MCP233::drive_speed_accel_distance_M1(int32_t accel, int32_t speed, int32_t distance, bool buffer){
    do {
        send_packet(15,_ID,44,SetDWORDval(accel),SetDWORDval(speed),SetDWORDval(distance),buffer);
    } while( read_() != 0xFF);
}

void MCP233::drive_speed_accel_distance_M2(int32_t accel, int32_t speed, int32_t distance, bool buffer){
    do {
        send_packet(15,_ID,45,SetDWORDval(accel),SetDWORDval(speed),SetDWORDval(distance),buffer);
    } while( read_() != 0xFF);
}

void MCP233::drive_speed_accel_distance(int32_t accel, int32_t speed_M1, int32_t distance_M1, int32_t speed_M2, int32_t distance_M2, bool buffer){
    do {
        send_packet(23,_ID,46,SetDWORDval(accel),SetDWORDval(speed_M1),SetDWORDval(distance_M1),SetDWORDval(speed_M2),SetDWORDval(distance_M2),buffer);
    } while( read_() != 0xFF);
}


void MCP233::drive_speed_accel_deccel_position_M1(int32_t accel, int32_t speed, int32_t deccel, int32_t position, bool buffer){
    do {
        send_packet(19,_ID,65,SetDWORDval(accel),SetDWORDval(speed),SetDWORDval(deccel),SetDWORDval(position),buffer);
    } while( read_() != 0xFF);
}

void MCP233::drive_speed_accel_deccel_position_M2(int32_t accel, int32_t speed, int32_t deccel, int32_t position, bool buffer){
    do {
        send_packet(19,_ID,66,SetDWORDval(accel),SetDWORDval(speed),SetDWORDval(deccel),SetDWORDval(position),buffer);
    } while( read_() != 0xFF);
}

void MCP233::drive_speed_accel_deccel_position(int32_t accel_M1, int32_t accel_M2, int32_t speed_M1,int32_t speed_M2, int32_t deccel_M1, int32_t deccel_M2, int32_t position_M1, int32_t position_M2, bool buffer){
    do {
        send_packet(35,_ID,67,SetDWORDval(accel_M1),SetDWORDval(speed_M1),SetDWORDval(deccel_M1),SetDWORDval(position_M1),SetDWORDval(accel_M2),SetDWORDval(speed_M2),SetDWORDval(deccel_M2),SetDWORDval(position_M2),buffer);
    } while( read_() != 0xFF);
}

