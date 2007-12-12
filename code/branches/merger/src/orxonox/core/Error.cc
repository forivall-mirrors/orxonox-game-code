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
		std::cout << "############################ "<< std::endl
							<< "#         Error "<<errorCode<< "          #"<< std::endl
							<< "############################ "<< std::endl
							<< "An error occured in Orxonox: "<< std::endl;
						
		if (errorMsg=="")
		{
			switch (errorCode)
			{
				case 1:
					errorMsg = "Some error!";
					break;
				default:
					errorMsg = "Unknown error!";
			}
		}
		std::cout << errorMsg << std::endl<< std::endl;
	}
}
