// Code By : Diar Ibrahim,  Contact :  https://www.linkedin.com/in/diar-ibrahim-ali/


#include "DinoInventoryCraftWidget.h"

#include "DinoInventoryCraftableDependencyWidget.h"
#include "DinoInventoryCraftableItemWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
#include "Dino/Inventory/DinoInventoryComponent.h"
#include "Dino/Inventory/Helpers/DinoInventoryFunctionLibrary.h"


void UDinoInventoryCraftWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// if Dino Inventory is set on spawn
	if(IsValid(OwningInventoryComponent))
	{
		RegisterWithDinoInventoryComponent(OwningInventoryComponent);
	}

	if(IsValid(CraftItemButton))
	{
		CraftItemButton->OnPressed.AddDynamic(this, &UDinoInventoryCraftWidget::StartCrafting);
	}
	
}

void UDinoInventoryCraftWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	// for in editor testing only !
	GenerateItemsForEditor();
	
}


void UDinoInventoryCraftWidget::RegisterWithDinoInventoryComponent(UDinoInventoryComponent* InventoryComponent)
{

	if(IsValid(InventoryComponent) == false) return;

	OwningInventoryComponent = InventoryComponent;
	
	OwningInventoryComponent->OnItemAdded.AddDynamic(this, &UDinoInventoryCraftWidget::OnInvenntoryItemAdded);
	OwningInventoryComponent->OnItemRemoved.AddDynamic(this, &UDinoInventoryCraftWidget::OnInvenntoryItemRemoved);
	OwningInventoryComponent->OnCraftWorkerAdded.AddDynamic(this, &UDinoInventoryCraftWidget::CraftWorkerChanged);
	OwningInventoryComponent->OnCraftWorkerCanceled.AddDynamic(this, &UDinoInventoryCraftWidget::CraftWorkerChanged);
	OwningInventoryComponent->OnCraftWorkerCompleted.AddDynamic(this, &UDinoInventoryCraftWidget::CraftWorkerChanged);
	
	TArray<FDinoInventoryItemData> CraftableItems = UDinoInventoryFunctionLibrary::GetCraftableItems();
	
	InitializeCraftableSlots(CraftableItems);
	RefreshDependency();
	
}



void UDinoInventoryCraftWidget::OnInvenntoryItemAdded(const FDinoInventorySlotContainer& SlotContainer,
                                                      const FGameplayTag& ItemTag, bool bFirstAddition)
{
	// Update dependency statistics for selected Craftable to be made
	RefreshDependency();

}

void UDinoInventoryCraftWidget::OnInvenntoryItemRemoved(const FDinoInventorySlotContainer& SlotContainer,
	const FGameplayTag& ItemTag, bool bAllRemoved)
{
	// Update dependency statistics for selected Craftable to be made
	RefreshDependency();

}

void UDinoInventoryCraftWidget::CraftWorkerChanged(const FDinoInventoryCraftWorker& Worker)
{
	// Update dependency statistics for selected Craftable to be made
	RefreshDependency();
}

void UDinoInventoryCraftWidget::GenerateItemsForEditor()
{
	
	if (IsValid(CraftableGridPanel) == false || IsValid(CraftableItemClass) == false || IsValid(CraftableItemDependencyClass) == false) return;
	
	CraftableGridPanel->ClearChildren();


	// generate craftable items
	for (size_t i = 0; i < 4; i++) {
		
		const int32 Row = i / Columns;
		const int32 Column = i % Columns;
		
		UDinoInventoryCraftableItemWidget* NewCraftableWidget = WidgetTree->ConstructWidget<UDinoInventoryCraftableItemWidget>(CraftableItemClass);
		UGridSlot* GridSlot = CraftableGridPanel->AddChildToGrid(NewCraftableWidget, Row, Column);
		GridSlot->SetPadding(CraftableGridPadding);
	}

	DependencyGridPanel->ClearChildren();

	// generate dependecies
	for (size_t i = 0; i < 3; i++) {
		
		const int32 Row = 0;
		const int32 Column = i % Columns;
		
		UDinoInventoryCraftableDependencyWidget* NewDependecyWidget = WidgetTree->ConstructWidget<UDinoInventoryCraftableDependencyWidget>(CraftableItemDependencyClass);

		// Generate random data for quantity for testing
		if(i == 0)
		{
			NewDependecyWidget->UpdateData(FDinoInventoryItemData(), 1,2);
		}else if (i == 1)
		{
			NewDependecyWidget->UpdateData(FDinoInventoryItemData(), 2,9);
		}else
		{
			NewDependecyWidget->UpdateData(FDinoInventoryItemData(), 1,0);

		}
		
		UGridSlot* GridSlot = DependencyGridPanel->AddChildToGrid(NewDependecyWidget, Row, Column);
		GridSlot->SetPadding(DependencyGridPadding);
	}


	
}

