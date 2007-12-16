/**
 @file  Argreader.h
 @brief reads arguments
 @author Benjamin Knecht <beni_at_orxonox.net>
 */

#ifndef __ARGREADER_CLASS__
#define __ARGREADER_CLASS__

#include <string>

namespace orxonox {

  class ArgReader
  {
    public:
      ArgReader(int argc, char **argv);
      void checkArgument(std::string option, std::string& string, bool must);
      void checkArgument(std::string option, int& integer, bool must);
      void checkArgument(std::string option, float& floating, bool must);
      bool errorHandling();
    private:
      int checkOption(std::string option);

    private:
      int counter_;
      char **arguments_;
      bool fail_;
      std::string errorStr_;
  };

}
#endif
