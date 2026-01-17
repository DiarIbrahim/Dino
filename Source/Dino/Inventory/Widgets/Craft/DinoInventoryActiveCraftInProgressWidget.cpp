// Code By : Diar Ibrahim,  Contact :  https://www.linkedin.com/in/diar-ibrahim-ali/


#include "DinoInventoryActiveCraftInProgressWidget.h"

#include "Dino/Inventory/DinoInventoryComponent.h"
#include "Dino/Inventory/Craft/DinoInventoryCraftWorker.h"

void UDinoInventoryActiveCraftInProgressWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(IsValid(CraftWorker))
	{
		InitCraftingWidget();
	}
}

void UDinoInventoryActiveCraftInProgressWidget::NativeOnMouseEnter(const FGeometry& InGeometry,
	const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	if(bAllowCanceling && !bIsCancelButtonShown)
	{
		ShowCancelButton();
	}
	
}

void UDinoInventoryActiveCraftInProgressWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	
	if(bAllowCanceling && bIsCancelButtonShown)
	{
		HideCancelButton();
	}
	
}

void UDinoInventoryActiveCraftInProgressWidget::NativeOnMouseCaptureLost(const FCaptureLostEvent& CaptureLostEvent)
{
	Super::NativeOnMouseCaptureLost(CaptureLostEvent);

	if(bAllowCanceling && bIsCancelButtonShown)
	{
		HideCancelButton();
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

void UDinoInventoryActiveCraftInProgressWidget::CancelCrafting()
{
	if(IsValid(CraftWorker))
	{
		UDinoInventoryComponent* InventoryComponent = CraftWorker->GetOwningComponent();
		if(IsValid(InventoryComponent))
		{
			InventoryComponent->CancelCrafting(CraftWorker->GetCraftingItem());
		}
	}
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

void UDinoInventoryActiveCraftInProgressWidget::ShowCancelButton()
{
	bIsCancelButtonShown = true;
	BP_OnShowCancelButton();
}

void UDinoInventoryActiveCraftInProgressWidget::HideCancelButton()
{
	bIsCancelButtonShown = false;
	BP_OnHideCancelButton();
}
