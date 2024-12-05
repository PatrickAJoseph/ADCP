
#ifndef MPU9250_H_
#define MPU9250_H_

#include <limits.h>
#include "spi.h"
#include "ADCP_frames.h"


/*******************    MPU9250 Registers.      *********************/

/* NOTE: Only essential registers are implemented as of now. */

// Registers 19 to 24: Gyroscope offset registers.

#define MPU9250_GYRO_X_OFFSET_H                 (19)
#define MPU9250_GYRO_X_OFFSET_L                 (20)
#define MPU9250_GYRO_Y_OFFSET_H                 (21)
#define MPU9250_GYRO_Y_OFFSET_L                 (22)
#define MPU9250_GYRO_Z_OFFSET_H                 (23)
#define MPU9250_GYRO_Z_OFFSET_L                 (24)

//  Register 25: Sample rate divider.

#define MPU9250_SMPLRT_DIV                      (25)

//  Register 26: Configuration.

#define MPU9250_CONFIG                          (26)

#define MPU9250_CONFIG_FIFO_MODE_Mask           ( 1 << 6 )
#define MPU9250_CONFIG_FIFO_MODE_NO_OVERWRITE   ( 1 << 6 )
#define MPU9250_CONFIG_FIFO_MODE_OVERWRITE      ( 0 << 6 )

#define MPU9250_CONFIG_EXT_SYNC_SET_Mask        ( 7 << 3 )
#define MPU9250_CONFIG_EXT_SYNC_SET_None        ( 0 << 3 )
#define MPU9250_CONFIG_EXT_SYNC_SET_TEMP_OUT_L  ( 1 << 3 )
#define MPU9250_CONFIG_EXT_SYNC_GYRO_XOUT_L     ( 2 << 3 )
#define MPU9250_CONFIG_EXT_SYNC_GYRO_YOUT_L     ( 3 << 3 )
#define MPU9250_CONFIG_EXT_SYNC_GYRO_ZOUT_L     ( 4 << 3 )
#define MPU9250_CONFIG_EXT_SYNC_ACCEL_XOUT_L    ( 5 << 3 )
#define MPU9250_CONFIG_EXT_SYNC_ACCEL_YOUT_L    ( 6 << 3 )
#define MPU9250_CONFIG_EXT_SYNC_ACCEL_ZOUT_L    ( 7 << 3 )

#define MPU9250_CONFIG_DLPF_CONFIG_Mask                ( 7 << 0 )
#define MPU9250_CONFIG_DLPF_CONFIG_0                   ( 0 << 0 )
#define MPU9250_CONFIG_DLPF_CONFIG_1                   ( 1 << 0 )
#define MPU9250_CONFIG_DLPF_CONFIG_2                   ( 2 << 0 )
#define MPU9250_CONFIG_DLPF_CONFIG_3                   ( 3 << 0 )
#define MPU9250_CONFIG_DLPF_CONFIG_4                   ( 4 << 0 )
#define MPU9250_CONFIG_DLPF_CONFIG_5                   ( 5 << 0 )
#define MPU9250_CONFIG_DLPF_CONFIG_6                   ( 6 << 0 )
#define MPU9250_CONFIG_DLPF_CONFIG_7                   ( 7 << 0 )


//  Register 27:    Gyroscope configurations.

#define MPU9250_GYRO_CONFIG                     (27)
#define MPU9250_GYRO_CONFIG_GYRO_FS_Mask        ( 3 << 3 )
#define MPU9250_GYRO_CONFIG_GYRO_FS_250DPS      ( 0 << 3 )
#define MPU9250_GYRO_CONFIG_GYRO_FS_500DPS      ( 1 << 3 )
#define MPU9250_GYRO_CONFIG_GYRO_FS_1000DPS     ( 2 << 3 )
#define MPU9250_GYRO_CONFIG_GYRO_FS_2000DPS     ( 3 << 3 )
#define MPU9250_GYRO_DLPF_ENABLE_DISABLE_Mask   ( 3 )
#define MPU9250_GYRO_DLPF_BYPASS                ( 3 )
#define MPU9250_GYRO_DLPF_ENABLE                ( 0 )

//  Register 28:    Accelerometer configurations.

