
#include <iostream>
#include "stringUtils.hpp"

using namespace std;


int main(void)
{
  string s = "[1,2,3,4,5]";	
  
  vector<string> elements = stringUtils::parse_array_string(s);

  for(int i = 0 ; i < elements.size() ; i++)
  {
	cout << elements[i] << endl;  
  }

  return(0);	
}