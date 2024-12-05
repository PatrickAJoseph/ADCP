
#include <bits/stdc++.h>
#include "windows.h"

#define N_BYTES_RX		6	

using namespace std;

int main()
{
  HANDLE h_serial;
  
  h_serial = CreateFile("\\\\.\\COM6",							//	Name of file.
						GENERIC_READ | GENERIC_WRITE,		//	Enable both read and write.
						0,									//	sharing mode: None.
						0,									//	Default security descriptor.
						OPEN_EXISTING,						//	Open existing COM Port.
						FILE_ATTRIBUTE_NORMAL,				//	
						0);   
  
  uint8_t buffer[] = {2, 2, 0, 3, 0, 1, 73, 249};
  DWORD dwWrite = 0;
  
  if( !WriteFile(h_serial, buffer, 9, &dwWrite, NULL) )
  {
	cout << "Serial port write not successful !" << endl;  
  }
  else
  {
	cout << "Serial port write successful !" << endl;  
  }
  
  CloseHandle(h_serial);
  
  return(0);
}

