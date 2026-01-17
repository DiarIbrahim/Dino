// Code By : Diar Ibrahim,  Contact :  https://www.linkedin.com/in/diar-ibrahim-ali/

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "Dino/Inventory/DinoInventoryTypes.h"
#include "DinoInventoryCraftableDependencyWidget.generated.h"

/**
 *  this is shown as a dependency of a craftible item in UDinoInventoryCraftWidget for when a Craftable item is selected to show what is needed for the selected items to be crafted 
 */
UCLASS()
class DINO_API UDinoInventoryCraftableDependencyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	void UpdateData(const FDinoInventoryItemData& Item, int32 RequiredQuantity, int32 AvailableQuantity);
	UFUNCTION(BlueprintImplementableEvent, DisplayName="On Update Data")
	void BP_OnUpdateData(const FDinoInventoryItemData& Item, int32 RequiredQuantity, int32 AvailableQuantity);
};
