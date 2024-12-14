
#include <iostream>

#include "MPU_Stick.hpp"

/*
*	@brief:		Enable connection to device.
*/

void MPU_Stick::enable_transmission()
{
	this->_adcp.open_node_connection( this->_adcp.get_node_id("MPU_Stick"), 
									  this->retry_interval, 
									  this->number_of_tries );
}

/*
*	@brief:		Disable connection to device.
*/

void MPU_Stick::disable_transmission()
{
	this->_adcp.close_node_connection( this->_adcp.get_node_id("MPU_Stick"), 
									  this->retry_interval, 
									  this->number_of_tries );
}

/*
*	@brief:									Read target configuration parameter from device.
*	@parameter configuration_parameter:		Configuration parameter to be read.
*/
		
void MPU_Stick::read_configuration(MPU_Stick::Configuration_Parameter configuration_parameter)
{
	uint8_t node_id = this->_adcp.get_node_id("MPU_Stick");

	vector<string> request_frames;

	switch(configuration_parameter)
	{
		case(MPU_Stick::Configuration_Parameter::SAMPLING_RATE):
		{
			request_frames.push_back("MPU_STICK::IMU_CONFIG_REG_A");
			
			this->_adcp.remote_frame_request(node_id, request_frames,this->retry_interval, this->number_of_tries);
			
			this->configurations.sampling_rate = 
			(uint16_t)this->_adcp.read_parameter("MPU_STICK::IMU_CONFIG_REG_A::IMU_CONFIG_SAMPLING_RATE");
		}
		break;

		case(MPU_Stick::Configuration_Parameter::CALIBRATION_INTERVAL):
		{
			request_frames.push_back("MPU_STICK::IMU_CONFIG_REG_A");
			
			this->_adcp.remote_frame_request(node_id, request_frames,this->retry_interval, this->number_of_tries);

			this->configurations.calibration_interval = 
			(uint16_t)this->_adcp.read_parameter("MPU_STICK::IMU_CONFIG_REG_A::IMU_CONFIG_CALIB_INTERVAL");
		}
		break;

		case(MPU_Stick::Configuration_Parameter::CALIBRATION_POINTS):
		{
			request_frames.push_back("MPU_STICK::IMU_CONFIG_REG_A");
			
			this->_adcp.remote_frame_request(node_id, request_frames,this->retry_interval, this->number_of_tries);

			this->configurations.calibration_points = 
			(uint16_t)this->_adcp.read_parameter("MPU_STICK::IMU_CONFIG_REG_A::IMU_CONFIG_CALIB_POINTS");
		}
		break;

		case(MPU_Stick::Configuration_Parameter::IMU_ACCEL_FS):
		{
			request_frames.push_back("MPU_STICK::IMU_CONFIG_REG_B");
			
			this->_adcp.remote_frame_request(node_id, request_frames,this->retry_interval, this->number_of_tries);

			this->configurations.imu_accel_fs = 
			(MPU_Stick::IMU_Accel_FS)((uint16_t)this->_adcp.read_parameter("MPU_STICK::IMU_CONFIG_REG_B::IMU_ACCEL_FS"));
		}
		break;

		case(MPU_Stick::Configuration_Parameter::IMU_GYRO_FS):
		{
			request_frames.push_back("MPU_STICK::IMU_CONFIG_REG_B");
			
			this->_adcp.remote_frame_request(node_id, request_frames,this->retry_interval, this->number_of_tries);

			this->configurations.imu_gyro_fs = 
			(MPU_Stick::IMU_Gyro_FS)((uint16_t)this->_adcp.read_parameter("MPU_STICK::IMU_CONFIG_REG_B::IMU_GYRO_FS"));
		}
		break;

		case(MPU_Stick::Configuration_Parameter::IMU_ACCEL_BW):
		{
			request_frames.push_back("MPU_STICK::IMU_CONFIG_REG_B");
			
			this->_adcp.remote_frame_request(node_id, request_frames,this->retry_interval, this->number_of_tries);

			this->configurations.imu_accel_bw = 
			(MPU_Stick::IMU_Accel_BW)((uint16_t)this->_adcp.read_parameter("MPU_STICK::IMU_CONFIG_REG_B::IMU_ACCEL_BW"));
		}
		break;

		case(MPU_Stick::Configuration_Parameter::IMU_GYRO_BW):
		{
			request_frames.push_back("MPU_STICK::IMU_CONFIG_REG_B");
			
			this->_adcp.remote_frame_request(node_id, request_frames,this->retry_interval, this->number_of_tries);

			this->configurations.imu_gyro_bw = 
			(MPU_Stick::IMU_Gyro_BW)((uint16_t)this->_adcp.read_parameter("MPU_STICK::IMU_CONFIG_REG_B::IMU_GYRO_BW"));
		}
		break;



		default:
		{
			
		}
	}		
}
