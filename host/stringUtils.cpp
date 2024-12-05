
#include "stringUtils.hpp"

#define STRING_UTIL_DEBUG cout << "STRING_UTILS >> " <<

void stringUtils::version()
{
  STRING_UTIL_DEBUG "String Utilization Tools v1.0" << endl;
}

char stringUtils::charAt(string s,int index)
{
  #ifdef DEBUG_STDOUT
    STRING_UTIL_DEBUG "Entering function: charAt(" << s << "," << index << ")" << endl;  
  #endif

  char rChar = 0;	//Character to be returned.

  try
  {  
	rChar = s[index];  
	  
    #ifdef DEBUG_STDOUT
		STRING_UTIL_DEBUG "Character at index " << index << " is " << rChar << endl;
	#endif	
  }
  catch(...)
  {
    #ifdef DEBUG_STDOUT
		STRING_UTIL_DEBUG "Index out of range !" << endl; 
	#endif
  }

  #ifdef DEBUG_STDOUT
    STRING_UTIL_DEBUG "Exiting function: charAt(" << s << "," << index << ")" << endl;  
  #endif		
}

int stringUtils::charIndex(string s, char c)
{
  #ifdef DEBUG_STDOUT
	STRING_UTIL_DEBUG "Entering function: charIndex(" << s << "," << c << ")" << endl;
  #endif	
	
  int index = -1;	

  for(int i = 0 ; i < s.size() ; i++ )
  {
    if( s[i] == c )
	{
      index = i;
	  break;
	}		
  }

  #ifdef DEBUG_STDOUT
	if( index == -1 )
	{
	  STRING_UTIL_DEBUG "Target character not found ....." << endl;		
	}
	else
	{
	  STRING_UTIL_DEBUG "Character " << c << " found at index: " << index << endl;	
	}
  #endif

  #ifdef DEBUG_STDOUT
	STRING_UTIL_DEBUG "Exiting function: charIndex(" << s << "," << c << ")" << endl;
  #endif	

  return(index);  
}



int stringUtils::charIndex(string s, char c, unsigned int occurence)
{
  #ifdef DEBUG_STDOUT
	STRING_UTIL_DEBUG "Entering function: charIndex(" << s << "," << c << "," << occurence << ")" << endl;
  #endif	
	
  int index = -1;	
  int occurence_count = 0;

  for(int i = 0 ; i < s.size() ; i++ )
  {
    if( s[i] == c )
	{
	  occurence_count++;
	  
	  if( occurence_count == occurence )
	  {
		index = i;   
	    break;		  
	  }
	}		
  }

  #ifdef DEBUG_STDOUT
	if( index == -1 )
	{
	  STRING_UTIL_DEBUG "Target character not found ....." << endl;		
	}
	else if( occurence_count < occurence )
	{
	  STRING_UTIL_DEBUG "Last occurence of character " << c << " is at index: " << index << " ( occurence " << occurence << ")" <<endl;	
	}
	else if( occurence_count == occurence )
	{
	  STRING_UTIL_DEBUG "Target character " << c << " found at index: " << index << endl;	
	}
  #endif

  #ifdef DEBUG_STDOUT
	STRING_UTIL_DEBUG "Exiting function: charIndex(" << s << "," << c << "," << occurence << ")" << endl;
  #endif	

  return(index);  
}


int stringUtils::searchSubstring(string s,string pattern)
{
  #ifdef DEBUG_STDOUT
	STRING_UTIL_DEBUG "Entering function: searchSubstring(" << s << "," << pattern << ")" << endl; 
  #endif  
	
	
  int pattern_char_match_count = 0;	
  int index = -1;
	
  for(int i = 0 ; i < s.size()-pattern.size() ; i++ )
  {
	pattern_char_match_count = 0;  
	  
	for(int j = 0 ; j < pattern.size() ; j++ )
	{
	  if( pattern[j] == s[i+j] )
	  {
		pattern_char_match_count++;
	  }		  
	}
	
	if( pattern_char_match_count == pattern.size() )
	{
	  index = i;	
	  break;	
	}
  }

  #ifdef DEBUG_STDOUT
	if( index != -1 )
	{
		STRING_UTIL_DEBUG "Substring found at position: " << index << endl;
	}
	else
	{
		STRING_UTIL_DEBUG "Substring not found !" << endl;
	}
  #endif	

  #ifdef DEBUG_STDOUT
	STRING_UTIL_DEBUG "Exiting function: searchSubstring(" << s << "," << pattern << ")" << endl; 
  #endif

  return(index);  
}

string stringUtils::stringBeforeMatch(string s,string pattern)
{
  #ifdef DEBUG_STDOUT
	STRING_UTIL_DEBUG "Entering function: stringBeforeMatch(" << s << "," << pattern << ")" << endl;
  #endif 

  int matchIndex = stringUtils::searchSubstring(s,pattern);
  string beforeMatch = s.substr(0,matchIndex);

  #ifdef DEBUG_STDOUT
	STRING_UTIL_DEBUG "String before pattern match: " << beforeMatch << endl;
  #endif	


  #ifdef DEBUG_STDOUT
	STRING_UTIL_DEBUG "Exiting function: stringBeforeMatch(" << s << "," << pattern << ")" << endl;
  #endif
  
  return(std::move(beforeMatch));
}
 
