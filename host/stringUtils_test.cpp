
#include <iostream>
#include <fstream>

#include "stringUtils.hpp"


int main()
{
  ifstream inputFile("test_param.csv");

  string line;

  while(getline(inputFile,line))
  {
    cout << line << endl;
	stringUtils::csv_strings(line);
	//stringUtils::charIndices(line,',');
  }	  
	
  stringUtils::charCount("qskkakjdwd",'k');
  
  return(0);	
}