// Code By : Diar Ibrahim,  Contact :  https://www.linkedin.com/in/diar-ibrahim-ali/


#include "DinoInventoryActiveCraftInProgressWidget.h"

#include "Dino/Inventory/Craft/DinoInventoryCraftWorker.h"

void UDinoInventoryActiveCraftInProgressWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(IsValid(CraftWorker))
	{
		InitCraftingWidget();
	}
}

void UDinoInventoryActiveCraftInProgressWidget::SetCraftWorker(UDinoInventoryCraftWorker* InCraftWorker)
{
	if(IsValid(InCraftWorker))
	{
		CraftWorker = InCraftWorker;
		InitCraftingWidget();
	}
}

void UDinoInventoryActiveCraftInProgressWidget::InitCraftingWidget()
{
	
	CraftWorker->OnProgress.AddDynamic(this, &UDinoInventoryActiveCraftInProgressWidget::OnCraftingProgress);
	CraftWorker->OnCompleted.AddDynamic(this, &UDinoInventoryActiveCraftInProgressWidget::OnCraftingCompleted);
	CraftWorker->OnCanceled.AddDynamic(this, &UDinoInventoryActiveCraftInProgressWidget::OnCraftingCanceled);

	BP_InitCraftingWidget(CraftWorker->GetCraftingItem(), CraftWorker->GetCraftingItemQuantity());

}

void UDinoInventoryActiveCraftInProgressWidget::OnCraftingProgress(float Progress)
{
	BP_OnCraftingProgress(Progress);
}

void UDinoInventoryActiveCraftInProgressWidget::OnCraftingCompleted()
{
	BP_OnCraftingCompleted();
	// clear reference !
	CraftWorker = nullptr;
}

void UDinoInventoryActiveCraftInProgressWidget::OnCraftingCanceled()
{
	BP_OnCraftingCanceled();
	// clear reference !
	CraftWorker = nullptr;
}
