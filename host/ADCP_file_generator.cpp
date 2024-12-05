
#include <iostream>
#include "ADCP.hpp"
//#include "ADCP_frames.h"

int main(void)
{
	ADCP s;
	
	s.read_file("test_param.csv");
	s.generate_frame_files(0);

	return(0);	
}