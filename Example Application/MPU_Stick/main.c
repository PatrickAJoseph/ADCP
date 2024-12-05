
#include <msp430.h>
#include "system.h"
#include <stdint.h>
#include <stdbool.h>

#define WRITE_CONFIGURATION     false


int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;
	
	System_Clock_Init();
	SPI_GPIO_Init();
	UART_GPIO_Init();
	I2C_GPIO_Init();

	/* Configure P1.0 LED to output healthy signal. */

	P1DIR |= BIT0;
	P1OUT &= ~BIT0;

	P1DIR |= BIT1;

    /* Enable pull-up resistors at P2.0 and configure it as an input. */

    P2OUT |= BIT0;
    P2REN |= BIT0;
    P2DIR &= ~BIT0;         // Configure P2.0 direction as input.

    /* Enable interrupt on P2.0 */

    P2IFG &= ~BIT0;             //  Clear interrupt flag on P2.0.
    P2IES &= ~BIT0;             //  Interrupt on rising edge transition.
    //P2IE |= BIT0;               //  Enable interrupt at P2.0.

	PM5CTL0 &= ~LOCKLPM5;

	I2C_Init(100000);

	if(WRITE_CONFIGURATION)
	{
        MPU9250.calibration_interval_ms = 250;
        MPU9250.calibration_point_count = 100;
        MPU9250.sampling_rate = 100;
        MPU9250.configuration_A.configuration_A_segments.accelerometer_filter_bandwidth = ACCEL_DLPF_BW_184Hz;
        MPU9250.configuration_A.configuration_A_segments.accelerometer_full_scale = ACCEL_FS_8G;
        MPU9250.configuration_A.configuration_A_segments.filter_enable = 1;
        MPU9250.configuration_A.configuration_A_segments.gyroscope_filter_bandwidth = GYRO_DLPF_BW_184Hz;
        MPU9250.configuration_A.configuration_A_segments.gyroscope_full_scale = GYRO_FS_1000DPS;

        MP.accelerometer_low_threshold[X_AXIS]  =   _IQ12(1500.0f);
        MP.accelerometer_low_threshold[Y_AXIS]  =   _IQ12(1000.0f);
        MP.accelerometer_low_threshold[Z_AXIS]  =   _IQ12(1000.0f);
        MP.gyroscope_low_threshold[X_AXIS]      =   _IQ12(100.0f);
        MP.gyroscope_low_threshold[Y_AXIS]      =   _IQ12(250.0f);
        MP.gyroscope_low_threshold[Z_AXIS]      =   _IQ12(500.0f);
        MP.angle_low_threshold[ROLL]            =   _IQ12(-10.0f);
        MP.angle_low_threshold[PITCH]           =   _IQ12(-15.0f);
        MP.angle_low_threshold[YAW]             =   _IQ12(-25.0f);

        MP.accelerometer_high_threshold[X_AXIS]  =   _IQ12(1500.0f);
        MP.accelerometer_high_threshold[Y_AXIS]  =   _IQ12(1000.0f);
        MP.accelerometer_high_threshold[Z_AXIS]  =   _IQ12(1000.0f);
        MP.gyroscope_high_threshold[X_AXIS]      =   _IQ12(100.0f);
        MP.gyroscope_high_threshold[Y_AXIS]      =   _IQ12(250.0f);
        MP.gyroscope_high_threshold[Z_AXIS]      =   _IQ12(500.0f);
        MP.angle_high_threshold[ROLL]            =   _IQ12(10.0f);
        MP.angle_high_threshold[PITCH]           =   _IQ12(15.0f);
        MP.angle_high_threshold[YAW]             =   _IQ12(25.0f);

        MP.motion_detection.motion_detection_segments.acc_x_threshold_type =
        MOTION_DETECTION_THRESHOLD_TYPE_ABSOLUTE_LESSER_THAN;
        //MP.accelerometer_low_threshold[X_AXIS] = _IQ12(1000.0f);
        MP.motion_detection.motion_detection_segments.acc_y_threshold_type =
        MOTION_DETECTION_THRESHOLD_TYPE_ABSOLUTE_LESSER_THAN;
        //MP.accelerometer_low_threshold[Y_AXIS] = _IQ12(1000.0f);
        MP.motion_detection.motion_detection_segments.acc_z_threshold_type =
        MOTION_DETECTION_THRESHOLD_TYPE_LESSER_THAN;
        //MP.accelerometer_low_threshold[Z_AXIS] = _IQ12(500.0f);

        MP.angle_high_threshold[ROLL]    =   _IQ12(10.0f);
        MP.angle_low_threshold[ROLL]   =   _IQ12(-20.0f);
        MP.motion_detection.motion_detection_segments.roll_threshold_type =
        MOTION_DETECTION_THRESHOLD_TYPE_WINDOWED_MODE_WITHIN_BOUNDS;

        MP.angle_high_threshold[PITCH]    =   _IQ12(10.0f);
        MP.angle_low_threshold[PITCH]   =   _IQ12(-20.0f);
        MP.motion_detection.motion_detection_segments.pitch_threshold_type =
        MOTION_DETECTION_THRESHOLD_TYPE_WINDOWED_MODE_OUTSIDE_BOUNDS;

        MP.angle_high_threshold[YAW]    =   _IQ12(10.0f);
        MP.angle_low_threshold[YAW]   =   _IQ12(5.0f);
        MP.motion_detection.motion_detection_segments.yaw_threshold_type =
        MOTION_DETECTION_THRESHOLD_TYPE_ABSOLUTE_GREATER_THAN;


        LED1_config.LED_config_segments.LED_mode = LED_MODE_ROLL_PWM;
        LED2_config.LED_config_segments.LED_mode = LED_MODE_PITCH_PWM;
        LED3_config.LED_config_segments.LED_mode = LED_MODE_YAW_PWM;

        MP.roll_filter_constant = _IQ12(0.25f);
        MP.pitch_filter_constant = _IQ12(0.15f);

        LED_update();

        config_write();
	}

	config_read();

	__delay_cycles(80000);


	SPI_Init();
	LED_Init();
	UART_Init(38400);
	MPU9250_Init();
	MPU9250_configure();
	MPU9250_calibrate();

	ADCP_Transmit_Handle = &UART_Transmit;
	ADCP_Init();
	MP_Init();
	Tick_Init();
	Task_Manager_Init();

    P2IE |= BIT0;               //  Enable interrupt at P2.0.


	MP.step_time = _IQ12(1.0f/(float)MPU9250.sampling_rate);

    __bic_SR_register(GIE);     //  Disable global interrupt.

    __delay_cycles(800);

    /* 0 represents highest priority.
     * 3 represents lowest priority. */

    ICCILSR0 = 0xFFFF;          //  Interrupt vectors 0 - 7.
    ICCILSR1 = 0xFF3F | ( 0 << 6 );          //  Interrupt vectors 8 - 15.
    ICCILSR2 = 0xFFFF;          //  Interrupt vectors 16 - 23.
    ICCILSR3 = 0xFFFF;          //  Priority level of interrupt vector 24 is set to zero.

    /* Priority of USCIA0: Highest. */

    //ICCILSR3 &=~ ( 3 << ( (33 - 24) << 1 ));
    //ICCILSR3 |=  ( 0 << ( (33 - 24) << 1 ));

    ICCSC |= ICCEN;             //  Enable Interrupt Compare Controller module.


    //P2IE &= ~BIT0;
    //TB0CCTL0 &= ~CCIE;


    __bis_SR_register(GIE);     //  Enable global interrupt.

    __delay_cycles(80000);

	while(1)
	{
	    Task_Manager_Execute_Tasks();

	    //ADCP_Receive_Handler();
	    //ADCP_Transmit_Handler();
	    //delay_ms(100);

	    //UART_puti( 1000.0f * _IQ12toF(MP.roll_filter) ); UART_putb(',');
	    //UART_puti( 1000.0f * _IQ12toF(MP.pitch_filter) ); UART_putb(',');
	    //UART_puti( 1000.0f * _IQ12toF(MP.yaw_filter) );
	    //UART_newline();
	    //delay_ms(100);
	}

	return 0;
}


#pragma vector = PORT2_VECTOR

__interrupt void Port_2_ISR()
{
  P2IFG &= ~BIT0;               //  Clear P2.0 interrupt flag.
  P1OUT ^= BIT0;                //  Set P1.0 (LED1)
  //P1OUT |= BIT1;

  MPU9250_read_sensor_data();
  MP_update();
  LED_update();

  MPU9250.configuration_A.configuration_A_segments.data_available = 1;

  //P1OUT &= ~BIT1;
}
