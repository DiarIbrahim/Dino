// Code By : Diar Ibrahim,  Contact :  https://www.linkedin.com/in/diar-ibrahim-ali/

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"


#define DECLARE_TAG(TagName) UE_DECLARE_GAMEPLAY_TAG_EXTERN(TagName)
#define DECLARE_TAG2(Category, TagName) UE_DECLARE_GAMEPLAY_TAG_EXTERN(Category##_##TagName)



// tags related to inventory
namespace DinoInventoryTags
{
	// -- Item Tags
	// tags here will be like   Inventory.Item.Wood
	DECLARE_TAG2(Item, Wood)
	DECLARE_TAG2(Item, Bread)
	DECLARE_TAG2(Item, Rock)
	DECLARE_TAG2(Item, Iron)
	DECLARE_TAG2(Item, Grass)
	DECLARE_TAG2(Item, HealthPoint)
	DECLARE_TAG2(Item, Axe)
	DECLARE_TAG2(Item, Sword)


	// -- Item Action Tags
	// Tags will be               Inventory.ItemAction.Use
	DECLARE_TAG2(ItemAction, Use)
	DECLARE_TAG2(ItemAction, Equip)
	DECLARE_TAG2(ItemAction, Drop) // this can mean Remove item from inventory

	
}
