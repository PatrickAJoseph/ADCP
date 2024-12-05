
#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include "LED.h"
#include "MPU9250.h"
#include "motion_processing.h"

#define FIRMWARE_DESCRIPTOR    "MPU_STICK_V1.0"

typedef struct _configuration
{
    char firmware_descriptor[20];

    LED_config  LED1_config;
    LED_config  LED2_config;
    LED_config  LED3_config;

    struct IMU_config
    {
        uint16_t sampling_rate;
        uint16_t calibration_interval_ms;
        uint16_t calibration_point_count;

        union IMU_config_segments
        {
            uint16_t all;

            struct IMU_config_segment
            {
                uint16_t  accelerometer_full_scale        :2;
                uint16_t  gyroscope_full_scale            :2;
                uint16_t  filter_enable                   :1;
                uint16_t  accelerometer_filter_bandwidth  :3;
                uint16_t  gyroscope_filter_bandwidth      :3;
            }IMU_config_segment;

        }IMU_config_segments;
    }IMU_config;


    union motion_detection_config
    {
        uint32_t all;


        struct _motion_detection_config_segments
        {
            uint32_t accel_x_threshold_type      :       3;
            uint32_t accel_y_threshold_type      :       3;
            uint32_t accel_z_threshold_type      :       3;
            uint32_t gyro_x_threshold_type       :       3;
            uint32_t gyro_y_threshold_type       :       3;
            uint32_t gyro_z_threshold_type       :       3;
            uint32_t roll_threshold_type         :       3;
            uint32_t pitch_threshold_type        :       3;
            uint32_t yaw_threshold_type          :       3;

        }motion_detection_config_segments;

    }motion_detection_config;

    _iq12 accelerometer_motion_detection_low_threshold[3];
    _iq12 accelerometer_motion_detection_high_threshold[3];
    _iq12 gyroscope_motion_detection_low_threshold[3];
    _iq12 gyroscope_motion_detection_high_threshold[3];
    _iq12 roll_motion_detection_low_threshold;
    _iq12 roll_motion_detection_high_threshold;
    _iq12 pitch_motion_detection_low_threshold;
    _iq12 pitch_motion_detection_high_threshold;
    _iq12 yaw_motion_detection_low_threshold;
    _iq12 yaw_motion_detection_high_threshold;

    _iq12 roll_filter_constant;
    _iq12 pitch_filter_constant;

}configuration_t;


configuration_t config;


void config_read();
void config_write();

#endif /* INC_CONFIGURATION_H_ */
