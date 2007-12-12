#ifndef __ERROR_CLASS__
#define __ERROR_CLASS__

#include <iostream>
#include <string>

namespace orxonox
{
	class Error
	{
	public:
		Error();
		Error(std::string errorMsg, int errorCode=0);
		Error(int errorCode, std::string errorMsg="");
	private:

	};
}

#endif
