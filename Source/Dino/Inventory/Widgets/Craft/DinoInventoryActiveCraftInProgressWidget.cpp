// Code By : Diar Ibrahim,  Contact :  https://www.linkedin.com/in/diar-ibrahim-ali/


#include "DinoInventoryActiveCraftInProgressWidget.h"

#include "Dino/Inventory/DinoInventoryComponent.h"
#include "Dino/Inventory/Craft/DinoInventoryCraftWorker.h"

void UDinoInventoryActiveCraftInProgressWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(IsValid(OwningInventoryComponent) && WorkerData.IsValid())
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

void UDinoInventoryActiveCraftInProgressWidget::SetCraftWorker(UDinoInventoryComponent* InInventoryComponent,
	FDinoInventoryCraftWorker InWorker)
{
	OwningInventoryComponent = InInventoryComponent;
	WorkerData = InWorker;

	
	if(IsValid(OwningInventoryComponent) && WorkerData.IsValid())
	{
		InitCraftingWidget();
	}
}



void UDinoInventoryActiveCraftInProgressWidget::InitCraftingWidget()
{
	
	OwningInventoryComponent->OnCraftWorkerProgress.AddDynamic(this, &UDinoInventoryActiveCraftInProgressWidget::OnCraftingProgress);
	OwningInventoryComponent->OnCraftWorkerCompleted.AddDynamic(this, &UDinoInventoryActiveCraftInProgressWidget::OnCraftingCompleted);
	OwningInventoryComponent->OnCraftWorkerCanceled.AddDynamic(this, &UDinoInventoryActiveCraftInProgressWidget::OnCraftingCanceled);
	
	BP_InitCraftingWidget(WorkerData);

}

void UDinoInventoryActiveCraftInProgressWidget::CancelCrafting()
{
	
	if(IsValid(OwningInventoryComponent))
	{
			OwningInventoryComponent->CancelCrafting(WorkerData.ItemToCraft);
	}
	
}

void UDinoInventoryActiveCraftInProgressWidget::OnCraftingProgress(const FDinoInventoryCraftWorker& Worker)
{
	if(Worker.ItemToCraft.MatchesTagExact(WorkerData.ItemToCraft))
	{
		BP_OnCraftingProgress(Worker.GetProgressPercent());
	}
}

void UDinoInventoryActiveCraftInProgressWidget::OnCraftingCompleted(const FDinoInventoryCraftWorker& Worker)
{
	if(Worker.ItemToCraft.MatchesTagExact(WorkerData.ItemToCraft))
	{
		BP_OnCraftingCompleted();
	}
}

void UDinoInventoryActiveCraftInProgressWidget::OnCraftingCanceled(const FDinoInventoryCraftWorker& Worker)
{
	if(Worker.ItemToCraft.MatchesTagExact(WorkerData.ItemToCraft))
	{
		BP_OnCraftingCanceled();
	}
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
