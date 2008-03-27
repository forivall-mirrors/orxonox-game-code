#include "Convert.h"
#include <iostream>

int main(int argc, char** argv)
{
  int input = rand();
  std::string sub;
  int output = 0;

  ConvertValue(&sub, input);
  ConvertValue(&output, sub);

  if (input == output)
    return 0;
  else
    return -1;
}
