#ifndef _Error_H__
#define _Error_H__

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

#endif /* _Error_H__ */
