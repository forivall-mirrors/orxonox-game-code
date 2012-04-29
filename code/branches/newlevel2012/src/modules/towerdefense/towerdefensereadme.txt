===============================================================================
Classes
===============================================================================

TowerDefense:
GameType class

TowerDefenseCenterpoint
Centerpoint class. Store tower template

Tower
Represents a Tower


===============================================================================
Other files
===============================================================================

/data/levels/towerdefense.oxw
Loads the level. In <Level>, we first create a SpawnPoint. It spawns a cylinder using the 'centerpointmark' Template, which uses the 'centerpointmarkcamera' Template to create a static camera. Then, we create an instance of TowerDefenseCenterpoint and pass the 'towertemplate' Template, so that we can later create towers using this template. We also plase the playfield.mesh in a way that maps every field on the playfield.mesh to nice coordinates: the playfield spans as a square from -8,-8 to 8,8.