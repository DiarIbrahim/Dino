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
    else {
        
        // add the new item
        FDinoInventorySlot NewSlot;
        NewSlot.ItemTag = InItemTag;
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
    int32 ItemIndex = -1;
    for (size_t i = 0; i < Slots.Num(); i++) {
        if (Slots[i].ItemTag.MatchesTagExact(InItemTag)) {
            ItemIndex = i;
            break;
        }
    }
    return ItemIndex;
}
