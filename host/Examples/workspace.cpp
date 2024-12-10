
#include "../ADCP.hpp"


#define COM_PORT			"COM12"
#define BAUD_RATE			38400
#define SYMBOL_FILE			"MPU_Stick.csv"

#define NODE_ID				0
#define NUMBER_OF_TRIES		20
#define TIMEOUT_INTERVAL	500

int main()
{
	ADCP adcp;
	
	adcp.serial_port_init(COM_PORT,BAUD_RATE);
	adcp.read_file(SYMBOL_FILE);
	adcp.reset_node(NODE_ID,TIMEOUT_INTERVAL,NUMBER_OF_TRIES);
	adcp.open_node_connection(NODE_ID,TIMEOUT_INTERVAL,NUMBER_OF_TRIES);
	
	for(int i = 0 ; i < 1000 ; i++)
	{
		adcp.read_and_update_frame(TIMEOUT_INTERVAL);
	}
	
	adcp.close_node_connection(NODE_ID,TIMEOUT_INTERVAL,NUMBER_OF_TRIES);
	adcp.serial_port_close();
	
	return(0);	
}
