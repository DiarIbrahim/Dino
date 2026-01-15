// Code By : Diar Ibrahim,  Contact :  https://www.linkedin.com/in/diar-ibrahim-ali/

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Dino/Inventory/DinoInventoryTypes.h"
#include "DinoInventoryFunctionLibrary.generated.h"


class UDinoInventoryComponent;

UCLASS()
class DINO_API UDinoInventoryFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/*
		finds and returns a copy of a slot data from a dino inventory slot container
		returns false if the item not found, true if found
	*/
	
	UFUNCTION(BlueprintCallable, Category = "Dino|Inventory")
	static bool GetInventorySlotByTag(const FDinoInventorySlotContainer& SlotContainer, const FGameplayTag& ItemTag, FDinoInventorySlot& OutFoundSlot);
	/*
		finds and returns a copy of a slot data from a dino inventory slot container
		returns false if the item not found, true if found
	*/
	UFUNCTION(BlueprintCallable, Category = "Dino|Inventory")
	static bool GetInventoryComponentSlotByTag(UDinoInventoryComponent* InventoryComponent, const FGameplayTag& ItemTag, FDinoInventorySlot& OutFoundSlot);

	// returns data for an inventory item (returns true if the item is valid in the InventoryItemRegistry in Settings > Dino > Dino Inventory)
	UFUNCTION(BlueprintCallable, Category = "Dino|Inventory")
	static bool GetDinoInventoryItemData(const FGameplayTag& ItemTag, FDinoInventoryItemData& OutItemData);

	// returns data for a pointer to an  inventory item(returns Valid pointer if the item is valid in the InventoryItemRegistry in Settings > Dino > Dino Inventory)
	static FDinoInventoryItemData* GetDinoInventoryItemDataPointer(const FGameplayTag& ItemTag);
	
};
