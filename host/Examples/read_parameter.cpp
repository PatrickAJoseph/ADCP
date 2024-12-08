
#include <chrono>
#include <pthread.h>
#include <stdio.h>

#include "../ADCP.hpp"

/**************** APPLICATION CONFIGURATIONS ********/

#define	COM_PORT				"COM13"
#define BAUD_RATE				38400
#define NODE_NAME				"MPU_STICK"
#define NODE_ID					0
#define SYMBOL_FILE				"MPU_Stick.csv"

#define TIMEOUT_INTERVAL		500	
#define NUMBER_OF_TRIES			10

#define RUN_TIME				10000
#define UPDATE_INTERVAL			250
#define MAXIMUM_CYCLE_TIME		2500

const string parameters_to_monitor[] = 
{
	"MPU_STICK::IMU_X_PROCESSED::IMU_X_ACCEL_PROCESSED",
	"MPU_STICK::IMU_Y_PROCESSED::IMU_Y_ACCEL_PROCESSED",
	"MPU_STICK::IMU_Z_PROCESSED::IMU_Z_ACCEL_PROCESSED"	
};

/**************	MAIN CODE *******************************/


ADCP adcp;

struct _PCB 
{
	void* args;
	uint8_t is_active; 
}_PCB;

typedef struct _PCB PCB; 

/* Thread for handling communication. */

pthread_t comm_thread;
PCB communication;


void* communication_task(void* args)
{
	cout << "Starting communication thread" << endl;
	
	do
	{		
	
		adcp.read_and_update_frame(MAXIMUM_CYCLE_TIME);	
	
	}while((((PCB*)args)->is_active));
	
	cout << "Stopping communication thread" << endl;
	
	pthread_exit(NULL);
}

/* Thread for handling display on command line */

pthread_t display_thread;
PCB display;

void* display_task(void* args)
{
	cout << "Starting display thread" << endl;
	
	uint64_t start_time = clock();
	uint64_t current_time = 0;
	uint64_t last_update_time = 0;
	
	double value;
	
	do
	{
		current_time = ( 1000 * ( ( (double)(clock() - start_time) ) / (double)CLOCKS_PER_SEC ) );
		
		if( ( current_time - last_update_time ) > UPDATE_INTERVAL )
		{
			for(int parameter_index = 0 ; parameter_index < sizeof(parameters_to_monitor)/sizeof(parameters_to_monitor[0]) ; parameter_index++ )
			{
				//value = adcp.read_parameter(parameters_to_monitor[parameter_index]);
				//cout << parameters_to_monitor[parameter_index] << " = " << value << endl; 
			}
			
			//cout << value << endl;
			
			last_update_time = current_time;
		}		
		
	}while((((PCB*)args)->is_active));
	
	cout << "Stopping display thread" << endl;
	
	pthread_exit(NULL);	
}

int main()
{	
	adcp.serial_port_init(COM_PORT,BAUD_RATE);
	adcp.read_file(SYMBOL_FILE);
	adcp.serial_port_flush(NULL);
	adcp.open_node_connection(NODE_ID, TIMEOUT_INTERVAL, NUMBER_OF_TRIES);
	
	adcp.reset_node( NODE_ID, TIMEOUT_INTERVAL, NUMBER_OF_TRIES );
	adcp.open_node_connection( NODE_ID, TIMEOUT_INTERVAL, NUMBER_OF_TRIES );

	communication.is_active = 1;
	display.is_active = 1;
	pthread_create(&comm_thread, NULL, &communication_task, &communication); 
	pthread_create(&display_thread, NULL, &display_task, &display); 
	
	
	uint64_t start_time = clock();
	uint64_t current_time = 0;
	
	do
	{
		current_time = ( 1000 * ( ( (double)(clock() - start_time) ) / (double)CLOCKS_PER_SEC ) );	
	}while( current_time < RUN_TIME );
	
	communication.is_active = 0;
	pthread_join( comm_thread, NULL );
	
	display.is_active = 0;
	pthread_join( display_thread, NULL );
	
	adcp.close_node_connection( NODE_ID, TIMEOUT_INTERVAL, NUMBER_OF_TRIES );
	adcp.serial_port_close();
	
	cout << "Exiting application" << endl;
	
	return(0);	
}