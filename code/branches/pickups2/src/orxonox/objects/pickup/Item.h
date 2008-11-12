namespace orxonox
{
	class _OrxonoxExport Item
	{
	public:
	virtual bool pickedUp(Pawn* player);
	virtual bool dropped(Pawn* player); //erst bei festen Items
	void addTo(Pawn* player);
	void removeFrom(Pawn* player);
	string getPlayerBaseClass();
	Item( BaseObject* creator);
	~Item();
	
	private:
	Identifier* PlayerBaseClass; //class(...)
	
	
	}
}
	