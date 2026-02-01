// Code By : Diar Ibrahim,  Contact :  https://www.linkedin.com/in/diar-ibrahim-ali/


#include "DinoTags.h"


#define DEFINE_TAG(TagName, Tag) UE_DEFINE_GAMEPLAY_TAG(TagName, Tag)



//  -- Inventory Tags

// Item tags
DEFINE_TAG(DinoInventoryTags::Item_Wood,        "Inventory.Item.Wood")
DEFINE_TAG(DinoInventoryTags::Item_Bread,	    "Inventory.Item.Bread")
DEFINE_TAG(DinoInventoryTags::Item_Rock,        "Inventory.Item.Rock")
DEFINE_TAG(DinoInventoryTags::Item_Iron,        "Inventory.Item.Iron")
DEFINE_TAG(DinoInventoryTags::Item_Grass,       "Inventory.Item.Grass")
DEFINE_TAG(DinoInventoryTags::Item_HealthPoint, "Inventory.Item.HealthPoint")
DEFINE_TAG(DinoInventoryTags::Item_Axe,         "Inventory.Item.Axe")
DEFINE_TAG(DinoInventoryTags::Item_Sword,       "Inventory.Item.Sword")

// Item Action Tags
DEFINE_TAG(DinoInventoryTags::ItemAction_Use,		"Inventory.ItemAction.Use")
DEFINE_TAG(DinoInventoryTags::ItemAction_Equip,     "Inventory.ItemAction.Equip")
DEFINE_TAG(DinoInventoryTags::ItemAction_Drop,		"Inventory.ItemAction.Drop")
