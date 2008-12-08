namespace orxonox
{
    const int maxweapons_ =2; //Weaponslots	 (provisorisch)
    const int maxslots_= 50; //Inventoryslots (provisorisch)


    bool ShipEquipment::CheckifSpace()
    {
        if((Usable.size()+Trunk.size())>=maxslots_)
            return false;
        return true;
    }

    /* Checks if the Ship can pick an Item up. Permanents will give a "false" back unless the Ship doesnt carry a Item for that Slot (2 Weaponslots) yet.Others will be picked up unless there is no Space in the Trunk.*/

    bool ShipEquipment::CheckifValid(Shipitem* toBeChecked)
    {
        switch(toBeChecked.CheckType())
        {
        case Powerups:
            activatePowerUp(); //gibts noch nicht
            return true;
        case Permanent:
            switch (toBeChecked.CheckSubType())
            {
            case Weapon:
                int weaponcheck=0;
                multimap<string, ShipItem*>::iterator it;
                for ( it=Equipment.begin() ; it != Equipment.end(); it++ ){
                    if((*it).second->CheckSubType()==Weapon)
                        weaponcheck++;
                };
                if (weaponcheck>=maxweapons_){
                    weaponcheck=0;
                    return false;
                }
                break;
            case Thrusters:
                multimap<string, ShipItem*>::iterator it;
                for ( it=Equipment.begin() ; it != Equipment.end(); it++ ){
                    if((*it).second->CheckSubType()==Thrusters)
                        return false;
                }
                break;
            case Shields:
                multimap<string, ShipItem*>::iterator it;
                for ( it=Equipment.begin() ; it != Equipment.end(); it++ ){
                    if((*it).second->CheckSubType()==Shields)
                        return false;
                }
                break;
            case Armor:
                multimap<string, ShipItem*>::iterator it;
                for ( it=Equipment.begin() ; it != Equipment.end(); it++ ){
                    if((*it).second->CheckSubType()==Armor)
                        return false;
                }
                break;
            default:;
            }
        case Useable:
            return CheckifSpace();
        case default:;
        }
        return true;
    }

    /*Adds the Item to the Ship*/
    void ShipEquipment::AddItem(Shipitem* toAddItem)
    {
        if(CheckifValid(toAddItem)==true)
        {
            switch(toAddItem.CheckType())
            {
                case Permanent:
                    Equipment.insert ( pair<std::string, ShipItem*>(toAddItem.itemname,*toAddItem) );
                    break;
                case Usable:
                    Usable.insert ( pair<std::string, ShipItem*>(toAddItem.itemname,*toAddItem) );
                    break;
                case Trunk:
                    Trunk.insert ( pair<std::string, ShipItem*>(toAddItem.itemname,*toAddItem) );
                    break;
            }
        }
        else if(toAddItem.CheckType()==Permanent)
        {
            if(CheckifSpace()==true)
                Trunk.insert ( pair<std::string, ShipItem*>(toAddItem.itemname,*toAddItem) );
        }
    }
}
