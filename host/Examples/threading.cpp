// C program to show thread functions 

#include <pthread.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <cstdint>
#include <iostream>

using namespace std;

typedef struct _PCB_struct
{
	void* parameters;
	uint8_t stop_execution;
}PCB;


PCB thread_params;

void* func(void* arg) 
{ 
	// detach the current thread 
	// from the calling thread 
	//pthread_detach(pthread_self()); 

	cout << "Starting thread execution" << endl;
	while( (((PCB*)arg)->stop_execution) == 0 ); 
	cout << "Stopping execution" << endl;

	// exit the current thread 
	pthread_exit(NULL); 
} 


void fun() 
{ 
	pthread_t ptid; 

	thread_params.stop_execution = 0;

	pthread_create(&ptid, NULL, &func, &thread_params); 

	thread_params.stop_execution = 1;

	pthread_join(ptid, NULL); 

	

	printf("This line will be printed"
		" after thread ends\n"); 

	pthread_exit(NULL); 
} 

// Driver code 
int main() 
{ 
	fun(); 
	return 0; 
} 
