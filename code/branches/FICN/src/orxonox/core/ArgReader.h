/**
 @file  Argreader.h
 @brief reads arguments
 @author Benjamin Knecht <beni_at_orxonox.net>
 */

#ifndef _ArgReader_H__
#define _ArgReader_H__

#include <string>

namespace orxonox {

  class ArgReader
  {
    public:
      ArgReader(int argc, char **argv);
      void checkArgument(std::string option, std::string& string, bool must=false);
      void checkArgument(std::string option, int& integer, bool must=false);
      void checkArgument(std::string option, float& floating, bool must=false);
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
#endif /* _ArgReader_H__ */
