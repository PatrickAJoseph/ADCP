
#include "MPU9250.h"

MPU9250_t volatile MPU9250;

/*
 * @brief:                          Writes value into target register.
 * @parameter register_number:      Target register number.
 * @parameter value:                Value to be written into the register.
 * @return:                         None.
 *
 * */

void MPU9250_write_register(uint8_t number, uint8_t value)
{
  uint8_t transmit_buffer[2] = { number, value };
  uint8_t receive_buffer[2]  = {0};

  SPI_Transfer(transmit_buffer,receive_buffer,2);
}


/*
 * @brief:                          Reads value from target register.
 * @parameter register_number:      Target register number.
 * @return:                         None.
 *
 * */

uint8_t MPU9250_read_register(uint8_t number)
{
    uint8_t transmit_buffer[2] = { number | 0x80 , 0 };
    uint8_t receive_buffer[2]  = {0};

    SPI_Transfer(transmit_buffer,receive_buffer,2);

    return(receive_buffer[1]);
}

/*
 * @brief:  Initializes MPU9250 sensor by enabling SPI communication and taking device
 *          out of reset mode. It also enables all sensors (gyroscope & accelerometer)
 *          available with the sensor.
 * */

void MPU9250_Init()
{
  uint8_t temp = 0;

  /* Perform power reset for device. */

  __delay_cycles(160000);

  MPU9250_write_register(MPU9250_POWER_MANAGEMENT_1,MPU9250_PM1_RESET);

  /* Wait for 200 milli-seconds for the power rail to set. */

  __delay_cycles(160000);

  MPU9250_write_register(MPU9250_POWER_MANAGEMENT_1,MPU9250_PM1_RESET);

  __delay_cycles(160000);

  /* Set comm. mode to I2C and reset all signal paths. */

  temp =    MPU9250_USER_CONTROL_DISABLE_I2C;
  temp |=   MPU9250_USER_CONTROL_SIG_COND_RESET;

  MPU9250_write_register(MPU9250_USER_CONTROL,temp);
  __delay_cycles(800);

  /* Enable all sensors (accelerometer and gyroscope ). */

  MPU9250_write_register(MPU9250_POWER_MANAGEMENT_2,0);
  __delay_cycles(800);

  /* Clear interrupt enable bits. */

  MPU9250_write_register(MPU9250_INTERRUPT_ENABLE,0);
  __delay_cycles(800);

  /* User-specific configurations. */

  MPU9250_configure();
  __delay_cycles(800);

  /* Enable ready data interrupt for proper sampling. */

  MPU9250_write_register(MPU9250_INTERRUPT_ENABLE,MPU9250_INTERRUPT_ENABLE_RAW_DATA_READY_ENABLE);
  __delay_cycles(800);

  MPU9250_write_register(MPU9250_INT_PIN_CONFIG,MPU9250_INT_PIN_CONFIG_INT_PIN_CLEAR_ON_DATA_READ |
                         MPU9250_INT_PIN_CONFIG_OUTPUT_DRIVE_PUSH_PULL |
                         MPU9250_INT_PIN_CONFIG_ACTIVE_LEVEL_HIGH |
                         MPU9250_INT_PIN_CONFIG_LATCH_MODE_LATCHED |
                         MPU9250_INT_PIN_CONFIG_INT_PIN_CLEAR_ON_STATUS_READ );
  __delay_cycles(800);

  MPU9250.configuration_A.configuration_A_segments.data_available = 0;

  /* User-specific configurations. */

  MPU9250_configure();

  /* Load data into communication frames. */

  IMU_CONFIG_REG_A.IMU_CONFIG_REG_A_segments.IMU_CONFIG_CALIB_INTERVAL = MPU9250.calibration_interval_ms;
  IMU_CONFIG_REG_A.IMU_CONFIG_REG_A_segments.IMU_CONFIG_CALIB_POINTS   = MPU9250.calibration_point_count;
  IMU_CONFIG_REG_A.IMU_CONFIG_REG_A_segments.IMU_CONFIG_SAMPLING_RATE  = MPU9250.sampling_rate;

  IMU_CONFIG_REG_B.IMU_CONFIG_REG_B_segments.IMU_CONFIG_ACCEL_BW       = MPU9250.configuration_A.configuration_A_segments.accelerometer_filter_bandwidth;
  IMU_CONFIG_REG_B.IMU_CONFIG_REG_B_segments.IMU_CONFIG_ACCEL_FS       = MPU9250.configuration_A.configuration_A_segments.accelerometer_full_scale;
  IMU_CONFIG_REG_B.IMU_CONFIG_REG_B_segments.IMU_CONFIG_GYRO_BW        = MPU9250.configuration_A.configuration_A_segments.gyroscope_filter_bandwidth;
  IMU_CONFIG_REG_B.IMU_CONFIG_REG_B_segments.IMU_CONFIG_GYRO_FS        = MPU9250.configuration_A.configuration_A_segments.gyroscope_full_scale;
}

