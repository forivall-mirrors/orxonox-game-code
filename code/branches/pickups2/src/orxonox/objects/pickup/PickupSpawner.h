

namespace orxonox
{
	class _OrxonoxExport PickupSpawner: public PositionableEnity
	{
	public:
	PickupSpawner(BaseObject* creator);
	virtual ~PickupSpawner();

        virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
	virtual void processEvent(Event& event);
	virtual void changedActivity();
	virtual void triggering(bool active, DistanceTrigger* trigger); // Wenn ein Spieler in die Naehe kommt
	void activateSpawner(bool active);

	void setItemTemplate(const std::string& itemtemplate);
	inline const sdt::string& getItemTemplate() const
		{ return this->itemtemplate_; }
	
	private:
	std::string itemtemplate_;
	Template* template_;
	bool isActive;
	}
	
	
	
















}
