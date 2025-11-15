// Code By : Diar Ibrahim,  Contact :  https://www.linkedin.com/in/diar-ibrahim-ali/


#include "DinoInventoryTypes.h"

bool FDinoInventorySlotContainer::ContainsItem(const FGameplayTag& InItemTag) const
{
    for (const FDinoInventorySlot& Slot : Slots) {

        if (Slot.ItemTag.MatchesTagExact(InItemTag)) return true;
    }

    return false;
}

bool FDinoInventorySlotContainer::AddItem(const FGameplayTag& InItemTag, int32 QuantityToAdd)
{
    if (ContainsItem(InItemTag)) {

        int32 ItemIndex = GetItemSlotIndex(InItemTag);

        if (Slots[ItemIndex].IsSlotFull()) {
            // the slot for the item exists and it is full
            return false;
        }


        //just updated the quanity
        for (FDinoInventorySlot& Slot : Slots) {

            if (Slot.ItemTag.MatchesTagExact(InItemTag)) {
                
                int32 NewQuantity = Slot.ItemQuantity + FMath::Abs(QuantityToAdd);
                NewQuantity = FMath::Min(NewQuantity, Slot.ItemCapacity);
                Slot.ItemQuantity = NewQuantity;
                return true;
            }
        }
    }
    else if (Slots.Num() < MaxSlotCount){
        
        // add the new item
        FDinoInventorySlot NewSlot;
        NewSlot.ItemTag = InItemTag;
        NewSlot.ItemCapacity = DefaultSlotItemCapacity;
        NewSlot.ItemQuantity = FMath::Min(QuantityToAdd, NewSlot.ItemCapacity);
        Slots.AddUnique(NewSlot);
        return true;
    }
    
    return false;

}

bool FDinoInventorySlotContainer::RemoveItem(const FGameplayTag& InItemTag, int32 QuantityToRemove)
{
    if (ContainsItem(InItemTag)) {

        int32 ItemIndex = GetItemSlotIndex(InItemTag);

        // remove the item if QuantityToRemove == -1  or QuantityToRemove is more or equal to current quantity of the item
        if (QuantityToRemove == -1 || Slots[ItemIndex].ItemQuantity <= FMath::Abs(QuantityToRemove)) {
            Slots.RemoveAt(ItemIndex);
            return true;
        }
        else {
            // else subtract the quantity
            int32 NewQuantity = Slots[ItemIndex].ItemQuantity - FMath::Abs(QuantityToRemove);
            Slots[ItemIndex].ItemQuantity = NewQuantity;
            return true;
        }
    }

    return false;
}

int32 FDinoInventorySlotContainer::GetItemSlotIndex(const FGameplayTag& InItemTag) const
{
    for (size_t i = 0; i < Slots.Num(); i++) {
        if (Slots[i].ItemTag.MatchesTagExact(InItemTag)) {
            return i;
        }
    }
    return -1;
}

bool FDinoInventorySlotContainer::GetItemSlot(const FGameplayTag& InItemTag, FDinoInventorySlot& OutFoundSlot) const
{
    for (size_t i = 0; i < Slots.Num(); i++) {
        if (Slots[i].ItemTag.MatchesTagExact(InItemTag)) {
            OutFoundSlot = Slots[i];
            return true;
        }
    }
    return false;
}

int32 FDinoInventorySlotContainer::GetItemQuantity(const FGameplayTag& InItemTag) const
{
    const int32 ItemIndex = GetItemSlotIndex(InItemTag);
    return ItemIndex == -1 ? -1 : Slots[ItemIndex].ItemQuantity;
}

void FDinoInventorySlotContainer::SetMaxSlotCount(int32 NewMaxSlot)
{
    // the max slot shall never be smaller than 1
    if (NewMaxSlot < 1) return;

    MaxSlotCount = NewMaxSlot;

    // TODO in case of shriniking find a way to handle the extra slots
}

void FDinoInventorySlotContainer::SetDefaultSlotItemCapacity(int32 NewDefaultSlotItemCapacity)
{
    if (NewDefaultSlotItemCapacity < 1) return;

    DefaultSlotItemCapacity = NewDefaultSlotItemCapacity;

    // TODO handle the cases when we want to update the item capacity for existing slots.
}
