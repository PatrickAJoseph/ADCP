
#include <cstdint>
#include <iostream>
#include "../../ADCP.hpp"

class MPU_Stick
{
	private:
		ADCP _adcp;					//	ADCP instance.
		uint32_t retry_interval;	//	Retry interval for operations in milli-seconds.
		uint32_t number_of_tries;	//	Number of tries for operations.
	
	public:
	
		/* Typedefs. */
		
		typedef enum _IMU_Accelerometer_FS	//	Accelerometer full-scale values.
		{
			ACCEL_FS_2G = 0,
			ACCEL_FS_4G = 1,
			ACCEL_FS_8G = 2,
			ACCEL_FS_16G = 3
		}IMU_Accel_FS;
	
		typedef enum _IMU_Accelerometer_BW	//	Accelerometer bandwidth.
		{
			ACCEL_BW_460_Hz = 0,
			ACCEL_BW_184_Hz = 1,
			ACCEL_BW_92_Hz = 2,
			ACCEL_BW_41_Hz = 3,
			ACCEL_BW_20_Hz = 4,
			ACCEL_BW_10_Hz = 5,
			ACCEL_BW_5_Hz = 6
		}IMU_Accel_BW;
	
		typedef enum _IMU_Gyroscope_FS		//	Gyroscope full-scale values.
		{
			GYRO_FS_250DPS = 0,
			GYRO_FS_500DPS = 1,
			GYRO_FS_1000DPS = 2,
			GYRO_FS_2000DPS = 3
		}IMU_Gyro_FS;
		
		typedef enum _IMU_Gyroscope_BW		//	Gyroscope bandwidth.
		{
			GYRO_BW_NONE = 0,
			GYRO_BW_184_Hz = 1,
			GYRO_BW_92_Hz = 2,
			GYRO_BW_41_Hz = 3,
			GYRO_BW_20_Hz = 4,
			GYRO_BW_10_Hz = 5,
			GYRO_BW_5_Hz = 6			
		}IMU_Gyro_BW;
		
		typedef enum _Motion_Detection_Threshold_Type	//	Motion detection threshold type.
		{
			ABS_LESSER_THAN = 0,
			ABS_GREATER_THAN = 1,
			LESSER_THAN = 2,
			GREATER_THAN = 3,
			WITHIN_BOUNDS = 4,
			OUTSIDE_BOUNDS = 5
		}MD_Threshold_Type;
	
		typedef enum _LED_operation_mode		//	LED configuration mode.
		{
			UC_DIGITAL_OUTPUT = 0,				//	User controller digital output.
			ACCEL_X_PWM = 1,					//	Accelerometer X PWM output.
			ACCEL_Y_PWM = 2,					//	Accelerometer Y PWM output.
			ACCEL_Z_PWM = 3,					//	Accelerometer Z PWM output.
			ACCEL_X_MD_OUTPUT = 4,				//	Accelerometer X motion-detection output.
			ACCEL_Y_MD_OUTPUT = 5,				//	Accelerometer Y motion-detection output.
			ACCEL_Z_MD_OUTPUT = 6,				//	Accelerometer Z motion-detection output.
			GYRO_X_PWM = 7,						//	Gyroscope X PWM output.
			GYRO_Y_PWM = 8,						//	Gyroscope Y PWM output.
			GYRO_Z_PWM = 9,						//	Gyroscope Z PWM output.
			GYRO_X_MD_OUTPUT = 10,				//	Gyroscope X motion-detection output.
			GYRO_Y_MD_OUTPUT = 11,				//	Gyroscope Y motion-detection output.
			GYRO_Z_MD_OUTPUT = 12,				//	Gyroscope Z motion-detection output.
			ROLL_PWM = 13,						//	Roll PWM output.
			PITCH_PWM = 14,						//	Pitch PWM output.
			YAW_PWM = 15,						//	Yaw PWM output.
			ROLL_MD_OUTPUT = 16,				//	Roll motion-detection output.
			PITCH_MD_OUTPUT = 17,				//	Pitch motion-detection output.
			YAW_MD_OUTPUT = 18					//	Yaw motion-detection output.
		}LED_mode;
		
