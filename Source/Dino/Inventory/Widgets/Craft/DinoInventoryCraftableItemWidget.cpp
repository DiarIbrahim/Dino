// Code By : Diar Ibrahim,  Contact :  https://www.linkedin.com/in/diar-ibrahim-ali/


#include "DinoInventoryCraftableItemWidget.h"

#include "Components/Button.h"


void UDinoInventoryCraftableItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(IsValid(Button))
	{
		Button->OnPressed.AddDynamic(this, &UDinoInventoryCraftableItemWidget::Select);
	}
}

void UDinoInventoryCraftableItemWidget::SetCraftableItemData(const FDinoInventoryItemData& InCraftableItemData)
{
	if(InCraftableItemData.CraftingData.CraftingPolicy != EDinoInventoryItemCraftingPolicy::Craftable) return;
	
	CraftableItemData = InCraftableItemData;

	// request to refresh
	BP_OnCraftableDataSet(CraftableItemData);
	
}

void UDinoInventoryCraftableItemWidget::Select()
{
	if(bSelected == true) return;

	
	bSelected = true;
	BP_Select();

	OnSelected.Broadcast(this);
	
}

void UDinoInventoryCraftableItemWidget::Deselect()
{
	if(bSelected == false) return;
	
	bSelected = false;
	BP_Deselect();
}
