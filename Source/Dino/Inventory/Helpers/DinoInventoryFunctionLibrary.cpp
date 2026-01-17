// Code By : Diar Ibrahim,  Contact :  https://www.linkedin.com/in/diar-ibrahim-ali/


#include "DinoInventoryFunctionLibrary.h"
#include "Dino/Inventory/Registry/DinoInventorySettings.h"
#include "Dino/Inventory/DinoInventoryComponent.h"

bool UDinoInventoryFunctionLibrary::GetInventorySlotByTag(const FDinoInventorySlotContainer& SlotContainer, const FGameplayTag& ItemTag, FDinoInventorySlot& OutFoundSlot)
{
    return SlotContainer.GetItemSlot(ItemTag, OutFoundSlot);
}

bool UDinoInventoryFunctionLibrary::GetInventoryComponentSlotByTag(UDinoInventoryComponent* InventoryComponent, const FGameplayTag& ItemTag, FDinoInventorySlot& OutFoundSlot)
{
    if (IsValid(InventoryComponent)) {
       return InventoryComponent->GetInventorySlots().GetItemSlot(ItemTag, OutFoundSlot);
    }
    return false;
}

bool UDinoInventoryFunctionLibrary::GetDinoInventoryItemData(const FGameplayTag& ItemTag, FDinoInventoryItemData& OutItemData)
{
    if (UDataTable* DataTable = UDinoInventorySettingsHelpers::GetDinoInventoryRegistryData()) {

        TArray<FDinoInventoryItemData*> OutRows;
        DataTable->GetAllRows("", OutRows);

        for (FDinoInventoryItemData* Row : OutRows) {
            if(Row->ItemTag.MatchesTagExact(ItemTag)){
                OutItemData = *Row;
                return true;
            }
        }
    }
    return false;
}

TArray<FDinoInventoryItemData> UDinoInventoryFunctionLibrary::GetCraftableItems()
{
    TArray<FDinoInventoryItemData> CraftableItems = {};
    if (UDataTable* DataTable = UDinoInventorySettingsHelpers::GetDinoInventoryRegistryData())
    {
        TArray<FDinoInventoryItemData*> OutRows;
        DataTable->GetAllRows("", OutRows);

        for (FDinoInventoryItemData* Row : OutRows) {
            if(Row->CraftingData.CraftingPolicy == EDinoInventoryItemCraftingPolicy::Craftable){
                CraftableItems.Add(*Row);
            }
        }
        
    }

    return CraftableItems;
}
