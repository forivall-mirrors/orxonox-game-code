/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *                    > www.orxonox.net <
 *
 *
 *   License notice:
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation; either version 2
 *   of the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *   Author:
 *      Reto Grieder
 *   Co-authors:
 *      Benjamin Knecht <beni_at_orxonox.net>
 *
 */

/**
 @file  ArgReader.cc
 @brief Argument Reader
 */

#include "ArgReader.h"
#include "SubString.h"
#include <ostream>
#include <iostream>

class FooBar
{
    //friend class ArgReader;
public:
    operator long long() const
    {
        return 0;
    }
    float float_;
    static FooBar& createFooBar();
private:
    //FooBar();
    //FooBar(const FooBar& instance);
    //~FooBar();
};

inline std::ostream& operator<<(std::ostream& outstream, const FooBar& fb)
{
    return outstream << fb.float_;
}
inline std::istream& operator>>(std::istream& instream,  const FooBar& fb);
//inline bool explicitConversion(const char** output, const FooBar input)
//{
//    return true;
//}

#include "Convert.h"

//template<>
//struct ConverterExplicit<const char*, FooBar>
//{
//    static bool convert(const char** output, const FooBar input)
//    {
//        return true;
//    }
//};

#include "MultiType.h"

ArgReader::ArgReader(int argc, char **argv)
{
  failure_ = false;
  errorString_ = "";
  CmdLineArg arg;

  //std::cout << errorString_;

  //const int a = conversionTests::ExplicitConversion<FooBar, const char*>::exists;
  //BOOST_STATIC_ASSERT(a == 0);
  //int val1;
  //long long val2 = 4LL;//conversion_cast<long long>(val1);
  //val2 = val1;
  //convertValue(&val2, val1);
  //convertValue(&FooBar(), val1);

  //using namespace1::fooBar1;
  //fooBar1();
  //int val1;
  //char val2;
  //convertValue(&val1, val2);

  //std::istringstream asdf;
  //asdf >> val2;

  int i = 1;
  while (i < argc)
  {
    if (argv[i][0] == '-' && argv[i][1] == '-') // name
    {
      if (argv[i][2] == '\0')
      {
        failure_ = true;
        errorString_ += "Expected word after \"--\".\n";
      }
      arg.bChecked_ = false;
      arg.name_ = argv[i] + 2;
      arg.value_ = "";
      arguments_.push_back(arg);
    }
    else // value
    {
      if (arguments_.size() == 0)
      {
        failure_ = true;
        errorString_ += "Expected \"--\" in command line arguments.\n";
        arg.bChecked_ = false;
        arg.name_ = "";
        arg.value_ = "";
        arguments_.push_back(arg);
      }

      if (arguments_.back().value_ != "")
        arguments_.back().value_ += " " + std::string(argv[i]);
      else
        arguments_.back().value_ = argv[i];
    }
    ++i;
  }
}

bool ArgReader::errorHandling()
{
  bool argumentsChecked = true;
  for (unsigned int i = 1; i < arguments_.size(); ++i)
    argumentsChecked &= arguments_[i].bChecked_;

  if (!argumentsChecked)
    errorString_ += "Not all arguments could be matched.\n";

  return !argumentsChecked || failure_;
}

const std::string& ArgReader::getErrorString()
{
  return errorString_;
}