#define MPU9250_ACCEL_CONFIG                    (28)
#define MPU9250_ACCEL_CONFIG_ACCEL_FS_Mask      ( 3 << 3 )
#define MPU9250_ACCEL_CONFIG_ACCEL_FS_2G        ( 0 << 3 )
#define MPU9250_ACCEL_CONFIG_ACCEL_FS_4G        ( 1 << 3 )
#define MPU9250_ACCEL_CONFIG_ACCEL_FS_8G        ( 2 << 3 )
#define MPU9250_ACCEL_CONFIG_ACCEL_FS_16G       ( 3 << 3 )

//  Register 29:    Accelerometer configuration 2.

#define MPU9250_ACCEL_CONFIG_2                           (29)
#define MPU9250_ACCEL_CONFIG_2_DLPF_ENABLE               ( 0 << 3 )
#define MPU9250_ACCEL_CONFIG_2_DLPF_DISABLE              ( 1 << 3 )
#define MPU9250_ACCEL_CONFIG_2_DLPF_ENABLE_DISABLE_Mask  ( 1 << 3 )
#define MPU9250_ACCEL_CONFIG_2_DLPF_CONFIG_Mask          (7)
#define MPU9250_ACCEL_CONFIG_2_DLPF_CONFIG_0             (0)
#define MPU9250_ACCEL_CONFIG_2_DLPF_CONFIG_1             (1)
#define MPU9250_ACCEL_CONFIG_2_DLPF_CONFIG_2             (2)
#define MPU9250_ACCEL_CONFIG_2_DLPF_CONFIG_3             (3)
#define MPU9250_ACCEL_CONFIG_2_DLPF_CONFIG_4             (4)
#define MPU9250_ACCEL_CONFIG_2_DLPF_CONFIG_5             (5)
#define MPU9250_ACCEL_CONFIG_2_DLPF_CONFIG_6             (6)
#define MPU9250_ACCEL_CONFIG_2_DLPF_CONFIG_7             (7)

// Register number 35: FIFO Enable register.

#define MPU9250_FIFO_ENABLE                             (35)
#define MPU9250_FIFO_ENABLE_TEMP_OUT_ENABLE             ( 1 << 7 )
#define MPU9250_FIFO_ENABLE_TEMP_OUT_DISABLE            ( 0 << 7 )
#define MPU9250_FIFO_TEMP_OUT_Mask               ( 1 << 7 )
#define MPU9250_FIFO_ENABLE_GYRO_XOUT_ENABLE            ( 1 << 6 )
#define MPU9250_FIFO_ENABLE_GYRO_XOUT_DISABLE           ( 0 << 6 )
#define MPU9250_FIFO_GYRO_XOUT_Mask              ( 1 << 6 )
#define MPU9250_FIFO_ENABLE_GYRO_YOUT_ENABLE            ( 1 << 5 )
#define MPU9250_FIFO_ENABLE_GYRO_YOUT_DISABLE           ( 0 << 5 )
#define MPU9250_FIFO_GYRO_YOUT_Mask              ( 1 << 5 )
#define MPU9250_FIFO_ENABLE_GYRO_ZOUT_ENABLE            ( 1 << 4 )
#define MPU9250_FIFO_ENABLE_GYRO_ZOUT_DISABLE           ( 0 << 4 )
#define MPU9250_FIFO_GYRO_ZOUT_Mask              ( 1 << 4 )
#define MPU9250_FIFO_ENABLE_ACCEL_ENABLE                ( 1 << 3 )
#define MPU9250_FIFO_ENABLE_ACCEL_DISABLE               ( 0 << 3 )
#define MPU9250_FIFO_ENABLE_ACCEL_Mask                  ( 1 << 3 )


//  Register number 55:     Interrupt pin configuration.

#define MPU9250_INT_PIN_CONFIG                          (55)

#define MPU9250_INT_PIN_CONFIG_ACTIVE_LEVEL_LOW         ( 1 << 7 )
#define MPU9250_INT_PIN_CONFIG_ACTIVE_LEVEL_HIGH        ( 0 << 7 )
#define MPU9250_INT_PIN_CONFIG_ACTIVE_LEVEL_Mask        ( 1 << 7 )