void UDinoInventoryCraftWidget::InitializeCraftableSlots(const TArray<FDinoInventoryItemData>& CraftabeItems)
{
	
	if (IsValid(CraftableGridPanel) == false || IsValid(CraftableItemClass) == false) return;
	
	CraftableGridPanel->ClearChildren();

	for (size_t i = 0; i < CraftabeItems.Num(); i++) {


		const int32 Row = i / Columns;
		const int32 Column = i % Columns;
		
		UDinoInventoryCraftableItemWidget* NewCraftableWidget = WidgetTree->ConstructWidget<UDinoInventoryCraftableItemWidget>(CraftableItemClass);
		if(IsValid(OwningInventoryComponent))
		{
			NewCraftableWidget->SetCraftableItemData(CraftabeItems[i]);
			NewCraftableWidget->OnSelected.AddDynamic(this, &UDinoInventoryCraftWidget::OnCraftableSelected);
			CraftableItemWidgets.Add(NewCraftableWidget);
		}
		
		UGridSlot* GridSlot = CraftableGridPanel->AddChildToGrid(NewCraftableWidget, Row, Column);
		GridSlot->SetPadding(CraftableGridPadding);
	}
}

void UDinoInventoryCraftWidget::RefreshDependency()
{

	// generate list of dependencies

	DependencyGridPanel->ClearChildren();

	if(IsValid(SelectedCraftableWidget) == false) return;
	
	const FDinoInventoryItemData& ItemData = SelectedCraftableWidget->GetItemData();

	const TArray<FDinoInventoryItemCraftingDependency>& Dependencies =  ItemData.CraftingData.CraftDependencies;
	
	for (size_t i = 0; i < Dependencies.Num(); i++) {
		
		FDinoInventoryItemCraftingDependency Dependency = Dependencies[i];
		
		
		FDinoInventoryItemData OutItemData;
		if(UDinoInventoryFunctionLibrary::GetDinoInventoryItemData(Dependency.ItemTag, OutItemData))
		{
			
			const int32 NumRequiredQuantity = Dependency.RequiredQuantity * CraftingQuantity;
			const int32 AvailableQuantity = OwningInventoryComponent->GetInventorySlots().ItemQuantity(Dependency.ItemTag);
			
			const int32 Row = 0;
			const int32 Column = i % Columns;
			
			UDinoInventoryCraftableDependencyWidget* NewDependecyWidget = WidgetTree->ConstructWidget<UDinoInventoryCraftableDependencyWidget>(CraftableItemDependencyClass);

			NewDependecyWidget->UpdateData(OutItemData, NumRequiredQuantity, AvailableQuantity);
			
			UGridSlot* GridSlot = DependencyGridPanel->AddChildToGrid(NewDependecyWidget, Row, Column);
			GridSlot->SetPadding(DependencyGridPadding);
		}
		
	}

	if(OwningInventoryComponent->IsItemCraftingInProgress(ItemData.ItemTag))
	{
		bCanCraftCurrentlySelectedCraftable = false;
	}else
	{
		bCanCraftCurrentlySelectedCraftable = OwningInventoryComponent->CanCraftItem(ItemData.ItemTag, CraftingQuantity);
	}

}

void UDinoInventoryCraftWidget::StartCrafting()
{
	if(IsValid(SelectedCraftableWidget) == false) return;

	const FDinoInventoryItemData& Data = SelectedCraftableWidget->GetItemData();

	OwningInventoryComponent->CraftItem(Data.ItemTag, CraftingQuantity);
}

void UDinoInventoryCraftWidget::OnCraftableSelected(UDinoInventoryCraftableItemWidget* NewSelectedCraftableWidget)
{

	// deselect others
	for(UDinoInventoryCraftableItemWidget* CraftableItem : CraftableItemWidgets)
	{
		if(IsValid(CraftableItem) &&  CraftableItem != NewSelectedCraftableWidget)
		{
			CraftableItem->Deselect();
		}
	}

	SelectedCraftableWidget = NewSelectedCraftableWidget;

	// request to refresh dependency
	RefreshDependency();
}
