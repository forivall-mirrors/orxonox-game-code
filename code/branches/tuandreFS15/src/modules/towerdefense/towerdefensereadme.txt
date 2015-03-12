===============================================================================
Classes
===============================================================================

TowerDefense:
GameType class

TowerDefenseCenterpoint
Centerpoint class. Store tower template

Tower
Represents a Tower
I'm not actually sure if you have to code C++ in order to have your result. I would suggest to simply create a new spaceship template or maybe better a
new template for a pawn. (The tower don't need to be spaceships; pawn is just fine.)
Example for a template: /data/levels/templates/assff.oxt && /data/levels/include/weaponSettingsAssff.oxi
@ assff.oxt: you don't need an engine (or your towers could fly away :-) and probably no Cameras. (just try what happens if you leave them out.) 
@ weaponSettingsAssff.oxi:
<weaponslots>: location where you want to have your tower to be equiped with weapons
<weaponsets> : if you press fire, you actually always fire a whole set of weapons. The amount of weaponsets is defined here. (I'm not 100% sure whether this explanation is right.)

There are three <Weapon> tags - one for each weaponslot. Each weapon can contain several objects which you would probably
refer to as "weapons":

LightningGun: Shoots a slow, rotation, yellow "ball".
HsW01: the default laser weapon
SimpleRocketFire: a weak (self steering) rocket
RocketFire: a strong rocket
                         
About your Tower's Template location - either keep it in the level or do it similarly to the Assff templates.
(If your towers behave & look nicely, I'm quite sure others want to include them in other levels, too.)


===============================================================================
Other files
===============================================================================

/data/levels/towerdefense.oxw
Loads the level. In <Level>, we first create a SpawnPoint. It spawns a cylinder using the 'centerpointmark' Template, which uses the 'centerpointmarkcamera' Template to create a static camera. Then, we create an instance of TowerDefenseCenterpoint and pass the 'towertemplate' Template, so that we can later create towers using this template. We also plase the playfield.mesh in a way that maps every field on the playfield.mesh to nice coordinates: the playfield spans as a square from -8,-8 to 8,8.