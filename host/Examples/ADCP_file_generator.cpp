
#include <iostream>
#include "../ADCP.hpp"

int main(void)
{
	ADCP s;
	
	s.read_file("MPU_Stick.csv");
	s.generate_frame_files(0);

	return(0);	
}