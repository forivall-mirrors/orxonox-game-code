#ifndef __STRING2NUMBER_H__
#define __STRING2NUMBER_H__

#include <string>
#include <sstream>
#include <iostream>

#include "../orxonox/core/Debug.h"

/**
* String to number conversion
*
* This class converts a number inside a std::string
* into a numeric type number (int,float,double)
* Number in string can be decimal, hexadecimal or octal
*
* @author Nicolas Perrenoud<nicolape@ee.ethz.ch>
*
* @example
* float f;
* String2Number<float>(f, std::string(" 123.45 "));
*/
template <class T>
class String2Number
{
private:
	bool success_;
public:
	/**
	* Constructor
	*
	* First value is the target variable, the second vector is the
	* string where the number is taken from, the third parameter
	* should be one of std::hex, std::dec or std::oct (dec is default value)
	*/
	inline String2Number(T& t, const std::string& s, std::ios_base& (*f)(std::ios_base&) =  std::dec, int haltOnError=1)
	{
	  std::istringstream iss(s);
	  success_ = !(iss >> f >> t).fail();

	  if (!success_ && haltOnError==1)
	  {
	  	COUT(1) << "Error: Conversion from string to number in \"" << s << "\" failed." << std::endl;
	  	exit(1);
	  }
	}
};

#endif
