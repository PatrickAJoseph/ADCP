
#ifndef STRINGUTILS_HPP
#define STRINGUTILS_HPP

#include <string>
#include <iostream>
#include <vector>

using namespace std;

namespace stringUtils
{
  void version();
  char charAt(string s,int index);
  int charIndex(string s, char c);
  int charIndex(string s, char c,unsigned int occurence);
  int searchSubstring(string s,string pattern);
  unsigned int charCount(string s,char c);
  std::vector<unsigned int> charIndices(string s,char c);
  string stringBeforeMatch(string s,string pattern);
  string stringAfterMatch(string s,string pattern);
  std::vector<string> csv_strings(string s);
  vector<string> parse_array_string(string s);
}

#endif /* STRINGUTILS_HPP */