/*
 * @brief:                          Checks if connection with sensor exists by checking
 *                                  the WHO_AM_I register.
 * @return:                         1 if connection is valid, 0 is invalid.
 *
 * */

uint8_t MPU9250_present()
{
    return( 0x70 == MPU9250_read_register(MPU9250_WHO_AM_I) );
}

/*
 * @brief:                          Checks if sensor data is ready.
 * @parameter:                      Timeout in milli-seconds.
 * @return:                         1 if sensor data ready, else returns 0.
 *
 * */

uint8_t MPU9250_data_ready(uint16_t timeout_ms)
{
    while( !( MPU9250_read_register(MPU9250_INTERRUPT_STATUS) & MPU9250_INTERRUPT_STATUS_RAW_DATA_READY ) && (timeout_ms != 0) )
    {
        timeout_ms--;
        __delay_cycles(8000);
    }

    return(timeout_ms != 0);
}

/*
 *
 * @brief:                          Set accelerometer full-scale range.
 * @parameter:                      Function to update configuration to device.
 * @return:                         None.
 *
 * */

void MPU9250_configure()
{
    uint8_t tempReg = 0;

    /* Load configurations associated with the accelerometer first. */

    tempReg = MPU9250_read_register(MPU9250_ACCEL_CONFIG);
    tempReg &= ~MPU9250_ACCEL_CONFIG_ACCEL_FS_Mask;

    switch(MPU9250.configuration_A.configuration_A_segments.accelerometer_full_scale)
    {
        case(ACCEL_FS_2G)   :{ tempReg |= MPU9250_ACCEL_CONFIG_ACCEL_FS_2G; }; break;
        case(ACCEL_FS_4G)   :{ tempReg |= MPU9250_ACCEL_CONFIG_ACCEL_FS_4G; }; break;
        case(ACCEL_FS_8G)   :{ tempReg |= MPU9250_ACCEL_CONFIG_ACCEL_FS_8G; }; break;
        case(ACCEL_FS_16G)  :{ tempReg |= MPU9250_ACCEL_CONFIG_ACCEL_FS_16G; }; break;
    }

    MPU9250_write_register(MPU9250_ACCEL_CONFIG,tempReg);


    /* Set gyroscope full-scale value. */

    tempReg = MPU9250_read_register(MPU9250_GYRO_CONFIG);
    tempReg &= ~MPU9250_GYRO_CONFIG_GYRO_FS_Mask;

    switch(MPU9250.configuration_A.configuration_A_segments.gyroscope_full_scale)
    {
        case(GYRO_FS_250DPS)    :{ tempReg |= MPU9250_GYRO_CONFIG_GYRO_FS_250DPS; }; break;
        case(GYRO_FS_500DPS)    :{ tempReg |= MPU9250_GYRO_CONFIG_GYRO_FS_500DPS; }; break;
        case(GYRO_FS_1000DPS)   :{ tempReg |= MPU9250_GYRO_CONFIG_GYRO_FS_1000DPS; }; break;
        case(GYRO_FS_2000DPS)   :{ tempReg |= MPU9250_GYRO_CONFIG_GYRO_FS_2000DPS; }; break;
    }

    MPU9250_write_register(MPU9250_GYRO_CONFIG,tempReg);

    /* Check for filter configuration for gyroscope. */

    //  Enable/ disable gyroscope filter first.

    tempReg = MPU9250_read_register(MPU9250_GYRO_CONFIG);
    tempReg &= ~MPU9250_GYRO_DLPF_ENABLE_DISABLE_Mask;

    if(MPU9250.configuration_A.configuration_A_segments.filter_enable)
    {
        tempReg |= MPU9250_GYRO_DLPF_ENABLE;
    }
    else
    {
        tempReg |= MPU9250_GYRO_DLPF_BYPASS;
    }

    MPU9250_write_register(MPU9250_GYRO_CONFIG,tempReg);

    //  Configure gyroscope DLPF.

    tempReg = MPU9250_read_register(MPU9250_CONFIG);

    tempReg &= ~(MPU9250_CONFIG_DLPF_CONFIG_Mask);

    if( MPU9250.configuration_A.configuration_A_segments.filter_enable )
    {
        switch(MPU9250.configuration_A.configuration_A_segments.gyroscope_filter_bandwidth)
        {
            case(GYRO_DLPF_BW_184Hz)   :{ tempReg |= MPU9250_CONFIG_DLPF_CONFIG_1; }; break;
            case(GYRO_DLPF_BW_92Hz)    :{ tempReg |= MPU9250_CONFIG_DLPF_CONFIG_2; }; break;
            case(GYRO_DLPF_BW_41Hz)    :{ tempReg |= MPU9250_CONFIG_DLPF_CONFIG_3; }; break;
            case(GYRO_DLPF_BW_20Hz)    :{ tempReg |= MPU9250_CONFIG_DLPF_CONFIG_4; }; break;
            case(GYRO_DLPF_BW_10Hz)    :{ tempReg |= MPU9250_CONFIG_DLPF_CONFIG_5; }; break;
            case(GYRO_DLPF_BW_5Hz)     :{ tempReg |= MPU9250_CONFIG_DLPF_CONFIG_6; }; break;
        }
    }

    MPU9250_write_register(MPU9250_CONFIG,tempReg);

    /* Check for filter configuration for accelerometer. */

    tempReg = MPU9250_read_register(MPU9250_ACCEL_CONFIG_2);

    tempReg &= ~(MPU9250_ACCEL_CONFIG_2_DLPF_ENABLE_DISABLE_Mask);
    tempReg &= ~(MPU9250_ACCEL_CONFIG_2_DLPF_CONFIG_Mask);

    if( MPU9250.configuration_A.configuration_A_segments.filter_enable )
    {
        tempReg |= MPU9250_ACCEL_CONFIG_2_DLPF_ENABLE;

        switch(MPU9250.configuration_A.configuration_A_segments.accelerometer_filter_bandwidth)
        {
            case(ACCEL_DLPF_BW_460Hz)   :{ tempReg |= MPU9250_ACCEL_CONFIG_2_DLPF_CONFIG_0; }; break;
            case(ACCEL_DLPF_BW_184Hz)   :{ tempReg |= MPU9250_ACCEL_CONFIG_2_DLPF_CONFIG_1; }; break;
            case(ACCEL_DLPF_BW_92Hz)    :{ tempReg |= MPU9250_ACCEL_CONFIG_2_DLPF_CONFIG_2; }; break;
            case(ACCEL_DLPF_BW_41Hz)    :{ tempReg |= MPU9250_ACCEL_CONFIG_2_DLPF_CONFIG_3; }; break;
            case(ACCEL_DLPF_BW_20Hz)    :{ tempReg |= MPU9250_ACCEL_CONFIG_2_DLPF_CONFIG_4; }; break;
            case(ACCEL_DLPF_BW_10Hz)    :{ tempReg |= MPU9250_ACCEL_CONFIG_2_DLPF_CONFIG_5; }; break;
            case(ACCEL_DLPF_BW_5Hz)     :{ tempReg |= MPU9250_ACCEL_CONFIG_2_DLPF_CONFIG_6; }; break;
        }
    }
    else
    {
        tempReg |= MPU9250_ACCEL_CONFIG_2_DLPF_DISABLE;
    }

    MPU9250_write_register(MPU9250_ACCEL_CONFIG_2,tempReg);


    /* Set sampling rate. */

    if( ( MPU9250.sampling_rate > 0 ) && ( MPU9250.sampling_rate <= 1000 ) )
    {
        MPU9250_write_register(MPU9250_SMPLRT_DIV,(1000/MPU9250.sampling_rate) - 1);
    }
    else
    {
        MPU9250.sampling_rate = 1000;
        MPU9250_write_register(MPU9250_SMPLRT_DIV,0);
    }


    uint8_t __sampling_rate = 0;
    uint8_t bkpt = 0;
    uint8_t configurations[4];

    /* Verification of configurations. */

    configurations[0] = MPU9250_read_register(MPU9250_CONFIG);
    configurations[1] = MPU9250_read_register(MPU9250_ACCEL_CONFIG_2);
    configurations[2] = MPU9250_read_register(MPU9250_GYRO_CONFIG);
    configurations[3] = MPU9250_read_register(MPU9250_SMPLRT_DIV);

    bkpt = bkpt;
}


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

