// Code By : Diar Ibrahim,  Contact :  https://www.linkedin.com/in/diar-ibrahim-ali/

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagcontainer.h"
#include "Components/Image.h"
#include "Dino/Inventory/DinoInventoryTypes.h"
#include "DinoInventorySlotWidget.generated.h"

class UDinoInventorySlotWidget;
class UDinoInventoryDragVisual;
class UDinoInventoryComponent;
class UGridPanel;


/**
 * 
 */
UCLASS()
class DINO_API UDinoInventorySlotWidget : public UUserWidget
{

public:

	GENERATED_BODY()


	UDinoInventorySlotWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, Category = "Dino Inventory Slot")
	TSubclassOf<UDinoInventoryDragVisual> DragVisualClass;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Item_Image;



	// --- Drag and Drop
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void   NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation);
	virtual bool   NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);
	virtual void   NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);


	// called when slot data set for the first time
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Slot Data Set"))
	void BP_SetSlotData(const FDinoInventorySlot& InSlotData);
	void SetSlotData(const FDinoInventorySlot& InSlotData);

	// called when slot data updated (e.g. Quantity updated)
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Slot Data Updated"))
	void BP_UpdateSlotData(const FDinoInventorySlot& InSlotData);
	void UpdateSlotData(const FDinoInventorySlot& InSlotData);

	// clears the slot data
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Slot Data Cleared"))
	void BP_EmptySlotData();
	void EmptySlotData();

	// returns true when the slot is holding a valid data
	UFUNCTION(BlueprintPure)
	bool IsValidSlotData() const;

	FDinoInventorySlot GetSlotData() const { return SlotData; };


protected: 
	
	UPROPERTY(BlueprintReadOnly)
	FDinoInventorySlot SlotData;

	
};
