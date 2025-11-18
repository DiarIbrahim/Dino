// Code By : Diar Ibrahim,  Contact :  https://www.linkedin.com/in/diar-ibrahim-ali/

#pragma once

#include "CoreMinimal.h"
#include "DinoInventorySlotWidget.h"
#include "DinoInventorySlotWidget_Item.generated.h"


class UDinoInventoryWidget;

UCLASS()
class DINO_API UDinoInventorySlotWidget_Item : public UDinoInventorySlotWidget
{
	GENERATED_BODY()


public:

	void InitializeWithInventoryWidget(UDinoInventoryWidget* InventoryWidget);

	// can receive drop
	virtual bool CanReceiveDrop_Implementation(UDinoInventoryDragDropOperation* Operation)override;
	// when we receive a valid drop
	virtual void DropReceived_Implementation(UDinoInventoryDragDropOperation* Operation) override;
	

protected:
	UPROPERTY(BlueprintReadOnly)
	UDinoInventoryWidget* OwningInventoryWidget;
	
};
