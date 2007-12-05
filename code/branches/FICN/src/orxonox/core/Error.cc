#include "Error.h"

namespace orxonox
{
	Error::Error()
	{
		Error(0,"");
	}

	Error::Error(std::string errorMsg, int errorCode)
	{
		Error(errorCode, errorMsg);
	}

	Error::Error(int errorCode, std::string errorMsg)
	{
		std::cout << "Orxonox Error: ";

		if (errorMsg!="")
		{
			std::cout << errorMsg;	
		}
		else
		{
			std::string msg;
			switch (errorCode)
			{
				case 1:
					msg = "General error";
					break;
				default:
					msg = "Unknown error!";
			}
			std::cout << msg;
		}
		std::cout << " (Code " << errorCode << ")" << std::endl<< std::endl;
	}
}
