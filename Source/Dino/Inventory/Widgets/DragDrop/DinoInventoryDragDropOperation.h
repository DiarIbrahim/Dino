// Code By : Diar Ibrahim,  Contact :  https://www.linkedin.com/in/diar-ibrahim-ali/

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "DinoInventoryDragDropOperation.generated.h"

class UDinoInventorySlotWidget;

UCLASS()
class DINO_API UDinoInventoryDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UDinoInventorySlotWidget* SourceSlot;

};
