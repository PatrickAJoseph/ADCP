
#include "configuration.h"
#include "EEPROM.h"

void config_read()
{
    EEPROM_read_bytes(0,(void*)&config,sizeof(config));

    LED1_config = config.LED1_config;
    LED2_config = config.LED2_config;
    LED3_config = config.LED3_config;

    MPU9250.calibration_interval_ms =   config.IMU_config.calibration_interval_ms;
    MPU9250.calibration_point_count =   config.IMU_config.calibration_point_count;
    MPU9250.sampling_rate           =   config.IMU_config.sampling_rate;

    MPU9250.configuration_A.configuration_A_segments.accelerometer_filter_bandwidth
    = config.IMU_config.IMU_config_segments.IMU_config_segment.accelerometer_filter_bandwidth;

    MPU9250.configuration_A.configuration_A_segments.accelerometer_full_scale
    = config.IMU_config.IMU_config_segments.IMU_config_segment.accelerometer_full_scale;

    MPU9250.configuration_A.configuration_A_segments.filter_enable
    = config.IMU_config.IMU_config_segments.IMU_config_segment.filter_enable;

    MPU9250.configuration_A.configuration_A_segments.gyroscope_filter_bandwidth
    = config.IMU_config.IMU_config_segments.IMU_config_segment.gyroscope_filter_bandwidth;

    MPU9250.configuration_A.configuration_A_segments.gyroscope_full_scale
    = config.IMU_config.IMU_config_segments.IMU_config_segment.gyroscope_full_scale;

    MP.motion_detection.motion_detection_segments.acc_x_threshold_type =
    config.motion_detection_config.motion_detection_config_segments.accel_x_threshold_type;

    MP.motion_detection.motion_detection_segments.acc_y_threshold_type =
    config.motion_detection_config.motion_detection_config_segments.accel_y_threshold_type;

    MP.motion_detection.motion_detection_segments.acc_z_threshold_type =
    config.motion_detection_config.motion_detection_config_segments.accel_z_threshold_type;

    MP.motion_detection.motion_detection_segments.gyr_x_threshold_type =
    config.motion_detection_config.motion_detection_config_segments.gyro_x_threshold_type;

    MP.motion_detection.motion_detection_segments.gyr_y_threshold_type =
    config.motion_detection_config.motion_detection_config_segments.gyro_y_threshold_type;

    MP.motion_detection.motion_detection_segments.gyr_z_threshold_type =
    config.motion_detection_config.motion_detection_config_segments.gyro_z_threshold_type;

    MP.motion_detection.motion_detection_segments.roll_threshold_type =
    config.motion_detection_config.motion_detection_config_segments.roll_threshold_type;

    MP.motion_detection.motion_detection_segments.roll_threshold_type =
    config.motion_detection_config.motion_detection_config_segments.roll_threshold_type;

    MP.motion_detection.motion_detection_segments.roll_threshold_type =
    config.motion_detection_config.motion_detection_config_segments.roll_threshold_type;

    MP.motion_detection.motion_detection_segments.pitch_threshold_type =
    config.motion_detection_config.motion_detection_config_segments.pitch_threshold_type;

    MP.motion_detection.motion_detection_segments.pitch_threshold_type =
    config.motion_detection_config.motion_detection_config_segments.pitch_threshold_type;

    MP.motion_detection.motion_detection_segments.pitch_threshold_type =
    config.motion_detection_config.motion_detection_config_segments.pitch_threshold_type;

    MP.motion_detection.motion_detection_segments.yaw_threshold_type =
    config.motion_detection_config.motion_detection_config_segments.yaw_threshold_type;

    MP.motion_detection.motion_detection_segments.yaw_threshold_type =
    config.motion_detection_config.motion_detection_config_segments.yaw_threshold_type;

    MP.motion_detection.motion_detection_segments.yaw_threshold_type =
    config.motion_detection_config.motion_detection_config_segments.yaw_threshold_type;

    MP.accelerometer_low_threshold[X_AXIS] = config.accelerometer_motion_detection_low_threshold[X_AXIS];
    MP.accelerometer_low_threshold[Y_AXIS] = config.accelerometer_motion_detection_low_threshold[Y_AXIS];
    MP.accelerometer_low_threshold[Z_AXIS] = config.accelerometer_motion_detection_low_threshold[Z_AXIS];
    MP.accelerometer_high_threshold[X_AXIS] = config.accelerometer_motion_detection_high_threshold[X_AXIS];
    MP.accelerometer_high_threshold[Y_AXIS] = config.accelerometer_motion_detection_high_threshold[Y_AXIS];
    MP.accelerometer_high_threshold[Z_AXIS] = config.accelerometer_motion_detection_high_threshold[Z_AXIS];

    MP.gyroscope_low_threshold[X_AXIS] = config.gyroscope_motion_detection_low_threshold[X_AXIS];
    MP.gyroscope_low_threshold[Y_AXIS] = config.gyroscope_motion_detection_low_threshold[Y_AXIS];
    MP.gyroscope_low_threshold[Z_AXIS] = config.gyroscope_motion_detection_low_threshold[Z_AXIS];
    MP.gyroscope_high_threshold[X_AXIS] = config.gyroscope_motion_detection_high_threshold[X_AXIS];
    MP.gyroscope_high_threshold[Y_AXIS] = config.gyroscope_motion_detection_high_threshold[Y_AXIS];
    MP.gyroscope_high_threshold[Z_AXIS] = config.gyroscope_motion_detection_high_threshold[Z_AXIS];

    MP.angle_low_threshold[ROLL]    =   config.roll_motion_detection_low_threshold;
    MP.angle_low_threshold[PITCH]   =   config.pitch_motion_detection_low_threshold;
    MP.angle_low_threshold[YAW]     =   config.yaw_motion_detection_low_threshold;
    MP.angle_high_threshold[ROLL]   =   config.roll_motion_detection_high_threshold;
    MP.angle_high_threshold[PITCH]  =   config.pitch_motion_detection_high_threshold;
    MP.angle_high_threshold[YAW]    =   config.yaw_motion_detection_high_threshold;

    MP.roll_filter_constant = config.roll_filter_constant;
    MP.pitch_filter_constant = config.pitch_filter_constant;
}