		typedef enum _configuration_parameter
		{
			SAMPLING_RATE = 0,
			CALIBRATION_POINTS = 1,
			CALIBRATION_INTERVAL = 2,
			IMU_ACCEL_FS = 3,
			IMU_ACCEL_BW = 4,
			IMU_GYRO_FS = 5,
			IMU_GYRO_BW = 6,
			X_ACCEL_THRESHOLD_TYPE = 7,
			Y_ACCEL_THRESHOLD_TYPE = 8,
			Z_ACCEL_THRESHOLD_TYPE = 9,
			X_GYRO_THRESHOLD_TYPE = 10,
			Y_GYRO_THRESHOLD_TYPE = 11,
			Z_GYRO_THRESHOLD_TYPE = 12,
			ROLL_THRESHOLD_TYPE = 13,
			PITCH_THRESHOLD_TYPE = 14,
			YAW_THRESHOLD_TYPE = 15,
			X_ACCEL_THRESHOLD_LOW = 16,
			X_ACCEL_THRESHOLD_HIGH = 17,
			Y_ACCEL_THRESHOLD_LOW = 18,
			Y_ACCEL_THRESHOLD_HIGH = 19,
			Z_ACCEL_THRESHOLD_LOW = 20,
			Z_ACCEL_THRESHOLD_HIGH = 21,
			X_GYRO_THRESHOLD_LOW = 22,
			X_GYRO_THRESHOLD_HIGH = 23,
			Y_GYRO_THRESHOLD_LOW = 24,
			Y_GYRO_THRESHOLD_HIGH = 25,
			Z_GYRO_THRESHOLD_LOW = 26,
			Z_GYRO_THRESHOLD_HIGH = 27,
			ROLL_THRESHOLD_LOW = 28,
			ROLL_THRESHOLD_HIGH = 29,
			PITCH_THRESHOLD_LOW = 30,
			PITCH_THRESHOLD_HIGH = 31,
			YAW_THRESHOLD_LOW = 32,
			YAW_THRESHOLD_HIGH = 33,
			ROLL_FILTER_CONSANT = 34,
			PITCH_FILTER_CONSTANT = 35,
			LED_1_MODE = 36,
			LED_2_MODE = 37,
			LED_3_MODE = 38
			
		}Configuration_Parameter;
		
		/* Configuration structure. */
		
		struct _configurations
		{
			/* IMU configurations. */
			
			uint16_t sampling_rate;									//	Sampling rate in Hz.
			uint16_t calibration_points;							//	Number of points for calibration.
			uint16_t calibration_interval;							//	Duration between consecutive samples in milli-seconds.
			
			IMU_Accel_FS 			imu_accel_fs;					//	Accelerometer full-scale value.
			IMU_Accel_BW 			imu_accel_bw;					//	Accelerometer bandwidth.
			IMU_Gyro_FS				imu_gyro_fs;					//	Gyroscope full-scale value.
			IMU_Gyro_BW				imu_gyro_bw;					//	Gyroscope bandwidth.
			
			/* Motion detection configurations. */
			
			MD_Threshold_Type		x_accel_threshold_type;		//	Accelerometer X threshold type.
			MD_Threshold_Type		y_accel_threshold_type;		//	Accelerometer Y threshold type.
			MD_Threshold_Type       z_accel_threshold_type;		//	Accelerometer Z threshold type.
			MD_Threshold_Type		x_gyro_threshold_type;		//	Gyroscope X threshold type.
			MD_Threshold_Type		y_gyro_threshold_type;		//	Gyroscope Y threshold type.
			MD_Threshold_Type		z_gyro_threshold_type;		//	Gyroscope Z threshold type.
			MD_Threshold_Type		roll_threshold_type;		//	Roll threshold type.
			MD_Threshold_Type		pitch_threshold_type;		//	Pitch threshold type.
			MD_Threshold_Type		yaw_threshold_type;			//	Yaw threshold type.

			
			double 	x_accel_threshold_low;
			double 	x_accel_threshold_high;
			double  y_accel_threshold_low;
			double 	y_accel_threshold_high;
			double  z_accel_threshold_low;
			double  z_accel_threshold_high;
			double  x_gyro_threshold_low;
			double	x_gyro_threshold_high;
			double  y_gyro_threshold_low;
			double  y_gyro_threshold_high;
			double  z_gyro_threshold_low;
			double  z_gyro_threshold_high;
			double  roll_threshold_low;
			double  roll_threshold_high;
			double  pitch_threshold_low;
			double  pitch_threshold_high;
			double  yaw_threshold_low;
			double  yaw_threshold_high;
			
			/* Complementary filter constants. */
			
			double  roll_filter_constant;
			double  pitch_filter_constant;
			
			/* LED mode configurations. */
			
			LED_mode LED1_mode;
			LED_mode LED2_mode;
			LED_mode LED3_mode;
			
		}configurations;
	
	public:

		/*
		*	@brief:						Constructor for MPU Stick
		*	@parameter port:			COM Port of the device.
		*	@parameter baudRate:		Baud rate for serial communication.
		*	
		*/

		MPU_Stick(string port,uint32_t baudRate)
		{
			_adcp.read_file("MPU_Stick.csv");			//	Load symbol file.
			_adcp.serial_port_init(port,baudRate);		//	Open serial port.
				
			retry_interval = 250;
			number_of_tries = 10;
				
			_adcp.reset_node( _adcp.get_node_id("MPU_Stick"), retry_interval, number_of_tries );		//	Reset connection with node.
		}

		/*
		*	@brief:						Descructor for MPU Stick.
		*
		*/

		~MPU_Stick()
		{
			_adcp.reset_node( _adcp.get_node_id("MPU_Stick"), retry_interval, number_of_tries );		//	Reset connection with node.
			_adcp.serial_port_flush(NULL);																	//	Flush serial port.
			_adcp.serial_port_close();																	//	Close serial port.
		}

		/*
		*	@brief:						Enable transmission of packets from device. 
		*
		*/
		
		void enable_transmission();
		
		/*
		*	@brief:						Disable transmission of packets from device. 
		*
		*/
		
		void disable_transmission();
		
		/*
		*	@brief:									Read target configuration parameter from device.
		*	@parameter configuration_parameter:		Configuration parameter to be read.
		*/
		
		void read_configuration(Configuration_Parameter configuration_parameter);
};