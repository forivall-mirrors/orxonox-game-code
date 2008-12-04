#ifndef _PickupSpawner_H__
#define _PickupSpawner_H__

#include "OrxonoxPrereqs.h"
#include "Item.h"

#include "objects/worldentities/PositionableEntity.h"
#include "objects/Tickable.h"

namespace orxonox
{
	class _OrxonoxExport PickupSpawner : public PositionableEntity, public Tickable
	{
	public:
	PickupSpawner(BaseObject* creator);
	virtual ~PickupSpawner();

	virtual void tick(float dt);

    virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
	virtual void triggering(Pawn* trigger); // Wenn ein Spieler in die Naehe kommt

	void setItemTemplate(const std::string& itemtemplate);
	inline const std::string& getItemTemplate() const
		{ return this->itemtemplate_; }

	inline void setDistance(float distance)
		{ this->distance_ = distance; }
	inline float getDistance() const
		{ return this->distance_; }

	private:
	std::string itemtemplate_;
	Template* template_;
	float distance_;
	};
}

#endif /* _PickupSpawner_H__ */

