string stringUtils::stringAfterMatch(string s,string pattern)
{
  #ifdef DEBUG_STDOUT
	STRING_UTIL_DEBUG "Entering function: stringAfterMatch(" << s << "," << pattern << ")" << endl;
  #endif 

  int matchIndex = stringUtils::searchSubstring(s,pattern);
  string afterMatch = s.substr(matchIndex+pattern.size()); 	

  #ifdef DEBUG_STDOUT
	STRING_UTIL_DEBUG "String after pattern match: " << afterMatch << endl;
  #endif	

  #ifdef DEBUG_STDOUT
	STRING_UTIL_DEBUG "Exiting function: stringAfterMatch(" << s << "," << pattern << ")" << endl;
  #endif
  
  return(afterMatch);
}

unsigned int stringUtils::charCount(string s, char c)
{
  #ifdef DEBUG_STDOUT
	STRING_UTIL_DEBUG "Entering function: charCount(" << s << "," << c << ")" << endl;
  #endif	
	
  unsigned int count = 0;
  
  for(int i = 0 ; i < s.size() ; i++ )
  {
    if( s[i] == c )
	{
		count++;
	}
  }

  #ifdef DEBUG_STDOUT
	STRING_UTIL_DEBUG "Number of characters in string: " << count << endl;
  #endif  

  #ifdef DEBUG_STDOUT
	STRING_UTIL_DEBUG "Exiting function: charCount(" << s << "," << c << ")" << endl;
  #endif	

  return(count);  
}	
 
std::vector<unsigned int> stringUtils::charIndices(string s,char c)
{
  #ifdef DEBUG_STDOUT
    STRING_UTIL_DEBUG "Entering function: charIndices(" << s << "," << c << ")" << endl;
  #endif  
	
  vector<unsigned int> indices;
  
  for(int i = 0 ; i < s.size() ; i++)
  {
	if( s[i] == c )
	{
	  indices.push_back(i);
	  
	  #ifdef DEBUG_STDOUT
		STRING_UTIL_DEBUG "Target character found @ index: " << i << endl;
	  #endif
	}		
  }

  #ifdef DEBUG_STDOUT
    STRING_UTIL_DEBUG "Exiting function: charIndices(" << s << "," << c << ")" << endl;
  #endif  
  
  return(indices);
}	

std::vector<string> stringUtils::csv_strings(string s)
{
  #ifdef DEBUG_STDOUT
	STRING_UTIL_DEBUG "Entering function: csv_strings(" << s << ")" << endl;
  #endif	
	
  vector<unsigned int> indices = stringUtils::charIndices(s,',');
  vector<string> csvs;	
  string tempString;


  for(int i = 0 ; i < indices.size() ; i++)
  {
    if( i == 0 )
	{
	  tempString =  s.substr(0,indices[0]); 	
	}
	else if( (i != 0 ) && ( i < indices.size() ) )
	{
	  tempString = s.substr( indices[i-1] + 1, indices[i] - indices[i-1] - 1);	  
	}
	else
	{
	  tempString = s.substr( indices[i]+1, (s.size() - 1) - indices[i]  - 1);	
	}
	
    csvs.push_back(tempString);    
  }

  #ifdef DEBUG_STDOUT
    STRING_UTIL_DEBUG "Comma seperated values: " << endl;
  
	for(int i = 0 ; i < indices.size() ; i++ )
	{
		STRING_UTIL_DEBUG csvs[i] << endl;
	}
	
	STRING_UTIL_DEBUG endl;
  #endif	

  #ifdef DEBUG_STDOUT
	STRING_UTIL_DEBUG "Exiting function: csv_strings" << endl;
  #endif	

  return(csvs);  
}

std::vector<string> stringUtils::parse_array_string(string s)
{
  unsigned int n_elements = (stringUtils::charCount(s,',') + 1);
  
  vector<string> elements;
  
  if( n_elements == 1 )
  {
	unsigned int opening_bracket_position = stringUtils::charIndex(s,'[');
	unsigned int closing_bracket_position = stringUtils::charIndex(s,']');
	
	elements.push_back( s.substr(opening_bracket_position+1, closing_bracket_position - opening_bracket_position - 1) );
  }
  else
  {	  
    unsigned int start;
	unsigned int stop;

	  for(int i = 0 ; i < n_elements ; i++)
	  {
		if( i == 0 )
		{
			start = stringUtils::charIndex(s,'[');
			stop  = stringUtils::charIndex(s,',');
			
			elements.push_back( s.substr( start+1 , stop-start-1 ) );
		}
		else if( i != (n_elements-1) )
		{
			start = stringUtils::charIndex(s,',',i);
			stop  = stringUtils::charIndex(s,',',i+1);			
			elements.push_back( s.substr( start+1 , stop-start-1 ) );
		}
		else
		{
			start = stringUtils::charIndex(s,',',i);
			stop  = stringUtils::charIndex(s,']');			
			elements.push_back( s.substr( start+1 , stop-start-1 ) );			
		}
	  }
  }

  return(elements);	
}
