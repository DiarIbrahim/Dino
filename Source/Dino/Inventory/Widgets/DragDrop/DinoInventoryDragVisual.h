// Code By : Diar Ibrahim,  Contact :  https://www.linkedin.com/in/diar-ibrahim-ali/

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "DinoInventoryDragVisual.generated.h"

 class UDinoInventorySlotWidget;

UCLASS()
class DINO_API UDinoInventoryDragVisual : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetDragSourceSlot(UDinoInventorySlotWidget* SourceSlotWidget);

protected:

	UPROPERTY(meta = (BindWidget))
	UImage* DraggedItemImage;


			
};
