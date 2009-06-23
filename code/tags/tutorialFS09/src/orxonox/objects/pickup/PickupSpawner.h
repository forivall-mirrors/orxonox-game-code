#ifndef _PickupSpawner_H__
#define _PickupSpawner_H__

#include "OrxonoxPrereqs.h"
#include "BaseItem.h"
#include "tools/Timer.h"
#include "objects/worldentities/StaticEntity.h"
#include "objects/Tickable.h"

namespace orxonox
{
	class _OrxonoxExport PickupSpawner : public StaticEntity, public Tickable
	{
	public:
	PickupSpawner(BaseObject* creator);
	virtual ~PickupSpawner();
	virtual void changedActivity();
	virtual void tick(float dt);
	virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
	virtual void triggering(Pawn* trigger); // Wenn ein Spieler in die Naehe kommt
	void triggerRespawnTimer();

	/*inline void setVisibility(bool visible)
		{ this->setActive(visible); }
	inline float getVisibility() const
		{ return this->isActive(); }*/

	void setItemTemplate(const std::string& itemtemplate);
	inline const std::string& getItemTemplate() const
		{ return this->itemtemplate_; }

	inline void setDistance(float distance)
		{ this->distance_ = distance; }
	inline float getDistance() const
		{ return this->distance_; }
	inline void setRespawnTimer (float respawntimer)
		{ this->respawntimer_ = respawntimer; }
	inline float getRespawnTimer() const
		{ return this->respawntimer_; }

	private:
	std::string itemtemplate_;
	Template* template_;
	float distance_;
	float respawntimer_;
	Timer<BaseObject> RespawnTimer_;
	};
}

#endif /* _PickupSpawner_H__ */

