#define MPU9250_INT_PIN_CONFIG_OUTPUT_DRIVE_OPEN_DRAIN  ( 1 << 6 )
#define MPU9250_INT_PIN_CONFIG_OUTPUT_DRIVE_PUSH_PULL   ( 0 << 6 )
#define MPU9250_INT_PIN_CONFIG_OUTPUT_DRIVE_Mask        ( 1 << 6 )

#define MPU9250_INT_PIN_CONFIG_LATCH_MODE_LATCHED       ( 1 << 5 )
#define MPU9250_INT_PIN_CONFIG_LATCH_MODE_PULSE         ( 0 << 5 )
#define MPU9250_INT_PIN_CONFIG_LATCH_MODE_Mask          ( 1 << 5 )

#define MPU9250_INT_PIN_CONFIG_INT_PIN_CLEAR_ON_DATA_READ       ( 1 << 4 )
#define MPU9250_INT_PIN_CONFIG_INT_PIN_CLEAR_ON_STATUS_READ     ( 0 << 4 )
#define MPU9250_INT_PIN_CONFIG_INT_PIN_CLEAR_Mask               ( 1 << 4 )

//  Register number 56:     Interrupt enable.

#define MPU9250_INTERRUPT_ENABLE                        (56)

#define MPU9250_INTERRUPT_ENABLE_WOM_ENABLE             ( 1 << 6 )
#define MPU9250_INTERRUPT_ENABLE_WOM_DISABLE            ( 0 << 6 )
#define MPU9250_INTERRUPT_ENABLE_WOM_Mask               ( 1 << 6 )

#define MPU9250_INTERRUPT_ENABLE_FIFO_OVERFLOW_ENABLE   ( 1 << 4 )
#define MPU9250_INTERRUPT_ENABLE_FIFO_OVERFLOW_DISABLE  ( 0 << 4 )
#define MPU9250_INTERRUPT_ENABLE_FIFO_OVERFLOW_Mask     ( 1 << 4 )

#define MPU9250_INTERRUPT_ENABLE_RAW_DATA_READY_ENABLE  ( 1 << 0 )
#define MPU9250_INTERRUPT_ENABLE_RAW_DATA_READY_DISABLE ( 0 << 0 )
#define MPU9250_INTERRUPT_ENABLE_RAW_DATA_READY_Mask    ( 1 << 0 )

//  Register number 58: Interrupt status.

#define MPU9250_INTERRUPT_STATUS                        (58)

#define MPU9250_INTERRUPT_STATUS_WOM                    ( 1 << 6 )
#define MPU9250_INTERRUPT_STATUS_FIFO_OVERFLOW          ( 1 << 4 )
#define MPU9250_INTERRUPT_STATUS_RAW_DATA_READY         ( 1 << 0 )

//  Register numbers 59 - 64: Accelerometer measurement registers.

#define MPU9250_ACCEL_XOUT_H                            (59)
#define MPU9250_ACCEL_XOUT_L                            (60)
#define MPU9250_ACCEL_YOUT_H                            (61)
#define MPU9250_ACCEL_YOUT_L                            (62)
#define MPU9250_ACCEL_ZOUT_H                            (63)
#define MPU9250_ACCEL_ZOUT_L                            (64)

//  Register numbers 65 - 66: Temperature sensor registers.

#define MPU9250_TEMP_OUT_H                              (65)
#define MPU9250_TEMP_OUT_L                              (66)

//  Register numbers 67 - 72: Gyroscope measurement registers.

#define MPU9250_GYRO_XOUT_H                            (67)
#define MPU9250_GYRO_XOUT_L                            (68)
#define MPU9250_GYRO_YOUT_H                            (69)
#define MPU9250_GYRO_YOUT_L                            (70)
#define MPU9250_GYRO_ZOUT_H                            (71)
#define MPU9250_GYRO_ZOUT_L                            (72)

//  Register number 104:    Signal path reset.

#define MPU9250_SIGNAL_PATH_RESET                      (104)

#define MPU9250_SIGNAL_PATH_RESET_GYRO_RESET            ( 1 << 2 )
#define MPU9250_SIGNAL_PATH_RESET_ACCEL_RESET           ( 1 << 1 )
#define MPU9250_SIGNAL_PATH_RESET_TEMP_RESET            ( 1 << 0 )

