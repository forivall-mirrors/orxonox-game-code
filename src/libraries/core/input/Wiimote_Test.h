/*
 * WiimoteTest.h
 *
 *  Created on: Oct 21, 2013
 *      Author: georgr
 */

#ifndef WIIMOTETEST_H_
#define WIIMOTETEST_H_

#include <tools/interfaces/Tickable.h>
#include "core/CoreIncludes.h"
#include "core/command/ConsoleCommand.h"
#include "InputState.h"

namespace orxonox
{
	class Wiimote_Test
	        : public WindowEventListener

			, public Singleton<Wiimote_Test>
	{
		friend class Singleton<Wiimote_Test>;
		public:
			void tick(float dt);
			Wiimote_Test();
			virtual ~Wiimote_Test();
		private:
			static Wiimote_Test* singletonPtr_s;

	};
}
#endif /* WIIMOTETEST_H_ */
