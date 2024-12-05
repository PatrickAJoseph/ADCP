
#include <iostream>
#include <chrono>
#include <windows.h>

using namespace std;


void progress_bar(double percentage)
{
  /* Number of characters to be printed: 2 + n_points. */	
	
  cout << "[";
  const unsigned int n_points = 50;

  for(int i = 0 ; i < n_points ; i++)
  {
    if( i < 0.01 * percentage * (double)n_points )
	{
		cout << "*";
	}
	else
	{
		cout << " ";
	}
  }
  
  cout << "]";
}

int main(void)
{
  for(int i = 0 ; i <= 10 ; i++)
  {	  
	progress_bar(10.0*(double)i);
	Sleep(1000);
	cout << "\r";
  }
  
  return(0);	
}