void MPU9250_read_sensor_data()
{
  uint8_t buffer[15] = {0};

  buffer[0] = ( 0x80 | MPU9250_ACCEL_XOUT_H );

  SPI_Transfer(buffer,buffer,15);

  uint16_t tempReg;
  int32_t tempI32;

  uint8_t i = 0;
  uint8_t is_negative = 0;

  /* Get accelerometer data. */


  for(i = 0 ; i < 3 ; i++)
  {
      tempReg = ( (((uint16_t)buffer[2*i+1]) << 8) | (uint16_t)buffer[2*i+2] );
      tempI32 = (int32_t)tempReg;

      if( tempI32 > 32767 )
      {
          is_negative = 1;
          tempI32 = (65536 - tempI32);
      }
      else
      {
          is_negative = 0;
      }

      tempI32 *= ( 2000 * (int32_t)( 1 << MPU9250.configuration_A.configuration_A_segments.accelerometer_full_scale ) );
      tempI32 >>= 15;

      if( is_negative )
      {
          tempI32 = -tempI32;
      }

      MPU9250.accel[i] = (tempI32 - MPU9250.accel_offset[i]);
  }

  /* Get temperature sensor data. */

  tempReg = (((uint16_t)buffer[7] << 8) | (uint16_t)buffer[8]);

  MPU9250.temperature = tempReg;

  /* Get gyroscope data. */

  for(i = 0 ; i < 3 ; i++)
  {
      tempReg = ( (((uint16_t)buffer[2*i+9]) << 8) | (uint16_t)buffer[2*i+10] );
      tempI32 = (int32_t)tempReg;

      if( tempI32 > 32767 )
      {
          is_negative = 1;
          tempI32 = (65536 - tempI32);
      }
      else
      {
          is_negative = 0;
      }

      tempI32 *= ( 250000 * (int32_t)( 1 << MPU9250.configuration_A.configuration_A_segments.gyroscope_full_scale ) );
      tempI32 >>= 15;

      if( is_negative )
      {
          tempI32 = -tempI32;
      }

      MPU9250.gyro[i] = (tempI32 - MPU9250.gyro_offset[i]);
  }

  IMU_X_RAW.IMU_X_RAW_segments.IMU_X_ACCEL_RAW = ( (((uint16_t)buffer[1]) << 8) + (uint16_t)buffer[2] );
  IMU_X_RAW.IMU_X_RAW_segments.IMU_X_GYRO_RAW  = ( (((uint16_t)buffer[9]) << 8) + (uint16_t)buffer[10] );
  IMU_Y_RAW.IMU_Y_RAW_segments.IMU_Y_ACCEL_RAW = ( (((uint16_t)buffer[3]) << 8) + (uint16_t)buffer[4] );
  IMU_Y_RAW.IMU_Y_RAW_segments.IMU_Y_GYRO_RAW  = ( (((uint16_t)buffer[11]) << 8) + (uint16_t)buffer[12] );
  IMU_Z_RAW.IMU_Z_RAW_segments.IMU_Z_ACCEL_RAW = ( (((uint16_t)buffer[5]) << 8) + (uint16_t)buffer[6] );
  IMU_Z_RAW.IMU_Z_RAW_segments.IMU_Z_GYRO_RAW  = ( (((uint16_t)buffer[13]) << 8) + (uint16_t)buffer[14] );

  IMU_X_PROCESSED.IMU_X_PROCESSED_segments.IMU_X_ACCEL_PROCESSED = MPU9250.accel[X_AXIS];
  IMU_X_PROCESSED.IMU_X_PROCESSED_segments.IMU_X_GYRO_PROCESSED  = (MPU9250.gyro[X_AXIS] / 100);
  IMU_Y_PROCESSED.IMU_Y_PROCESSED_segments.IMU_Y_ACCEL_PROCESSED = MPU9250.accel[Y_AXIS];
  IMU_Y_PROCESSED.IMU_Y_PROCESSED_segments.IMU_Y_GYRO_PROCESSED  = (MPU9250.gyro[Y_AXIS] / 100);
  IMU_Z_PROCESSED.IMU_Z_PROCESSED_segments.IMU_Z_ACCEL_PROCESSED = MPU9250.accel[Z_AXIS];
  IMU_Z_PROCESSED.IMU_Z_PROCESSED_segments.IMU_Z_GYRO_PROCESSED  = (MPU9250.gyro[Z_AXIS] / 100);
}

