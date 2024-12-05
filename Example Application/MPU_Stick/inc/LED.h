
#ifndef INC_LED_H_
#define INC_LED_H_


#include <msp430.h>
#include <stdint.h>

#include "motion_processing.h"
#include "ADCP_frames.h"

#define LED_MODE_USER_CONTROLLED_DIGITAL_OUTPUT 0
#define LED_MODE_X_ACCELEROMETER_PWM            1
#define LED_MODE_Y_ACCELEROMETER_PWM            2
#define LED_MODE_Z_ACCELEROMETER_PWM            3
#define LED_MODE_X_ACCELEROMETER_THRESHOLD      4
#define LED_MODE_Y_ACCELEROMETER_THRESHOLD      5
#define LED_MODE_Z_ACCELEROMETER_THRESHOLD      6
#define LED_MODE_X_GYROSCOPE_PWM                7
#define LED_MODE_Y_GYROSCOPE_PWM                8
#define LED_MODE_Z_GYROSCOPE_PWM                9
#define LED_MODE_X_GYROSCOPE_THRESHOLD          10
#define LED_MODE_Y_GYROSCOPE_THRESHOLD          11
#define LED_MODE_Z_GYROSCOPE_THRESHOLD          12
#define LED_MODE_ROLL_PWM                       13
#define LED_MODE_PITCH_PWM                      14
#define LED_MODE_YAW_PWM                        15
#define LED_MODE_ROLL_THRESHOLD                 16
#define LED_MODE_PITCH_THRESHOLD                17
#define LED_MODE_YAW_THRESHOLD                  18
#define LED_MODE_USER_CONTROLLED_PWM            19

typedef union LED_config
{
  uint16_t all;

  struct LED_config_segments
  {
      uint16_t  LED_mode    :   5;
      uint16_t  LED_state   :   1;
      uint16_t  LED_duty    :   10;
  }LED_config_segments;

}LED_config;


extern LED_config LED1_config;
extern LED_config LED2_config;
extern LED_config LED3_config;

void LED_Init();
void LED_update();
uint16_t LED_map(uint16_t value);


#endif /* INC_LED_H_ */
