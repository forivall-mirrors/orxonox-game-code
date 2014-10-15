/*
 * DodgeRace.h
 *
 *  Created on: Oct 15, 2014
 *      Author: sriedel
 */

#ifndef DODGERACE_H_
#define DODGERACE_H_

namespace orxonox
{

	class DodgeRace
	{
		public:
			DodgeRace();
			virtual ~DodgeRace();

			void init();

		private:
			int point;
			int level;
			int lives

	};

} /* namespace orxonox */
#endif /* DODGERACE_H_ */
