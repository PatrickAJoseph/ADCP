
#include "ADCP.hpp"

#include <chrono>
#include <thread>

int main()
{ 
  ADCP s;

  s.read_file("test_param.csv");

  
  s.serial_port_scan();
  s.serial_port_init("COM17",CBR_38400);  
  //s.serial_port_flush(NULL);
 
  Sleep(500);	

  uint8_t counter = 0;
  
  s.reset_node(0,1000,30);	
  
  Sleep(500);
  
  s.open_node_connection(0,1000,30);	

  Sleep(250);

  /*
	
  Sleep(500);	

  s.repeated_send_parameter("MPU_STICK::CONTROL::MP_RESET",1,10,200);	

  cout << "Motion Processing reset." << endl;

  Sleep(500);
  
  s.repeated_send_parameter("MPU_STICK::IMU_CONFIG_WRITE_ENABLE::IMU_CONFIG_CALIB_POINTS_WE",1,10,200);
  s.repeated_send_parameter("MPU_STICK::IMU_CONFIG_WRITE_ENABLE::IMU_CONFIG_CALIB_INTERVAL_WE",1,10,200);

  cout << "Config. write enabled" << endl;	

  Sleep(1000);

  s.repeated_send_parameter("MPU_STICK::IMU_CONFIG_REG_A::IMU_CONFIG_CALIB_INTERVAL",50,10,200);
  s.repeated_send_parameter("MPU_STICK::IMU_CONFIG_REG_A::IMU_CONFIG_CALIB_POINTS",200,10,200);
  
  cout << "Calibration configuration written to device" << endl;
  
  Sleep(1000);

  vector<string> parameters;
    
  parameters.push_back("MPU_STICK::MP_ROLL_FILTER::MP_ROLL_FILTER");
  parameters.push_back("MPU_STICK::MP_PITCH_FILTER::MP_PITCH_FILTER");
  parameters.push_back("MPU_STICK::MP_YAW_FILTER::MP_YAW_FILTER");

  s.monitor_parameters(parameters,30000);
  
  */
  
  /*
  
  vector<string> frames;
  
  frames.push_back("MPU_STICK::IMU_CONFIG_REG_B");
  
  s.remote_frame_request(0,frames,2500);  Sleep(250);
  s.remote_frame_request(0,frames,2500);  Sleep(250);
  s.remote_frame_request(0,frames,2500);  Sleep(250);
  s.remote_frame_request(0,frames,2500);  Sleep(250);
  s.remote_frame_request(0,frames,2500);  Sleep(250);

  cout << "Gyroscope Bandwidth (value): " << s.read_parameter("MPU_STICK::IMU_CONFIG_REG_B::IMU_CONFIG_GYRO_BW") << endl;	
  cout << "Accelerometer Bandwidth (value): " << s.read_parameter("MPU_STICK::IMU_CONFIG_REG_B::IMU_CONFIG_ACCEL_BW") << endl;	
  cout << "Gyroscope full-scale (value): " << s.read_parameter("MPU_STICK::IMU_CONFIG_REG_B::IMU_CONFIG_GYRO_FS") << endl;	
  cout << "Accelerometer full-scale (value): " << s.read_parameter("MPU_STICK::IMU_CONFIG_REG_B::IMU_CONFIG_ACCEL_FS") << endl;	
	
  cout << "Gyroscope Bandwidth (enum name): " << s.read_parameter_enum("MPU_STICK::IMU_CONFIG_REG_B::IMU_CONFIG_GYRO_BW") << endl;	
  cout << "Accelerometer Bandwidth (enum name): " << s.read_parameter_enum("MPU_STICK::IMU_CONFIG_REG_B::IMU_CONFIG_ACCEL_BW") << endl;	
  cout << "Gyroscope full-scale (enum name): " << s.read_parameter_enum("MPU_STICK::IMU_CONFIG_REG_B::IMU_CONFIG_GYRO_FS") << endl;	
  cout << "Accelerometer full-scale (enum name): " << s.read_parameter_enum("MPU_STICK::IMU_CONFIG_REG_B::IMU_CONFIG_ACCEL_FS") << endl;	

  */

  cout << "Cycle time (IMU_X_RAW): " << s.get_cycle_time("MPU_STICK::IMU_X_RAW",10000) << endl; 
  cout << "Cycle time (IMU_Y_RAW): " << s.get_cycle_time("MPU_STICK::IMU_Y_RAW",10000) << endl; 
  cout << "Cycle time (IMU_Z_RAW): " << s.get_cycle_time("MPU_STICK::IMU_Z_RAW",10000) << endl; 
  //cout << "Cycle time (IMU_X_PROCESSED): " << s.get_cycle_time("MPU_STICK::IMU_X_PROCESSED",10000) << endl; 
  cout << "Cycle time (IMU_Y_PROCESSED): " << s.get_cycle_time("MPU_STICK::IMU_Y_PROCESSED",10000) << endl; 
  cout << "Cycle time (IMU_Z_PROCESSED): " << s.get_cycle_time("MPU_STICK::IMU_Z_PROCESSED",10000) << endl; 
  cout << "Cycle time (MP_ROLL_ACCEL): " << s.get_cycle_time("MPU_STICK::MP_ROLL_ACCEL",10000) << endl; 
  cout << "Cycle time (MP_ROLL_GYRO): " << s.get_cycle_time("MPU_STICK::MP_ROLL_GYRO",10000) << endl; 
  cout << "Cycle time (MP_ROLL_FILTER): " << s.get_cycle_time("MPU_STICK::MP_ROLL_FILTER",10000) << endl; 
  cout << "Cycle time (MP_PITCH_ACCEL): " << s.get_cycle_time("MPU_STICK::MP_PITCH_ACCEL",10000) << endl; 
  cout << "Cycle time (MP_PITCH_GYRO): " << s.get_cycle_time("MPU_STICK::MP_PITCH_GYRO",10000) << endl; 
  cout << "Cycle time (MP_PITCH_FILTER): " << s.get_cycle_time("MPU_STICK::MP_PITCH_FILTER",10000) << endl; 
  cout << "Cycle time (MP_YAW_ACCEL): " << s.get_cycle_time("MPU_STICK::MP_YAW_ACCEL",10000) << endl; 
  cout << "Cycle time (MP_YAW_GYRO): " << s.get_cycle_time("MPU_STICK::MP_YAW_GYRO",10000) << endl; 
  cout << "Cycle time (MP_YAW_FILTER): " << s.get_cycle_time("MPU_STICK::MP_YAW_FILTER",10000) << endl; 
  cout << "Cycle time (MP_ROLL_ACCEL): " << s.get_cycle_time("MPU_STICK::MP_ROLL_ACCEL",10000) << endl; 
  cout << "Cycle time (MP_ROLL_GYRO): " << s.get_cycle_time("MPU_STICK::MP_ROLL_GYRO",10000) << endl; 
  cout << "Cycle time (MD_OUTPUT): " << s.get_cycle_time("MPU_STICK::MD_OUTPUT",10000) << endl; 
  cout << "Cycle time (MP_ROLL_ACCEL): " << s.get_cycle_time("MPU_STICK::MP_ROLL_ACCEL",10000) << endl; 
  cout << "Cycle time (MP_ROLL_GYRO): " << s.get_cycle_time("MPU_STICK::MP_ROLL_GYRO",10000) << endl; 
  cout << "Cycle time (LED_OUTPUT): " << s.get_cycle_time("MPU_STICK::LED_OUTPUT",10000) << endl; 
  
	
	
	
  Sleep(250);	
	
  s.close_node_connection(0,1000,30);
	
  Sleep(500);	
  //s.serial_port_flush(NULL);
  s.serial_port_close();
  s.debugLog.close();
  
  return(0);	
}