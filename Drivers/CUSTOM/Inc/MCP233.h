#ifndef MCP233_H
#define MCP233_H

#include "main.h"

class MCP233
{
    public:
        MCP233(uint8_t id, UART_HandleTypeDef &huart);

        void drive_forward_M1(int speed);
        void drive_backward_M1(int speed);
        void drive_forward_M2(int speed);
        void drive_backward_M2(int speed);
        void drive_forward(int speed);
        void drive_backward(int speed);

        void read_firmware_version();

        int32_t read_encoder_count_M1();
        int32_t read_encoder_count_M2();
        int32_t read_encoder_speed_M1();
        int32_t read_encoder_speed_M2();
        void reset_encoder_counts();
        void set_encoder_count_M1(int32_t count);
        void set_encoder_count_M2(int32_t count);
        int32_t read_current_raw_speed_M1();
        int32_t read_current_raw_speed_M2();
        void read_encodeurs_counts(int32_t counts[2]);
        void read_motor_speeds(int32_t speeds[2]);


        void drive_speed_distance_M1(int32_t speed, int32_t distance, bool buffer);
        void drive_speed_distance_M2(int32_t speed, int32_t distance, bool buffer);
        void drive_speed_distance(int32_t speed_M1,int32_t speed_M2, int32_t position_M1, int32_t distance_M2, bool buffer);

        void drive_speed_accel_distance_M1(int32_t accel, int32_t speed, int32_t distance, bool buffer);
        void drive_speed_accel_distance_M2(int32_t accel, int32_t speed, int32_t distance, bool buffer);
        void drive_speed_accel_distance(int32_t accel, int32_t speed_M1, int32_t distance_M1, int32_t speed_M2, int32_t distance_M2, bool buffer);

        void drive_speed_accel_deccel_position_M1(int32_t accel, int32_t speed, int32_t deccel, int32_t position, bool buffer);
        void drive_speed_accel_deccel_position_M2(int32_t accel, int32_t speed, int32_t deccel, int32_t position, bool buffer);
        void drive_speed_accel_deccel_position(int32_t accel_M1, int32_t accel_M2, int32_t speed_M1,int32_t speed_M2, int32_t deccel_M1, int32_t deccel_M2, int32_t position_M1, int32_t position_M2, bool buffer);

    private:
        // Attributs
        UART_HandleTypeDef &huart;
        uint16_t _crc;
        uint8_t _ID;

        // Méthodes privée pour crc
        void crc_clear();
        void crc_update(uint8_t data);
        uint16_t crc_get();
        uint16_t crc16(uint8_t *packet, int nBytes);

        // Méthodes privée pour liaison série
        void send_packet(uint8_t cnt, ...);
        void write_(uint8_t command, uint8_t data, bool reading, bool crcon);
        uint8_t read_(void);
};

#endif
