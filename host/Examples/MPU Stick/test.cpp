
#include "MPU_Stick.hpp"


#define COM_PORT				"COM12"
#define BAUD_RATE				38400

int main()
{
	MPU_Stick stick(COM_PORT,BAUD_RATE);
	
	stick.enable_transmission();
	Sleep(1000);
	stick.disable_transmission();
	
	stick.read_configuration(MPU_Stick::SAMPLING_RATE);
	
	cout << "Sampling rate: " << stick.configurations.sampling_rate << endl;
	
	return(0);
}