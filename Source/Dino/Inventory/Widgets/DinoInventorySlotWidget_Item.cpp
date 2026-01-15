// Code By : Diar Ibrahim,  Contact :  https://www.linkedin.com/in/diar-ibrahim-ali/


#include "DinoInventorySlotWidget_Item.h"
#include "DinoInventorySlotWidget_Craft.h"
#include "DinoInventoryWidget.h"
#include "DragDrop/DinoInventoryDragDropOperation.h"

void UDinoInventorySlotWidget_Item::SetOwningInventory(UDinoInventoryWidget* InventoryWidget)
{
	OwningInventoryWidget = InventoryWidget;
}

bool UDinoInventorySlotWidget_Item::CanReceiveDrop_Implementation(UDinoInventoryDragDropOperation* Operation)
{
	if (IsValid(Operation) == false) return false;

	// if from other item slot
	if (UDinoInventorySlotWidget_Item* AsItem = Cast<UDinoInventorySlotWidget_Item>(Operation->SourceSlot)) {
		// in this case we only allow drop from a valid Item slot to an Empty Slot
		return !IsValidSlotData();
	}
	else if (UDinoInventorySlotWidget_Craft* AsCraft = Cast<UDinoInventorySlotWidget_Craft>(Operation->SourceSlot)){
		// we are moving back from craft slot to item slot, in this case the slot can have valid data or not

		if (IsValidSlotData() == false) {
			// the slot is empty, we can just accept the drop and no more checks needed
			return true;
		}

		// the slot data is valid, lets see if the dragged slot have the same Item type as this slot
		if (SlotData.ItemTag.MatchesTagExact(Operation->SourceSlot->GetSlotData().ItemTag)) {
			// we still can accept this if the this slot is not already full
			return !SlotData.IsSlotFull();
		}
	}

	return false;
	
}

void UDinoInventorySlotWidget_Item::DropReceived_Implementation(UDinoInventoryDragDropOperation* Operation)
{
	Super::DropReceived_Implementation(Operation);

	// notify owner inventory about this change
	if (IsValid(OwningInventoryWidget)) {
		//
		OwningInventoryWidget->UpdateInventorySlotMap(SlotData.ItemTag, this);
	}

}
