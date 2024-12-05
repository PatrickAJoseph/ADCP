
#include "LED.h"

LED_config LED1_config;
LED_config LED2_config;
LED_config LED3_config;

#define LED1_PWM_MODE()     { P6SEL1 &= ~BIT0;  P6SEL0 |= BIT0; };
#define LED1_GPIO_MODE()    { P6SEL1 &= ~BIT0;  P6SEL0 &= ~BIT0; };
#define LED2_PWM_MODE()     { P6SEL1 &= ~BIT1;  P6SEL0 |= BIT1; };
#define LED2_GPIO_MODE()    { P6SEL1 &= ~BIT1;  P6SEL0 &= ~BIT1; };
#define LED3_PWM_MODE()     { P6SEL1 &= ~BIT2;  P6SEL0 |= BIT2; };
#define LED3_GPIO_MODE()    { P6SEL1 &= ~BIT2;  P6SEL0 &= ~BIT2; };
#define LED1_SET_DUTY(value)     { TB3CCR1 = LED_map(value); LED1_config.LED_config_segments.LED_duty = value; };
#define LED2_SET_DUTY(value)     { TB3CCR2 = LED_map(value); LED2_config.LED_config_segments.LED_duty = value; };
#define LED3_SET_DUTY(value)     { TB3CCR3 = LED_map(value); LED3_config.LED_config_segments.LED_duty = value; };
#define LED1_SET_STATE(state)    { P6OUT &= ~BIT0; P6OUT |= ( state << 0 ); };
#define LED2_SET_STATE(state)    { P6OUT &= ~BIT1; P6OUT |= ( state << 1 ); };
#define LED3_SET_STATE(state)    { P6OUT &= ~BIT2; P6OUT |= ( state << 2 ); };

void LED_Init()
{
    P6DIR   |= (BIT0|BIT1|BIT2);
    P6SEL0  |= (BIT0|BIT1|BIT2);

    TB3CCR0 = 24000 - 1;                                  //  Set PWM frequency to 1kHz.
    TB3CCTL1 = OUTMOD_7;                                //  Set output mode to non-inverting PWM.
    TB3CCTL2 = OUTMOD_7;                                //  Set output mode to non-inverting PWM.
    TB3CCTL3 = OUTMOD_7;                                //  Set output mode to non-inverting PWM.
                                                        //  Output is ON when timer limit is reached.
    TB3CCR1 = 500;                                        //  Set PWM channel 1.
    TB3CCR2 = 700;                                        //  Set PWM channel 2.
    TB3CCR3 = 250;                                        //  Set PWM channel 3.
                                                        //  Output is OFF when compare value is reached.
    TB3CTL = (TBSSEL__SMCLK | MC__UP | ID__8 | TBCLR ); //  Clock source is SMCLK and counting mode is UP.

    /* Load data into communication frames. */

    LED_MODE_CONFIG.LED_MODE_CONFIG_segments.LED1_MODE = LED1_config.LED_config_segments.LED_mode;
    LED_MODE_CONFIG.LED_MODE_CONFIG_segments.LED2_MODE = LED2_config.LED_config_segments.LED_mode;
    LED_MODE_CONFIG.LED_MODE_CONFIG_segments.LED3_MODE = LED3_config.LED_config_segments.LED_mode;

    /* Update LED configurations. */

    LED_update();
}


uint16_t LED_map(uint16_t duty_in)
{
    uint32_t duty_out = 0;

    duty_out = (240 * duty_in + 100);

    return((uint16_t)duty_out);
}

uint16_t LED_accelerometer_percentage_value(uint8_t axis)
{
    _iq12 tempIQ12;
    uint16_t percentage;

    switch(MPU9250.configuration_A.configuration_A_segments.accelerometer_full_scale)
    {
        case(ACCEL_FS_2G):
        {
            tempIQ12 = _IQ12div( _IQ12(MPU9250.accel[axis]), _IQ12(20.0f) );
        }
        break;

        case(ACCEL_FS_4G):
        {
            tempIQ12 = _IQ12div( _IQ12(MPU9250.accel[axis]), _IQ12(40.0f) );
        }
        break;

        case(ACCEL_FS_8G):
        {
            tempIQ12 = _IQ12div( _IQ12(MPU9250.accel[axis]), _IQ12(80.0f) );
        }
        break;

        case(ACCEL_FS_16G):
        {
            tempIQ12 = _IQ12div( _IQ12(MPU9250.accel[axis]), _IQ12(160.0f) );
        }
        break;
    }

    percentage = (uint16_t)_IQ12toF(_IQ12abs(tempIQ12));

    return(percentage);
}


