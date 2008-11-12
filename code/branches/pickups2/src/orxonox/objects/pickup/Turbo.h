#include "tools/Timer.h"

namespace orxonox
{
	class _OrxonoxExport Turbo: public Item
	{
	
	public:
	void setSpeedBoost(int boost, float duration, Spaceship* ship)
	void unsetSpeedBoost(int boost, Spaceship* ship)
	private:
	Timer<Turbo> turbotimer_;
	float boost;
	float duration;
	}
}