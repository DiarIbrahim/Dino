// Code By : Diar Ibrahim,  Contact :  https://www.linkedin.com/in/diar-ibrahim-ali/


#include "DinoInventorySlotWidget_Item.h"
#include "DinoInventoryWidget.h"
#include "DragDrop/DinoInventoryDragDropOperation.h"

bool UDinoInventorySlotWidget_Item::CanReceiveDrop_Implementation(UDinoInventoryDragDropOperation* Operation)
{
	if (IsValid(Operation) == false) return false;

	// if from other item slot
	if (UDinoInventorySlotWidget_Item* AsItem = Cast<UDinoInventorySlotWidget_Item>(Operation->SourceSlot)) {
		// in this case we only allow drop from a valid Item slot to an Empty Slot
		return !IsValidSlotData();
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
