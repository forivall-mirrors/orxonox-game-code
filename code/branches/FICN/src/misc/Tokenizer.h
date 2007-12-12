#ifndef __TOKEINZER_H__
#define __TOKEINZER_H__

#include <string>
#include <iostream>

/**
* String tokenizer
*
* Splits a given string into several smaller strings
* using a delmiter (default is the comma).
* Returns the result as a vector<string> object
*
* @autor Nicolas Perrenoud<nicolape@ee.ethz.ch>
*/

inline std::vector<std::string> tokenize(const std::string& str, const std::string& delimiters = ",")
{
	vector<std::string> tokens;
	
  // Skip delimiters at beginning.
  std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
  // Find first "non-delimiter".
  std::string::size_type pos     = str.find_first_of(delimiters, lastPos);

  while (std::string::npos != pos || std::string::npos != lastPos)
  {
      // Found a token, add it to the vector.
      tokens.push_back(str.substr(lastPos, pos - lastPos));
      // Skip delimiters.  Note the "not_of"
      lastPos = str.find_first_not_of(delimiters, pos);
      // Find next "non-delimiter"
      pos = str.find_first_of(delimiters, lastPos);
  }	
  return tokens;    
}


#endif
