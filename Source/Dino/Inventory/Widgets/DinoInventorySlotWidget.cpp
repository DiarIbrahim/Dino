// Code By : Diar Ibrahim,  Contact :  https://www.linkedin.com/in/diar-ibrahim-ali/


#include "DinoInventorySlotWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/MenuAnchor.h"
#include "Dino/Inventory/Helpers/DinoInventoryFunctionLibrary.h"
#include "DragDrop/DinoInventoryDragDropOperation.h"
#include "DragDrop/DinoInventoryDragVisual.h"
#include "ItemAction/DinoInventoryItemActionMenuWidget.h"


UDinoInventorySlotWidget::UDinoInventorySlotWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {

}

FReply UDinoInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// if we have valid data and drag detected
	if (IsDragAllowed() && InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton)) {
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

	OutOperation = Operation;
	
	DragStarted();
}

bool UDinoInventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{

	if (UDinoInventoryDragDropOperation* DinoOperation = Cast<UDinoInventoryDragDropOperation>(InOperation)) {

		if (CanReceiveDrop(DinoOperation)) {
			
			DropReceived(DinoOperation); // we received a drop
			DinoOperation->SourceSlot->Dropped(DinoOperation); // we are dropped

			return true;
		}
	}

	DropFailed();
	return false;
}

void UDinoInventorySlotWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::OnDragCancelled(InDragDropEvent, InOperation);
	DropFailed();
}

void UDinoInventorySlotWidget::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);

	UDinoInventoryDragDropOperation* DinoOperation = Cast<UDinoInventoryDragDropOperation>(InOperation);

	// invalid dino operation
	if (IsValid(DinoOperation) == false) return;

	//  same slot, no drag enter effect needed
	if (DinoOperation->SourceSlot == this) return;

	if (CanReceiveDrop(DinoOperation)) {
		BP_OnDragHover_Enter(true);
		bWasValidDragHover = true;
	}
	else {

		BP_OnDragHover_Enter(false);
		bWasValidDragHover = false;
	}
}

void UDinoInventorySlotWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);

	UDinoInventoryDragDropOperation* DinoOperation = Cast<UDinoInventoryDragDropOperation>(InOperation);
	
	// invalid dino operatopn
	if (IsValid(DinoOperation) == false) return;

	// same slot, no drag effect needed
	if (DinoOperation->SourceSlot == this) return;

	BP_OnDragHover_Leave(bWasValidDragHover);
	bWasValidDragHover = false; // reset
		
	
}

void UDinoInventorySlotWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

}

void UDinoInventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if(IsValid(ActionMenuAnchor))
	{
		ActionMenuAnchor->OnGetUserMenuContentEvent.BindDynamic(this, &UDinoInventorySlotWidget::ConstructItemActionMenu);
	}

}

void UDinoInventorySlotWidget::SetOwningInventory(UDinoInventoryWidget* InventoryWidget)
{
	OwningInventoryWidget = InventoryWidget;
}

void UDinoInventorySlotWidget::ShowActionMenu()
{

	if(IsValid(ActionMenuAnchor) == false) return;

	if(ActionMenuAnchor->IsOpen()) return;
	
	// if this itm has any action or allows action

	FDinoInventoryItemData OutItemData;
	UDinoInventoryFunctionLibrary::GetDinoInventoryItemData(SlotData.ItemTag, OutItemData);

	// we have no action
	if(OutItemData.ItemActionData.bHasActions == false || OutItemData.ItemActionData.Actions.IsEmpty()) return;
	
	ActionMenuAnchor->Open(false);
	
	
}

void UDinoInventorySlotWidget::HideActionMenu()
{
	if(IsValid(ActionMenuAnchor) == false) return;

	if(ActionMenuAnchor->IsOpen() == false) return;
	
	ActionMenuAnchor->Close();
	
}

UUserWidget* UDinoInventorySlotWidget::ConstructItemActionMenu()
{
	if(IsValid(ItemActionMenuWidgetClass) == false) return nullptr;

	FDinoInventoryItemData OutItemData;
	UDinoInventoryFunctionLibrary::GetDinoInventoryItemData(SlotData.ItemTag, OutItemData);

	// we have no action
	if(OutItemData.ItemActionData.bHasActions == false || OutItemData.ItemActionData.Actions.IsEmpty()) return nullptr;
	
	UDinoInventoryItemActionMenuWidget* Menu = CreateWidget<UDinoInventoryItemActionMenuWidget>(GetWorld(), ItemActionMenuWidgetClass);
	Menu->SetItemData(OutItemData);

	return Menu;
}



bool UDinoInventorySlotWidget::IsDragAllowed_Implementation()
{
	// we should only drag slots that hold valid data
	return IsValidSlotData();
}

void UDinoInventorySlotWidget::DragStarted_Implementation()
{
}

bool UDinoInventorySlotWidget::CanReceiveDrop_Implementation(UDinoInventoryDragDropOperation* Operation)
{
	// In the base class, we will allow the drop as long as the slot is empty
	// we might need more calulations for _Item class when we move slot from _craft slot to _item slot (like aditions and so on)
	return !IsValidSlotData();
}

void UDinoInventorySlotWidget::DropReceived_Implementation(UDinoInventoryDragDropOperation* Operation)
{
	// copy the slot data
	SetSlotData(Operation->SourceSlot->GetSlotData());
}

void UDinoInventorySlotWidget::Dropped_Implementation(UDinoInventoryDragDropOperation* Operation)
{
	// Clear our data
	EmptySlotData();
}

void UDinoInventorySlotWidget::DropFailed_Implementation()
{
}

void UDinoInventorySlotWidget::SetSlotData_Implementation(const FDinoInventorySlot& InSlotData)
{
	if (InSlotData.IsValidSlot()) {
		SlotData = InSlotData;
	}
}

void UDinoInventorySlotWidget::UpdateSlotData_Implementation(const FDinoInventorySlot& InSlotData)
{
	if (IsValidSlotData() == false) return;

	// make sure we only update when we receive an update for the same data
	if (InSlotData.ItemTag.MatchesTagExact(SlotData.ItemTag) == false) return;

	SlotData = InSlotData;

}


void UDinoInventorySlotWidget::EmptySlotData_Implementation()
{
	SlotData = FDinoInventorySlot();
}

bool UDinoInventorySlotWidget::IsValidSlotData() const
{
	return SlotData.ItemTag.IsValid();
}