//  Register number 106:    User control.

#define MPU9250_USER_CONTROL                           (106)

#define MPU9250_USER_CONTROL_FIFO_ENABLE                ( 1 << 6 )
#define MPU9250_USER_CONTROL_FIFO_DISABLE               ( 0 << 6 )
#define MPU9250_USER_CONTROL_FIFO_ENABLE_DISABLE_Mask   ( 1 << 6 )

#define MPU9250_USER_CONTROL_DISABLE_I2C                ( 1 << 4 )
#define MPU9250_USER_CONTROL_ENABLE_I2C                 ( 0 << 4 )
#define MPU9250_USER_CONTROL_ENABLE_DISABLE_I2C_Mask    ( 1 << 4 )

#define MPU9250_USER_CONTROL_RESET_FIFO                 ( 1 << 2 )
#define MPU9250_USER_CONTROL_SIG_COND_RESET             ( 1 << 0 )

//  Register number 107:    Power management 1.

#define MPU9250_POWER_MANAGEMENT_1                     (107)

#define MPU9250_PM1_RESET                               ( 1 << 7 )          //  Reset device.
#define MPU9250_PM1_SLEEP_MODE                          ( 1 << 6 )
#define MPU9250_PM1_CYCLE                               ( 1 << 5 )
#define MPU9250_PM1_GYRO_STANDBY                        ( 1 << 4 )
#define MPU9250_PM1_PD_PTAT                             ( 1 << 3 )
#define MPU9250_PM1_CLOCK_SOURCE_20MHz                  (0)
#define MPU9250_PM1_CLOCK_SOURCE_Mask                   (7)

//  Register number 108:    Power management 2.

#define MPU9250_POWER_MANAGEMENT_2                      (108)

#define MPU9250_PM2_DISABLE_XA                          ( 1 << 5 )
#define MPU9250_PM2_DISABLE_YA                          ( 1 << 4 )
#define MPU9250_PM2_DISABLE_ZA                          ( 1 << 3 )
#define MPU9250_PM2_DISABLE_XG                          ( 1 << 2 )
#define MPU9250_PM2_DISABLE_YG                          ( 1 << 1 )
#define MPU9250_PM2_DISABLE_ZG                          ( 1 << 0 )
#define MPU9250_PM2_ENABLE_XA                          ( 0 << 5 )
#define MPU9250_PM2_ENABLE_YA                          ( 0 << 4 )
#define MPU9250_PM2_ENABLE_ZA                          ( 0 << 3 )
#define MPU9250_PM2_ENABLE_XG                          ( 0 << 2 )
#define MPU9250_PM2_ENABLE_YG                          ( 0 << 1 )
#define MPU9250_PM2_ENABLE_ZG                          ( 0 << 0 )

//  Register numbers 114- 115: FIFO count registers.

#define MPU9250_FIFO_COUNT_H                            (114)
#define MPU9250_FIFO_COUNT_L                            (115)

//  Register numbers 116: FIFO read-write.

#define MPU9250_FIFO_R_W                                (116)

//  Register number 117:   WHO AM I register.

#define MPU9250_WHO_AM_I                                (117)

//  Register numbers 119- 126: Accelerometer offset registers.

#define MPU9250_ACCEL_X_OFFSET_H                        (119)
#define MPU9250_ACCEL_X_OFFSET_L                        (120)
#define MPU9250_ACCEL_Y_OFFSET_H                        (122)
#define MPU9250_ACCEL_Y_OFFSET_L                        (123)
#define MPU9250_ACCEL_Z_OFFSET_H                        (125)
#define MPU9250_ACCEL_Z_OFFSET_L                        (126)

/* Accelerometer and gyroscope full-scale ranges. */

#define ACCEL_FS_2G                                     0
#define ACCEL_FS_4G                                     1
#define ACCEL_FS_8G                                     2
#define ACCEL_FS_16G                                    3

#define GYRO_FS_250DPS                                  0
#define GYRO_FS_500DPS                                  1
#define GYRO_FS_1000DPS                                 2
#define GYRO_FS_2000DPS                                 3

/* Bandwidth for filters. */

