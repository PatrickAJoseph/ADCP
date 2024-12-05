

#include <stdio.h>
#include "cli.hpp"

using namespace std;

int main(void)
{
  string filename;	
	
  cli_init();
  
  cout << "Enter script file name (script files have extension of .adcp_script):\t";
  
  cin >> filename;
  
  cli_script(filename);
	
  return(0);	 
}