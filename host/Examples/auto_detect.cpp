


#include "../ADCP.hpp"

#include <chrono>
#include <thread>

#define AUTO_DETECT_PORTS	0
#define NUMBER_OF_TRIES		10
#define TIMEOUT_INTERVAL	250

int main()
{
	ADCP adcp;

	vector<string> ports;
	
	if(AUTO_DETECT_PORTS)
	{
		ports = adcp.serial_port_scan();		//	Get all serial ports.
	}
	else
	{
		ports.push_back("COM12");
		ports.push_back("COM13");
	}
	

	cout << "Available serial ports" << endl << endl;

	for(auto port = ports.begin() ; port != ports.end() ; port++ )
	{
		cout << *port << endl;
	}

	uint32_t baud_rates[] = { //CBR_300,	
							  //CBR_600,	
							  //CBR_1200,	
							  //CBR_2400,
							  //CBR_4800,	
							  //CBR_9600,	
							  //CBR_19200,	
							  CBR_38400,
							  //CBR_57600, 
							  CBR_115200}; 

	
	
	uint8_t reset_ack = 0;
	
	//	For all available serial ports.
	
	for(auto port = ports.begin(); port != ports.end() ; port++ )
	{
		//	For all possible standard baud rates.
		
		for(int baud_rate_idx = 0; baud_rate_idx < sizeof(baud_rates)/sizeof(baud_rates[0]) ; baud_rate_idx++)
		{
			cout << "Scanning for nodes @ " << *port << " with BAUD_RATE = " << baud_rates[baud_rate_idx] << endl; 
			
			adcp.serial_port_init(*port,baud_rates[baud_rate_idx]);
			Sleep(500);
			
			//	For all possible node IDs.
			
			for(int node_id = 0 ; node_id < 16 ; node_id++)
			{	
				reset_ack = adcp.reset_node(node_id,TIMEOUT_INTERVAL,NUMBER_OF_TRIES);	// Send sync/reset command to node and check for ACK.
				
				if( reset_ack )
				{
					cout << "Device/node detected !    " << "NODE_ID: " << node_id << " COM_PORT: " << *port << " BAUD_RATE: " << baud_rates[baud_rate_idx] << endl;
					
					break;
				}
			}
			
			if(!reset_ack)
			{
				cout << "No nodes found" << endl;
			}
			
			adcp.serial_port_close();	//	Close serial port.
		}
	}
	
	
	return(0);
}