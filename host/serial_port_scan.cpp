
#include <iostream>
#include <windows.h>

using namespace std;

static void show_serial_ports()
{
  string comPortName = ("\\\\.\\COM");

  for(int i = 0 ; i < 256 ; i++)
  {
  comPortName = ("\\\\.\\COM" + to_string(i));	  
	  
  HANDLE serialHandle =  CreateFile(comPortName.c_str(),			//	Name of file.
						GENERIC_READ | GENERIC_WRITE,		//	Enable both read and write.
						0,									//	sharing mode: None.
						0,									//	Default security descriptor.
						OPEN_EXISTING,						//	Open existing COM Port.
						FILE_ATTRIBUTE_NORMAL,				//	
						0);         	

  if( serialHandle == INVALID_HANDLE_VALUE )
  {
    if( GetLastError() == ERROR_FILE_NOT_FOUND )
	{
		//cout << "COM Port: " << comPortName << " not found" << endl; 
	}
  }
  else
  {
		cout << "Connection open @ " << comPortName << endl;
  }
  }  	
}

int main(void)
{	

  show_serial_ports();
  
  return 0;	
}