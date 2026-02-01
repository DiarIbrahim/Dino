// Code By : Diar Ibrahim,  Contact :  https://www.linkedin.com/in/diar-ibrahim-ali/

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Dino/Inventory/DinoInventoryTypes.h"
#include "DinoInventoryItemActionWidget.generated.h"



// item action

UCLASS()
class DINO_API UDinoInventoryItemActionWidget : public UUserWidget
{
	GENERATED_BODY()


public:

protected:

	UPROPERTY(BlueprintReadOnly)
	FDinoInventoryItemActionData InventoryItemActionData;


public:

	void SetActionData(const FDinoInventoryItemActionData& InInventoryItemAction);

	UFUNCTION(BlueprintImplementableEvent,  DisplayName="On Item Action Data Set")
	void BP_OnActionDataSet(const FDinoInventoryItemActionData& ItemActionData);
	
	
};
