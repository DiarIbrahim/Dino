// Code By : Diar Ibrahim,  Contact :  https://www.linkedin.com/in/diar-ibrahim-ali/

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagcontainer.h"
#include "Components/Image.h"
#include "Dino/Inventory/DinoInventoryTypes.h"
#include "DinoInventorySlotWidget.generated.h"

class UDinoInventoryItemActionMenuWidget;
class UMenuAnchor;
class UDinoInventoryWidget;
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

	UPROPERTY(EditAnywhere, Category = "Dino Inventory")
	TSubclassOf<UDinoInventoryDragVisual> DragVisualClass;

	UPROPERTY(EditAnywhere, Category = "Dino Inventory")
	TSubclassOf<UDinoInventoryItemActionMenuWidget> ItemActionMenuWidgetClass;
	

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Item_Image;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UMenuAnchor* ActionMenuAnchor;
	
	UPROPERTY(BlueprintReadOnly)
	UDinoInventoryWidget* OwningInventoryWidget;
	


	// --- Drag and Drop
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void   NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation);
	virtual bool   NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);
	virtual void   NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);

	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	void SetOwningInventory(UDinoInventoryWidget* InventoryWidget);


	
	UFUNCTION(BlueprintCallable, Category = "Dino Inventory Slot")
	void ShowActionMenu();

	UFUNCTION(BlueprintCallable, Category = "Dino Inventory Slot")
	void HideActionMenu();

	UFUNCTION()
	UUserWidget* ConstructItemActionMenu();
	
	// are we allowing to drag this slot ?
	UFUNCTION(BlueprintNativeEvent, Category = "Dino Inventory Slot")
	bool IsDragAllowed();
	virtual bool IsDragAllowed_Implementation();


	// drag requested and sucessfully started
	UFUNCTION(BlueprintNativeEvent, Category = "Dino Inventory Slot")
	void DragStarted();
	virtual void DragStarted_Implementation();

	// can we drop here (an other slot is deagged to this slot)
	UFUNCTION(BlueprintNativeEvent, Category = "Dino Inventory Slot")
	bool CanReceiveDrop(UDinoInventoryDragDropOperation* Operation);
	virtual bool CanReceiveDrop_Implementation(UDinoInventoryDragDropOperation* Operation);

	// we accepted and recived a drop (from an other slot) (sucessful operation)
	UFUNCTION(BlueprintNativeEvent, Category = "Dino Inventory Slot")
	void DropReceived(UDinoInventoryDragDropOperation* Operation);
	virtual void DropReceived_Implementation(UDinoInventoryDragDropOperation* Operation);

	// when we are dropped on other slots (this slot dragged then dropped on an other slot) (sucessful operation)
	UFUNCTION(BlueprintNativeEvent, Category = "Dino Inventory Slot")
	void Dropped(UDinoInventoryDragDropOperation* Operation);
	virtual void Dropped_Implementation(UDinoInventoryDragDropOperation* Operation);

	// Drop Failed, when this item drogged and then drop operation failed or rejected
	UFUNCTION(BlueprintNativeEvent, Category = "Dino Inventory Slot")
	void DropFailed();
	virtual void DropFailed_Implementation();

	
	// when data set for the first time (or data is cleared and set again)
	UFUNCTION(BlueprintNativeEvent, Category = "Dino Inventory Slot")
	void SetSlotData(const FDinoInventorySlot& InSlotData);
	virtual void SetSlotData_Implementation(const FDinoInventorySlot& InSlotData);

	// when data updated
	UFUNCTION(BlueprintNativeEvent, Category = "Dino Inventory Slot")
	void UpdateSlotData(const FDinoInventorySlot& InSlotData);
	virtual void UpdateSlotData_Implementation(const FDinoInventorySlot& InSlotData);

	// when data updated
	UFUNCTION(BlueprintNativeEvent, Category = "Dino Inventory Slot")
	void EmptySlotData();
	virtual void EmptySlotData_Implementation();


	

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Drag Hover Enter"))
	void BP_OnDragHover_Enter(bool bCanBeDropped);
	
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Drop Hover Leave"))
	void BP_OnDragHover_Leave(bool bCanBeDropped);


	// returns true when the slot is holding a valid data
	UFUNCTION(BlueprintPure)
	bool IsValidSlotData() const;

	FDinoInventorySlot GetSlotData() const { return SlotData; };


protected: 
	
	UPROPERTY(BlueprintReadOnly)
	FDinoInventorySlot SlotData;

	bool bWasValidDragHover = false;

	
};
