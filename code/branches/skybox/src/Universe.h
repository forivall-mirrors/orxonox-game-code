class Universe
{
	private:
		int numberOfStars;
		int lightcoordinates[77000][3];

	public:
		Universe(int number);
		void createCoordinates();
		void createBillboards(Ogre::SceneManager *mgrtemp,  Ogre::Camera * camtemp, Ogre::Root *mRoottemp);
		
};