#define ACCEL_DLPF_BW_460Hz                             0
#define ACCEL_DLPF_BW_184Hz                             1
#define ACCEL_DLPF_BW_92Hz                              2
#define ACCEL_DLPF_BW_41Hz                              3
#define ACCEL_DLPF_BW_20Hz                              4
#define ACCEL_DLPF_BW_10Hz                              5
#define ACCEL_DLPF_BW_5Hz                               6

#define GYRO_DLPF_BW_184Hz                              1
#define GYRO_DLPF_BW_92Hz                               2
#define GYRO_DLPF_BW_41Hz                               3
#define GYRO_DLPF_BW_20Hz                               4
#define GYRO_DLPF_BW_10Hz                               5
#define GYRO_DLPF_BW_5Hz                                6

#define X_AXIS                                          0
#define Y_AXIS                                          1
#define Z_AXIS                                          2

/* Structure to contain information about the IMU sensor. */

typedef struct
{
  uint16_t _align;

  /* Accelerometer readings in G * 1000. */

  int32_t accel[3];
  int32_t accel_offset[3];

  /* Gyroscope readings in DPS * 1000. */

  int32_t gyro[3];
  int32_t gyro_offset[3];

  /* Temperature in degrees * 1000. */

  int32_t temperature;

  union configuration_struct_A
  {
      uint16_t all;

      struct configuration_segments_A
      {
          uint16_t  accelerometer_full_scale        :2;
          uint16_t  gyroscope_full_scale            :2;
          uint16_t  filter_enable                   :1;
          uint16_t  accelerometer_filter_bandwidth  :3;
          uint16_t  gyroscope_filter_bandwidth      :3;
          uint16_t  data_available                  :1;
      }configuration_A_segments;

  }configuration_A;


  uint16_t calibration_interval_ms;
  uint16_t calibration_point_count;
  uint16_t sampling_rate;

}MPU9250_t;


extern volatile MPU9250_t MPU9250;

/*
 * @brief:  Initializes MPU9250 sensor by enabling SPI communication and taking device
 *          out of reset mode. It also enables all sensors (gyroscope & accelerometer)
 *          available with the sensor.
 * */

void MPU9250_Init();

/*
 * @brief:                          Writes value into target register.
 * @parameter register_number:      Target register number.
 * @parameter value:                Value to be written into the register.
 * @return:                         None.
 *
 * */

void MPU9250_write_register(uint8_t number, uint8_t value);


/*
 * @brief:                          Reads value from target register.
 * @parameter register_number:      Target register number.
 * @return:                         None.
 *
 * */

uint8_t MPU9250_read_register(uint8_t number);

/*
 * @brief:                          Checks if connection with sensor exists by checking
 *                                  the WHO_AM_I register.
 * @return:                         1 if connection is valid, 0 is invalid.
 *
 * */

uint8_t MPU9250_present();

/*
 *
 * @brief:                          Set accelerometer full-scale range.
 * @parameter:                      Function to update configuration to device.
 * @return:                         None.
 *
 * */

void MPU9250_configure();

/*
 * @brief:                          Function to read sensor data in the following order.
 *                                  ACCEL_XOUT_H, ACCEL_XOUT_L
 *                                  ACCEL_YOUT_H,ACCEL_YOUT_L,
 *                                  ACCEL_ZOUT_H,ACCEL_ZOUT_L,
 *                                  TEMP_OUT_H,TEMP_OUT_L,
 *                                  GYRO_XOUT_H,GYRO_XOUT_L,
 *                                  GYRO_YOUT_H,GYRO_YOUT_L,
 *                                  GYRO_ZOUT_H,GYRO_ZOUT_L.
 * */

void MPU9250_read_sensor_data();

/*
 * @brief:                          Function to perform calibration for IMU sensor with IMU sensor kept
 *                                  flat on the ground, normal to gravitational field.
 *
 * @parameters:                     None.
 * @details:                        This function must be called only after loading required configuration
 *                                  in the IMU structure.
 * */

void MPU9250_calibrate();

/*
 * @brief:                          Checks if sensor data is ready.
 * @parameter:                      Timeout in milli-seconds.
 * @return:                         1 if sensor data ready, else returns 0.
 *
 * */

uint8_t MPU9250_data_ready(uint16_t timeout_ms);

#endif /* MPMPU9250_H_ */
