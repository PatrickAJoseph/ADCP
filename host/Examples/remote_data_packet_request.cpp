
#include <chrono>
#include <thread>

#include "../ADCP.hpp"

#define BAUD_RATE			38400
#define NODE_ID				0
#define TIMEOUT_INTERVAL	400
#define NUMBER_OF_TRIES		20
#define SYMBOL_FILE			"MPU_Stick.csv"
#define NODE_NAME			"MPU_STICK"

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
			cout << "auto_connect : Successfully opened connection @ " << " port " << *port << endl;
			break;
		}
		else
		{
			cout << "auto_connect: Not able to open connection @ " << " port " << *port << endl; 
			adcp.serial_port_close();
		}
	}
}

int main()
{
	ADCP adcp;
	
	vector<string> ports;
	vector<string> request_frame_name;
	
	request_frame_name.push_back("");
	
	//ports.push_back("COM12");
	ports.push_back("COM13");
	
	auto_connect(adcp,ports,BAUD_RATE);

	adcp.read_file(SYMBOL_FILE);

	for( auto frame = (adcp.frames).begin() ; frame != (adcp.frames).end() ; frame++ )
	{
		request_frame_name[0] = (*frame).frame_name;
		
		if( adcp.remote_frame_request( adcp.get_node_id(NODE_NAME), request_frame_name, TIMEOUT_INTERVAL, NUMBER_OF_TRIES) )
		{
			cout << (*frame).frame_name << " >> received from device successfully" << endl;
		}
		else
		{
			cout << (*frame).frame_name << " >> NOT received from node !" << endl;
		}
	}
	
	adcp.serial_port_close();
	
	return(0);
}