uint16_t LED_gyroscope_percentage_value(uint8_t axis)
{
    _iq12 tempIQ12;
    uint16_t percentage;

    /* Gyroscope readings are stored in DPS * 10 in the MPU9250 struct.
     * for percentage calculation normalized to mDPS, value is given by the equation.
     *
     * 100.0 * (MPU9250_DPS * 100 / GYRO_FS_mDPS) = 10000 * MPU9250_DPS / GYRO_FS_mDPS
     * = (10 * MPU9250_DPS) / GYRO_FS_DPS.
     *
     * */

    switch(MPU9250.configuration_A.configuration_A_segments.gyroscope_full_scale)
    {
        case(GYRO_FS_250DPS):
        {
            tempIQ12 = _IQ12div( _IQ12(MPU9250.gyro[axis]), _IQ12(25.0f) );
        }
        break;

        case(GYRO_FS_500DPS):
        {
            tempIQ12 = _IQ12div( _IQ12(MPU9250.gyro[axis]), _IQ12(50.0f) );
        }
        break;

        case(GYRO_FS_1000DPS):
        {
            tempIQ12 = _IQ12div( _IQ12(MPU9250.gyro[axis]), _IQ12(100.0f) );
        }
        break;

        case(GYRO_FS_2000DPS):
        {
            tempIQ12 = _IQ12div( _IQ12(MPU9250.gyro[axis]), _IQ12(200.0f) );
        }
        break;
    }

    percentage = (uint16_t)_IQ12toF(_IQ12abs(tempIQ12));

    return(percentage);
}


uint16_t LED_angle_percentage_value(uint8_t axis)
{
    _iq12 tempIQ12;

    if( axis == ROLL )
    {
        tempIQ12 = MP.roll_filter;
    }

    if( axis == PITCH )
    {
        tempIQ12 = MP.pitch_filter;
    }

    if( axis == YAW )
    {
        tempIQ12 = MP.yaw_filter;
    }

    tempIQ12 = _IQ12div( tempIQ12, _IQ12(1.8f) );
    tempIQ12 = _IQ12abs(tempIQ12);

    uint16_t percentage = (uint16_t)_IQ12toF(tempIQ12);
    percentage = percentage;

    return(percentage);
}