void config_write()
{
    uint8_t i = 0;

    for( i = 0 ; i < 20 ; i++ )
    {
        config.firmware_descriptor[i] = FIRMWARE_DESCRIPTOR[i];
    }

    config.LED1_config = LED1_config;
    config.LED2_config = LED2_config;
    config.LED3_config = LED3_config;

    config.IMU_config.calibration_interval_ms =   MPU9250.calibration_interval_ms;
    config.IMU_config.calibration_point_count =   MPU9250.calibration_point_count;
    config.IMU_config.sampling_rate           =   MPU9250.sampling_rate;

    config.IMU_config.IMU_config_segments.IMU_config_segment.accelerometer_filter_bandwidth =
    MPU9250.configuration_A.configuration_A_segments.accelerometer_filter_bandwidth;

    config.IMU_config.IMU_config_segments.IMU_config_segment.accelerometer_full_scale =
    MPU9250.configuration_A.configuration_A_segments.accelerometer_full_scale;

    config.IMU_config.IMU_config_segments.IMU_config_segment.filter_enable =
    MPU9250.configuration_A.configuration_A_segments.filter_enable;

    config.IMU_config.IMU_config_segments.IMU_config_segment.gyroscope_filter_bandwidth =
    MPU9250.configuration_A.configuration_A_segments.gyroscope_filter_bandwidth;

    config.IMU_config.IMU_config_segments.IMU_config_segment.gyroscope_full_scale =
    MPU9250.configuration_A.configuration_A_segments.gyroscope_full_scale;

    config.motion_detection_config.motion_detection_config_segments.accel_x_threshold_type =
    MP.motion_detection.motion_detection_segments.acc_x_threshold_type;

    config.motion_detection_config.motion_detection_config_segments.accel_y_threshold_type =
    MP.motion_detection.motion_detection_segments.acc_y_threshold_type;

    config.motion_detection_config.motion_detection_config_segments.accel_z_threshold_type =
    MP.motion_detection.motion_detection_segments.acc_z_threshold_type;

    config.motion_detection_config.motion_detection_config_segments.gyro_x_threshold_type =
    MP.motion_detection.motion_detection_segments.gyr_x_threshold_type;

    config.motion_detection_config.motion_detection_config_segments.gyro_y_threshold_type =
    MP.motion_detection.motion_detection_segments.gyr_y_threshold_type;

    config.motion_detection_config.motion_detection_config_segments.gyro_z_threshold_type =
    MP.motion_detection.motion_detection_segments.gyr_z_threshold_type;

    config.motion_detection_config.motion_detection_config_segments.roll_threshold_type =
    MP.motion_detection.motion_detection_segments.roll_threshold_type;

    config.motion_detection_config.motion_detection_config_segments.pitch_threshold_type =
    MP.motion_detection.motion_detection_segments.pitch_threshold_type;

    config.motion_detection_config.motion_detection_config_segments.yaw_threshold_type =
    MP.motion_detection.motion_detection_segments.yaw_threshold_type;

    config.accelerometer_motion_detection_low_threshold[X_AXIS] = MP.accelerometer_low_threshold[X_AXIS];
    config.accelerometer_motion_detection_low_threshold[Y_AXIS] = MP.accelerometer_low_threshold[Y_AXIS];
    config.accelerometer_motion_detection_low_threshold[Z_AXIS] = MP.accelerometer_low_threshold[Z_AXIS];
    config.accelerometer_motion_detection_high_threshold[X_AXIS] = MP.accelerometer_high_threshold[X_AXIS];
    config.accelerometer_motion_detection_high_threshold[Y_AXIS] = MP.accelerometer_high_threshold[Y_AXIS];
    config.accelerometer_motion_detection_high_threshold[Z_AXIS] = MP.accelerometer_high_threshold[Z_AXIS];

    config.gyroscope_motion_detection_low_threshold[X_AXIS] = MP.gyroscope_low_threshold[X_AXIS];
    config.gyroscope_motion_detection_low_threshold[Y_AXIS] = MP.gyroscope_low_threshold[Y_AXIS];
    config.gyroscope_motion_detection_low_threshold[Z_AXIS] = MP.gyroscope_low_threshold[Z_AXIS];
    config.gyroscope_motion_detection_high_threshold[X_AXIS] = MP.gyroscope_high_threshold[X_AXIS];
    config.gyroscope_motion_detection_high_threshold[Y_AXIS] = MP.gyroscope_high_threshold[Y_AXIS];
    config.gyroscope_motion_detection_high_threshold[Z_AXIS] = MP.gyroscope_high_threshold[Z_AXIS];

    config.roll_motion_detection_low_threshold = MP.angle_low_threshold[ROLL];
    config.pitch_motion_detection_low_threshold = MP.angle_low_threshold[PITCH];
    config.yaw_motion_detection_low_threshold = MP.angle_low_threshold[YAW];
    config.roll_motion_detection_high_threshold = MP.angle_high_threshold[ROLL];
    config.pitch_motion_detection_high_threshold = MP.angle_high_threshold[PITCH];
    config.yaw_motion_detection_high_threshold = MP.angle_high_threshold[YAW];

    config.roll_filter_constant = MP.roll_filter_constant;
    config.pitch_filter_constant = MP.pitch_filter_constant;

    uint16_t size = sizeof(config);

    EEPROM_write_bytes(0,(void*)&config,sizeof(config));

}

