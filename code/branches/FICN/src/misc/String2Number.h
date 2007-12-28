#ifndef _String2Number_H__
#define _String2Number_H__

#include <string>
#include <sstream>
#include <iostream>

#include "orxonox/core/Debug.h"

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


/**
    @brief Converts a number of any type to a string.
    @param variable The variable, containing the number.

    @example
    float f = 3.14;
    std::string output = number2String(f);
*/
template <typename T>
const std::string number2String(T variable)
{
    std::ostringstream oss;
    if (oss << variable)
        return std::string(oss.str());

    return std::string("");
}

/**
    @brief Converts a number of any type to a string and assigns a defaultvalue if the conversion fails.
    @param variable The variable, containing the number.
    @param fallbackString The assigned string if the conversion fails.

    @example
    float f = 3.14;
    std::string output = number2String(f, "0.000000");
*/
template <typename T>
const std::string number2String(T variable, const std::string& fallbackString)
{
    std::ostringstream oss;
    if (oss << variable)
        return std::string(oss.str());

    return fallbackString;
}

/**
    @brief Converts a string to a number of any type.
    @param variable The variable wherein the value gets written
    @return True if the conversion was successful

    @example
    float f;
    bool success = string2Number(f, "3.14");
*/
template <typename T>
bool string2Number(T& variable, const std::string& valueString)
{
    std::istringstream iss(valueString);
    if (iss >> variable)
        return true;

    return false;
}

/**
    @brief Converts a string to a number of any type.
    @param variable The variable wherein the value gets written
    @param fallbackValue The assigned value if the conversion fails
    @return True if the conversion was successful

    @example
    float f;
    bool success = string2Number(f, "3.14", 0.000000);
*/
template <typename T>
bool string2Number(T& variable, const std::string& valueString, T fallbackValue)
{
    std::istringstream iss(valueString);
    if (iss >> variable)
        return true;

    variable = fallbackValue;
    return false;
}

#endif /* _String2Number_H__ */
