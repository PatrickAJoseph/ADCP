
#include <chrono>
#include <thread>

#include "../ADCP.hpp"

#define BAUD_RATE	38400
#define NODE_ID		0
#define TIMEOUT_INTERVAL	250
#define NUMBER_OF_TRIES		10


void auto_connect(ADCP& adcp,vector<string>& ports,uint32_t baud_rate)
{
	uint8_t found = 0;
	
	for( auto port = ports.begin() ; port != ports.end() ; port++ )
	{
		adcp.serial_port_init( *port, baud_rate );
		
		for(int node_id = 0 ; node_id < 16 ; node_id++)
		{
			if( adcp.reset_node( node_id, TIMEOUT_INTERVAL, NUMBER_OF_TRIES ) )
			{
				found = 1;
				break;
			}
		}
		
		if( found = 1 )
		{
			break;
		}
	}
}

int main()
{
	ADCP adcp;
	
	vector<string> ports;
	
	ports.push_back("COM12");
	ports.push_back("COM13");
	
	auto_connect(adcp,ports,BAUD_RATE);
	
	adcp.open_node_connection(NODE_ID,TIMEOUT_INTERVAL,NUMBER_OF_TRIES);
	
	Sleep(5000);

	adcp.close_node_connection(NODE_ID,TIMEOUT_INTERVAL,NUMBER_OF_TRIES);
	
	adcp.serial_port_close();
	
	return(0);
}