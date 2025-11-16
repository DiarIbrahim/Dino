// Code By : Diar Ibrahim,  Contact :  https://www.linkedin.com/in/diar-ibrahim-ali/


#include "DinoInventorySlotWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "DragDrop/DinoInventoryDragDropOperation.h"
#include "DragDrop/DinoInventoryDragVisual.h"


UDinoInventorySlotWidget::UDinoInventorySlotWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {

}

FReply UDinoInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// if we have valid data and drag detected
	if (IsValidSlotData() &&  InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton)) {
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UDinoInventorySlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	UDinoInventoryDragVisual* DragVisualWidget = CreateWidget<UDinoInventoryDragVisual>(this, DragVisualClass);
	DragVisualWidget->SetDragSourceSlot(this);

	UDinoInventoryDragDropOperation* Operation = NewObject<UDinoInventoryDragDropOperation>();
	Operation->DefaultDragVisual = DragVisualWidget;
	Operation->SourceSlot = this;
	
	BP_OnDragStarted();

	OutOperation = Operation;
}

bool UDinoInventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	// this slot is already have item in it, we will ignore the drop call
	if (IsValidSlotData()) {
		BP_OnDropFailed();
		return false;
	}

	if (UDinoInventoryDragDropOperation* DinoOperation = Cast<UDinoInventoryDragDropOperation>(InOperation)) {
		if (IsValid(DinoOperation->SourceSlot)) {
			// copy the lsot data
			SetSlotData(DinoOperation->SourceSlot->GetSlotData());
			// empty source slot
			DinoOperation->SourceSlot->EmptySlotData();

			BP_OnRecivedDropSucessfully();

			DinoOperation->SourceSlot->BP_OnDroppedSucessfully();

			return true;
		}
	}

	BP_OnDropFailed();
	return false;
}

void UDinoInventorySlotWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::OnDragCancelled(InDragDropEvent, InOperation);
	BP_OnDropFailed();
}

void UDinoInventorySlotWidget::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);

	if (UDinoInventoryDragDropOperation* DinoOperation = Cast<UDinoInventoryDragDropOperation>(InOperation)) {
		if (DinoOperation->SourceSlot != this && IsValidSlotData() == false) {
			BP_OnDragHover_Enter(true);
			bWasValidDragHover = true;
			return;
		}
	}

	BP_OnDragHover_Enter(false);
	bWasValidDragHover = false;
}

void UDinoInventorySlotWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);

	BP_OnDragHover_Leave(bWasValidDragHover);
	bWasValidDragHover = false; // reset
}



void UDinoInventorySlotWidget::SetSlotData(const FDinoInventorySlot& InSlotData)
{
	SlotData = InSlotData;
	BP_SetSlotData(SlotData);
}

void UDinoInventorySlotWidget::UpdateSlotData(const FDinoInventorySlot& InSlotData)
{
	if (IsValidSlotData() == false) return;

	// make sure we only update when we receive an update for the same data
	if (InSlotData.ItemTag.MatchesTagExact( SlotData.ItemTag) == false) return;

	SlotData = InSlotData;

	BP_UpdateSlotData(SlotData);
}

void UDinoInventorySlotWidget::EmptySlotData()
{
	SlotData = FDinoInventorySlot();

	BP_EmptySlotData();
}

bool UDinoInventorySlotWidget::IsValidSlotData() const
{
	return SlotData.ItemTag.IsValid();
}