void LED_update()
{
  /* LED1 configuration update. */

  switch(LED1_config.LED_config_segments.LED_mode)
  {
      /* User controlled digital output: Output can be controlled via user by
       * issuing commands via serial interface. */

      case(LED_MODE_USER_CONTROLLED_DIGITAL_OUTPUT):
      {
          LED1_GPIO_MODE();
          LED1_SET_STATE(LED1_config.LED_config_segments.LED_state);
      }
      break;

      /* X accelerometer PWM output: PWM signal with duty proportional to
       * accelerometer value normalized to full-scale value is issued. */

      case(LED_MODE_X_ACCELEROMETER_PWM):
      {
          LED1_PWM_MODE();
          LED1_SET_DUTY( LED_accelerometer_percentage_value(X_AXIS) );
      }
      break;

      /* Y accelerometer PWM output: PWM signal with duty proportional to
       * accelerometer value normalized to full-scale value is issued. */

      case(LED_MODE_Y_ACCELEROMETER_PWM):
      {
          LED1_PWM_MODE();
          LED1_SET_DUTY( LED_accelerometer_percentage_value(Y_AXIS) );
      }
      break;

      /* Z accelerometer PWM output: PWM signal with duty proportional to
       * accelerometer value normalized to full-scale value is issued. */

      case(LED_MODE_Z_ACCELEROMETER_PWM):
      {
          LED1_PWM_MODE();
          LED1_SET_DUTY( LED_accelerometer_percentage_value(Z_AXIS) );
      }
      break;

      /* Accelerometer X threshold detection output. */

      case(LED_MODE_X_ACCELEROMETER_THRESHOLD):
      {
          LED1_GPIO_MODE();
          LED1_SET_STATE(MP.motion_detection.motion_detection_segments.acc_x);
      }
      break;

      /* Accelerometer Y threshold detection output. */

      case(LED_MODE_Y_ACCELEROMETER_THRESHOLD):
      {
          LED1_GPIO_MODE();
          LED1_SET_STATE(MP.motion_detection.motion_detection_segments.acc_y);
      }
      break;

      /* Accelerometer Z threshold detection output. */

      case(LED_MODE_Z_ACCELEROMETER_THRESHOLD):
      {
          LED1_GPIO_MODE();
          LED1_SET_STATE(MP.motion_detection.motion_detection_segments.acc_z);
      }
      break;

      /* X gyroscope PWM output: PWM signal with duty proportional to
       * gyroscope value normalized to full-scale value is issued. */

      case(LED_MODE_X_GYROSCOPE_PWM):
      {
          LED1_PWM_MODE();
          LED1_SET_DUTY( LED_gyroscope_percentage_value(X_AXIS) );
      }
      break;

      /* Y gyroscope PWM output: PWM signal with duty proportional to
       * gyroscope value normalized to full-scale value is issued. */

      case(LED_MODE_Y_GYROSCOPE_PWM):
      {
          LED1_PWM_MODE();
          LED1_SET_DUTY( LED_gyroscope_percentage_value(Y_AXIS) );
      }
      break;

      /* Z gyroscope PWM output: PWM signal with duty proportional to
       * gyroscope value normalized to full-scale value is issued. */

      case(LED_MODE_Z_GYROSCOPE_PWM):
      {
          LED1_PWM_MODE();
          LED1_SET_DUTY( LED_gyroscope_percentage_value(Z_AXIS) );
      }
      break;

      /* Gyroscope X threshold detection output. */

      case(LED_MODE_X_GYROSCOPE_THRESHOLD):
      {
          LED1_GPIO_MODE();
          LED1_SET_STATE(MP.motion_detection.motion_detection_segments.gyr_x);
      }
      break;

      /* Gyroscope Y threshold detection output. */

      case(LED_MODE_Y_GYROSCOPE_THRESHOLD):
      {
          LED1_GPIO_MODE();
          LED1_SET_STATE(MP.motion_detection.motion_detection_segments.gyr_y);
      }
      break;

      /* Gyroscope Z threshold detection output. */

      case(LED_MODE_Z_GYROSCOPE_THRESHOLD):
      {
          LED1_GPIO_MODE();
          LED1_SET_STATE(MP.motion_detection.motion_detection_segments.gyr_z);
      }
      break;

      /* Roll angle PWM value. */

      case(LED_MODE_ROLL_PWM):
      {
          LED1_PWM_MODE();
          LED1_SET_DUTY(LED_angle_percentage_value(ROLL));
      }
      break;

      /* Pitch angle PWM value. */

      case(LED_MODE_PITCH_PWM):
      {
          LED1_PWM_MODE();
          LED1_SET_DUTY(LED_angle_percentage_value(PITCH));
      }
      break;

      /* Yaw angle PWM value. */

      case(LED_MODE_YAW_PWM):
      {
          LED1_PWM_MODE();
          LED1_SET_DUTY(LED_angle_percentage_value(YAW));
      }
      break;

      /* Roll motion detection state output. */

      case(LED_MODE_ROLL_THRESHOLD):
      {
          LED1_GPIO_MODE();
          LED1_SET_STATE(MP.motion_detection.motion_detection_segments.roll);
      }
      break;

      /* Pitch motion detection state output. */

      case(LED_MODE_PITCH_THRESHOLD):
      {
          LED1_GPIO_MODE();
          LED1_SET_STATE(MP.motion_detection.motion_detection_segments.pitch);
      }
      break;

      /* Yaw motion detection state output. */

      case(LED_MODE_YAW_THRESHOLD):
      {
          LED1_GPIO_MODE();
          LED1_SET_STATE(MP.motion_detection.motion_detection_segments.yaw);
      }
      break;

      /* User controlled PWM output. */

      case(LED_MODE_USER_CONTROLLED_PWM):
      {
          LED1_PWM_MODE();
          LED1_SET_DUTY(LED1_config.LED_config_segments.LED_duty);
      }
      break;
  }



  /* LED2 configuration update. */

  switch(LED2_config.LED_config_segments.LED_mode)
  {
      /* User controlled digital output: Output can be controlled via user by
       * issuing commands via serial interface. */

      case(LED_MODE_USER_CONTROLLED_DIGITAL_OUTPUT):
      {
          LED2_GPIO_MODE();
          LED2_SET_STATE(LED2_config.LED_config_segments.LED_state);
      }
      break;

      /* X accelerometer PWM output: PWM signal with duty proportional to
       * accelerometer value normalized to full-scale value is issued. */

      case(LED_MODE_X_ACCELEROMETER_PWM):
      {
          LED2_PWM_MODE();
          LED2_SET_DUTY( LED_accelerometer_percentage_value(X_AXIS) );
      }
      break;

      /* Y accelerometer PWM output: PWM signal with duty proportional to
       * accelerometer value normalized to full-scale value is issued. */

      case(LED_MODE_Y_ACCELEROMETER_PWM):
      {
          LED2_PWM_MODE();
          LED2_SET_DUTY( LED_accelerometer_percentage_value(Y_AXIS) );
      }
      break;

      /* Z accelerometer PWM output: PWM signal with duty proportional to
       * accelerometer value normalized to full-scale value is issued. */

      case(LED_MODE_Z_ACCELEROMETER_PWM):
      {
          LED2_PWM_MODE();
          LED2_SET_DUTY( LED_accelerometer_percentage_value(Z_AXIS) );
      }
      break;

      /* Accelerometer X threshold detection output. */

      case(LED_MODE_X_ACCELEROMETER_THRESHOLD):
      {
          LED2_GPIO_MODE();
          LED2_SET_STATE(MP.motion_detection.motion_detection_segments.acc_x);
      }
      break;

      /* Accelerometer Y threshold detection output. */

      case(LED_MODE_Y_ACCELEROMETER_THRESHOLD):
      {
          LED2_GPIO_MODE();
          LED2_SET_STATE(MP.motion_detection.motion_detection_segments.acc_y);
      }
      break;

      /* Accelerometer Z threshold detection output. */

      case(LED_MODE_Z_ACCELEROMETER_THRESHOLD):
      {
          LED2_GPIO_MODE();
          LED2_SET_STATE(MP.motion_detection.motion_detection_segments.acc_z);
      }
      break;

      /* X gyroscope PWM output: PWM signal with duty proportional to
       * gyroscope value normalized to full-scale value is issued. */

      case(LED_MODE_X_GYROSCOPE_PWM):
      {
          LED2_PWM_MODE();
          LED2_SET_DUTY( LED_gyroscope_percentage_value(X_AXIS) );
      }
      break;

      /* Y gyroscope PWM output: PWM signal with duty proportional to
       * gyroscope value normalized to full-scale value is issued. */

      case(LED_MODE_Y_GYROSCOPE_PWM):
      {
          LED2_PWM_MODE();
          LED2_SET_DUTY( LED_gyroscope_percentage_value(Y_AXIS) );
      }
      break;

      /* Z gyroscope PWM output: PWM signal with duty proportional to
       * gyroscope value normalized to full-scale value is issued. */

      case(LED_MODE_Z_GYROSCOPE_PWM):
      {
          LED2_PWM_MODE();
          LED2_SET_DUTY( LED_gyroscope_percentage_value(Z_AXIS) );
      }
      break;

      /* Gyroscope X threshold detection output. */

      case(LED_MODE_X_GYROSCOPE_THRESHOLD):
      {
          LED2_GPIO_MODE();
          LED2_SET_STATE(MP.motion_detection.motion_detection_segments.gyr_x);
      }
      break;

      /* Gyroscope Y threshold detection output. */

      case(LED_MODE_Y_GYROSCOPE_THRESHOLD):
      {
          LED2_GPIO_MODE();
          LED2_SET_STATE(MP.motion_detection.motion_detection_segments.gyr_y);
      }
      break;

      /* Gyroscope Z threshold detection output. */

      case(LED_MODE_Z_GYROSCOPE_THRESHOLD):
      {
          LED2_GPIO_MODE();
          LED2_SET_STATE(MP.motion_detection.motion_detection_segments.gyr_z);
      }
      break;

      /* Roll angle PWM value. */

      case(LED_MODE_ROLL_PWM):
      {
          LED2_PWM_MODE();
          LED2_SET_DUTY(LED_angle_percentage_value(ROLL));
      }
      break;

      /* Pitch angle PWM value. */

      case(LED_MODE_PITCH_PWM):
      {
          LED2_PWM_MODE();
          LED2_SET_DUTY(LED_angle_percentage_value(PITCH));
      }
      break;

      /* Yaw angle PWM value. */

      case(LED_MODE_YAW_PWM):
      {
          LED2_PWM_MODE();
          LED2_SET_DUTY(LED_angle_percentage_value(YAW));
      }
      break;

      /* Roll motion detection state output. */

      case(LED_MODE_ROLL_THRESHOLD):
      {
          LED2_GPIO_MODE();
          LED2_SET_STATE(MP.motion_detection.motion_detection_segments.roll);
      }
      break;

      /* Pitch motion detection state output. */

      case(LED_MODE_PITCH_THRESHOLD):
      {
          LED2_GPIO_MODE();
          LED2_SET_STATE(MP.motion_detection.motion_detection_segments.pitch);
      }
      break;

      /* Yaw motion detection state output. */

      case(LED_MODE_YAW_THRESHOLD):
      {
          LED2_GPIO_MODE();
          LED2_SET_STATE(MP.motion_detection.motion_detection_segments.yaw);
      }
      break;

      /* User controlled PWM output. */

      case(LED_MODE_USER_CONTROLLED_PWM):
      {
          LED2_PWM_MODE();
          LED2_SET_DUTY(LED2_config.LED_config_segments.LED_duty);
      }
      break;
  }




  /* LED3 configuration update. */

  switch(LED3_config.LED_config_segments.LED_mode)
  {
      /* User controlled digital output: Output can be controlled via user by
       * issuing commands via serial interface. */

      case(LED_MODE_USER_CONTROLLED_DIGITAL_OUTPUT):
      {
          LED3_GPIO_MODE();
          LED3_SET_STATE(LED3_config.LED_config_segments.LED_state);
      }
      break;

      /* X accelerometer PWM output: PWM signal with duty proportional to
       * accelerometer value normalized to full-scale value is issued. */

      case(LED_MODE_X_ACCELEROMETER_PWM):
      {
          LED3_PWM_MODE();
          LED3_SET_DUTY( LED_accelerometer_percentage_value(X_AXIS) );
      }
      break;

      /* Y accelerometer PWM output: PWM signal with duty proportional to
       * accelerometer value normalized to full-scale value is issued. */

      case(LED_MODE_Y_ACCELEROMETER_PWM):
      {
          LED3_PWM_MODE();
          LED3_SET_DUTY( LED_accelerometer_percentage_value(Y_AXIS) );
      }
      break;

      /* Z accelerometer PWM output: PWM signal with duty proportional to
       * accelerometer value normalized to full-scale value is issued. */

      case(LED_MODE_Z_ACCELEROMETER_PWM):
      {
          LED3_PWM_MODE();
          LED3_SET_DUTY( LED_accelerometer_percentage_value(Z_AXIS) );
      }
      break;

      /* Accelerometer X threshold detection output. */

      case(LED_MODE_X_ACCELEROMETER_THRESHOLD):
      {
          LED3_GPIO_MODE();
          LED3_SET_STATE(MP.motion_detection.motion_detection_segments.acc_x);
      }
      break;

      /* Accelerometer Y threshold detection output. */

      case(LED_MODE_Y_ACCELEROMETER_THRESHOLD):
      {
          LED3_GPIO_MODE();
          LED3_SET_STATE(MP.motion_detection.motion_detection_segments.acc_y);
      }
      break;

      /* Accelerometer Z threshold detection output. */

      case(LED_MODE_Z_ACCELEROMETER_THRESHOLD):
      {
          LED3_GPIO_MODE();
          LED3_SET_STATE(MP.motion_detection.motion_detection_segments.acc_z);
      }
      break;

      /* X gyroscope PWM output: PWM signal with duty proportional to
       * gyroscope value normalized to full-scale value is issued. */

      case(LED_MODE_X_GYROSCOPE_PWM):
      {
          LED3_PWM_MODE();
          LED3_SET_DUTY( LED_gyroscope_percentage_value(X_AXIS) );
      }
      break;

      /* Y gyroscope PWM output: PWM signal with duty proportional to
       * gyroscope value normalized to full-scale value is issued. */

      case(LED_MODE_Y_GYROSCOPE_PWM):
      {
          LED3_PWM_MODE();
          LED3_SET_DUTY( LED_gyroscope_percentage_value(Y_AXIS) );
      }
      break;

      /* Z gyroscope PWM output: PWM signal with duty proportional to
       * gyroscope value normalized to full-scale value is issued. */

      case(LED_MODE_Z_GYROSCOPE_PWM):
      {
          LED3_PWM_MODE();
          LED3_SET_DUTY( LED_gyroscope_percentage_value(Z_AXIS) );
      }
      break;

      /* Gyroscope X threshold detection output. */

      case(LED_MODE_X_GYROSCOPE_THRESHOLD):
      {
          LED3_GPIO_MODE();
          LED3_SET_STATE(MP.motion_detection.motion_detection_segments.gyr_x);
      }
      break;

      /* Gyroscope Y threshold detection output. */

      case(LED_MODE_Y_GYROSCOPE_THRESHOLD):
      {
          LED3_GPIO_MODE();
          LED3_SET_STATE(MP.motion_detection.motion_detection_segments.gyr_y);
      }
      break;

      /* Gyroscope Z threshold detection output. */

      case(LED_MODE_Z_GYROSCOPE_THRESHOLD):
      {
          LED3_GPIO_MODE();
          LED3_SET_STATE(MP.motion_detection.motion_detection_segments.gyr_z);
      }
      break;

      /* Roll angle PWM value. */

      case(LED_MODE_ROLL_PWM):
      {
          LED3_PWM_MODE();
          LED3_SET_DUTY(LED_angle_percentage_value(ROLL));
      }
      break;

      /* Pitch angle PWM value. */

      case(LED_MODE_PITCH_PWM):
      {
          LED3_PWM_MODE();
          LED3_SET_DUTY(LED_angle_percentage_value(PITCH));
      }
      break;

      /* Yaw angle PWM value. */

      case(LED_MODE_YAW_PWM):
      {
          LED3_PWM_MODE();
          LED3_SET_DUTY(LED_angle_percentage_value(YAW));
      }
      break;

      /* Roll motion detection state output. */

      case(LED_MODE_ROLL_THRESHOLD):
      {
          LED3_GPIO_MODE();
          LED3_SET_STATE(MP.motion_detection.motion_detection_segments.roll);
      }
      break;

      /* Pitch motion detection state output. */

      case(LED_MODE_PITCH_THRESHOLD):
      {
          LED3_GPIO_MODE();
          LED3_SET_STATE(MP.motion_detection.motion_detection_segments.pitch);
      }
      break;

      /* Yaw motion detection state output. */

      case(LED_MODE_YAW_THRESHOLD):
      {
          LED3_GPIO_MODE();
          LED3_SET_STATE(MP.motion_detection.motion_detection_segments.yaw);
      }
      break;

      /* User controlled PWM output. */

      case(LED_MODE_USER_CONTROLLED_PWM):
      {
          LED3_PWM_MODE();
          LED3_SET_DUTY(LED3_config.LED_config_segments.LED_duty);
      }
      break;
  }

  /* Update communication frames. */

  LED_OUTPUT.LED_OUTPUT_segments.LED1_OUTPUT_DUTY   =   LED1_config.LED_config_segments.LED_duty;
  LED_OUTPUT.LED_OUTPUT_segments.LED1_OUTPUT_STATE  =   LED1_config.LED_config_segments.LED_state;
  LED_OUTPUT.LED_OUTPUT_segments.LED2_OUTPUT_DUTY   =   LED2_config.LED_config_segments.LED_duty;
  LED_OUTPUT.LED_OUTPUT_segments.LED2_OUTPUT_STATE  =   LED2_config.LED_config_segments.LED_state;
  LED_OUTPUT.LED_OUTPUT_segments.LED3_OUTPUT_DUTY   =   LED3_config.LED_config_segments.LED_duty;
  LED_OUTPUT.LED_OUTPUT_segments.LED3_OUTPUT_STATE  =   LED3_config.LED_config_segments.LED_state;
}