/*
 * @brief:                          Function to perform calibration for IMU sensor with IMU sensor kept
 *                                  flat on the ground, normal to gravitational field.
 *
 * @parameters:                     None.
 * @details:                        This function must be called only after loading required configuration
 *                                  in the IMU structure.
 * */

void MPU9250_calibrate()
{
    uint8_t i = 0;
    uint8_t j = 0;
    uint8_t k = 0;

    int32_t offset[6] = {0};

    MPU9250.accel_offset[0] = 0;
    MPU9250.accel_offset[1] = 0;
    MPU9250.accel_offset[2] = 0;
    MPU9250.gyro_offset[0] = 0;
    MPU9250.gyro_offset[1] = 0;
    MPU9250.gyro_offset[2] = 0;

    for( i = 0 ; i < MPU9250.calibration_point_count ; i++)
    {
        if( MPU9250_data_ready(MPU9250.calibration_interval_ms) )
        {
            MPU9250_read_sensor_data();

            for(k = 0 ; k < 3 ; k++)
            {
                offset[k]       +=  (int32_t)MPU9250.accel[k];
                offset[3+k]     +=  (int32_t)MPU9250.gyro[k];
            }
        }


        for( j = 0 ; j < MPU9250.calibration_interval_ms ; j++)
        {
            __delay_cycles(24000);
        }
    }

    for( i = 0 ; i < 6 ; i++ )
    {
        offset[i] /= (int32_t)MPU9250.calibration_point_count;
    }

    MPU9250.accel_offset[X_AXIS]    = offset[0];
    MPU9250.accel_offset[Y_AXIS]    = offset[1];
    MPU9250.accel_offset[Z_AXIS]    = (offset[2] - 1000);
    MPU9250.gyro_offset[X_AXIS]     = offset[3];
    MPU9250.gyro_offset[Y_AXIS]     = offset[4];
    MPU9250.gyro_offset[Z_AXIS]     = offset[5];
}
