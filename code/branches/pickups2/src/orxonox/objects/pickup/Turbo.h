#ifndef _Turbo_H__
#define _Turbo_H__

#include "tools/Timer.h"
#include "Item.h"
#include "OrxonoxPrereqs.h"
#include "util/Math.h"

namespace orxonox
{
	class _OrxonoxExport Turbo : public Item
	{
	
	public:
	Turbo(BaseObject* creator);
	virtual ~Turbo();

	virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

	virtual bool pickedUp(Pawn* player);
	virtual bool dropped(Pawn* player); //erst bei festen Items

	void setSpeedBoost(SpaceShip* ship);
	void unsetSpeedBoost(SpaceShip* ship);

	inline void setBoost(float boost)
		{ this->boost_ = boost; }
	inline float getBoost() const
		{ return this->boost_; }

	inline void setDuration(float duration)
		{ this->duration_ = duration; }
	inline float getDuration() const
		{ return this->duration_; }

	inline void setRotAcc(Degree rotacc)
		{ this->rotacc_ = rotacc; }
	inline Degree getRotAcc() const
		{ return this->rotacc_; }

	
	inline void setAccBoost(float accboost)
		{ this->accboost_ = accboost; }
	inline float getAccBoost() const
		{ return this->accboost_; }

	private:
	Timer<Turbo> turbotimer_;
	float boost_;
	float duration_;
	float accboost_;
	Degree rotacc_;
	};
}

#endif /* _Turbo_H__ */
