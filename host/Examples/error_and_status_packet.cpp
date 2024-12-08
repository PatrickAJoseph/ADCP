
#include <chrono>
#include <thread>

#include "../ADCP.hpp"

#define BAUD_RATE			38400
#define NODE_ID				0
#define TIMEOUT_INTERVAL	500
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
			adcp.serial_port_flush(NULL);
			break;
		}
		else
		{
			cout << "auto_connect: Not able to open connection @ " << " port " << *port << endl; 
			adcp.serial_port_close();
		}
	}
}

const string get_status_string(ADCP::Network_Error_Status_t status)
{
	string _status;
	
	switch(status)
	{
		case(ADCP::Network_Error_Status_t::NONE):
		{
			_status = "NONE";
		}
		break;

		case(ADCP::Network_Error_Status_t::ALREADY_OPEN):
		{
			_status = "ALREADY_OPEN";
		}
		break;
		
		case(ADCP::Network_Error_Status_t::ALREADY_CLOSED):
		{
			_status = "ALREADY_CLOSE";
		}
		break;

		case(ADCP::Network_Error_Status_t::INVALID_FRAME_REQUESTED):
		{
			_status = "INVALID_FRAME_REQUESTED";
		}
		break;

		default:
		{
			_status = "TIMEOUT";
		}
	}
	
	return(_status);
}

int main()
{
	ADCP adcp;
	
	vector<string> ports;
	vector<string> request_frame_name;
	
	request_frame_name.push_back("");
	
	ports.push_back("COM13");
	
	auto_connect(adcp,ports,BAUD_RATE);

	adcp.read_file(SYMBOL_FILE);

	cout << "Opening connection...." << endl;

	adcp.open_node_connection( adcp.get_node_id(NODE_NAME), TIMEOUT_INTERVAL, NUMBER_OF_TRIES );	
	
	cout << "Network error status after opening connection when connection was already close : ";
	
	cout << get_status_string(adcp.get_node_network_error_status( adcp.get_node_id(NODE_NAME), TIMEOUT_INTERVAL, NUMBER_OF_TRIES )) << endl;

	cout << "Open connection command when connection to node is open" << endl;

	adcp.open_node_connection( adcp.get_node_id(NODE_NAME), TIMEOUT_INTERVAL, NUMBER_OF_TRIES );	
	
	cout << "Network error status after opening connection when connection was already open : ";
	
	cout << get_status_string(adcp.get_node_network_error_status( adcp.get_node_id(NODE_NAME), TIMEOUT_INTERVAL, NUMBER_OF_TRIES )) << endl;

	cout << "Close connection command when connection to node is open" << endl;

	adcp.close_node_connection( adcp.get_node_id(NODE_NAME), TIMEOUT_INTERVAL, NUMBER_OF_TRIES );	

	cout << "Network error status after closing connection when connection was already open : ";
	
	cout << get_status_string(adcp.get_node_network_error_status( adcp.get_node_id(NODE_NAME), TIMEOUT_INTERVAL, NUMBER_OF_TRIES )) << endl;
	
	cout << "Close connection command when connection to node is close" << endl;

	adcp.close_node_connection( adcp.get_node_id(NODE_NAME), TIMEOUT_INTERVAL, NUMBER_OF_TRIES );	

	cout << "Network error status after closing connection when connection was already closed : ";
	
	cout << get_status_string(adcp.get_node_network_error_status( adcp.get_node_id(NODE_NAME), TIMEOUT_INTERVAL, NUMBER_OF_TRIES )) << endl;

	cout << "Requesting for invalid packet from node" << endl;

	vector<string> request_frames;
		
	request_frames.push_back("MPU_STICK::INVALID_PACKET");
	
	adcp.remote_frame_request( adcp.get_node_id(NODE_NAME), request_frames, TIMEOUT_INTERVAL, NUMBER_OF_TRIES );
	
	cout << "Network error status after requesting for an invalid frame/packet from node : ";
	
	cout << get_status_string(adcp.get_node_network_error_status( adcp.get_node_id(NODE_NAME), TIMEOUT_INTERVAL, NUMBER_OF_TRIES )) << endl;
	
	adcp.serial_port_close();
	
	return(0);
}