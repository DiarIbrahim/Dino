// Code By : Diar Ibrahim,  Contact :  https://www.linkedin.com/in/diar-ibrahim-ali/



#include "DinoInventoryWidget.h"
#include "Dino/Inventory/DinoInventoryComponent.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
#include "DinoInventorySlotWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Misc/UObjectToken.h"

void UDinoInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();


	if (IsValid(InventoryComponent)) {
		
		// initialize slots
		InitializeSlots(InventoryComponent->GetInventorySlots());

		// bind delegates
		InventoryComponent->OnItemAdded.AddDynamic(this, &UDinoInventoryWidget::OnInvenntoryItemAdded);
		InventoryComponent->OnItemRemoved.AddDynamic(this, &UDinoInventoryWidget::OnInvenntoryItemRemoved);

	}
}

void UDinoInventoryWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

#if WITH_EDITOR
	if (IsValid(InventorySlotClass) == false)
	{
		FMessageLog("Blueprint").Warning()
			->AddToken(FTextToken::Create(FText::FromString("Please provide a valid class for InventorySlotClass.")))
			->AddToken(FUObjectToken::Create(this));
	}
#endif


	// generate the grid for editor

	InitializeSlots(FDinoInventorySlotContainer(InventoryColumns * TestInventoryRow, 0));

}


void UDinoInventoryWidget::InitializeSlots(const FDinoInventorySlotContainer& SlotContainer)
{


	if (IsValid(InventoryGridPanel) == false || IsValid(InventorySlotClass) == false) return;
	
	InventorySlotWidgets.Empty();
	InventoryGridPanel->ClearChildren();

	for (size_t i = 0; i < SlotContainer.GetMaxSlotCount(); i++) {

		const int32 Row = i / InventoryColumns;
		const int32 Column = i % InventoryColumns;
		
		UDinoInventorySlotWidget* NewSlot = WidgetTree->ConstructWidget<UDinoInventorySlotWidget>(InventorySlotClass);
		// to ensure we know that the widget needs to be empty
		NewSlot->EmptySlotData();

		UGridSlot* GridSlot = InventoryGridPanel->AddChildToGrid(NewSlot, Row, Column);
		GridSlot->SetPadding(InventorySlotPadding);

		InventorySlotWidgets.Add(NewSlot);
	}
}

void UDinoInventoryWidget::OnInvenntoryItemAdded(const FDinoInventorySlotContainer& SlotContainer, const FGameplayTag& ItemTag, bool bFirstAddition)
{
	if (bFirstAddition) {
		// createthe widget
		UDinoInventorySlotWidget* SlotToOccupy = GetEmptySlotToOccupy();
		if (IsValid(SlotToOccupy)) {
			
			FDinoInventorySlot SlotData;
			SlotContainer.GetItemSlot(ItemTag, SlotData);

			SlotToOccupy->SetSlotData(SlotData);

			// store a reference
			OccupiedSlots.Add(ItemTag, SlotToOccupy);
		}
		else{
			// there must be an error, the delegate should never fire in this case (we do not have empty slots and a new item added, which means we might forgot to clear a slot after been removed)
		}
	}
	else if(OccupiedSlots.Contains(ItemTag)){

		if (UDinoInventorySlotWidget* SlotWidget = *OccupiedSlots.Find(ItemTag)) {

			FDinoInventorySlot SlotData;
			SlotContainer.GetItemSlot(ItemTag, SlotData);

			SlotWidget->UpdateSlotData(SlotData);
		}

	}
}

void UDinoInventoryWidget::OnInvenntoryItemRemoved(const FDinoInventorySlotContainer& SlotContainer, const FGameplayTag& ItemTag, bool bAllRemoved)
{
	// find the occupied slot and empty
	UDinoInventorySlotWidget* ItemSlotWidget = nullptr;

	if (OccupiedSlots.Contains(ItemTag)) {
		ItemSlotWidget = *OccupiedSlots.Find(ItemTag);
	}

	if (IsValid(ItemSlotWidget) == false) return;


	if (bAllRemoved) {
		ItemSlotWidget->EmptySlotData();
		// remove form the map
		OccupiedSlots.Remove(ItemTag);
		
	}
	else {

		FDinoInventorySlot SlotData;
		SlotContainer.GetItemSlot(ItemTag, SlotData);

		ItemSlotWidget->UpdateSlotData(SlotData);
	}
}

bool UDinoInventoryWidget::HasEmptySlot() const
{
	for (size_t i = 0; i < InventorySlotWidgets.Num(); i++) {
		if (IsValid(InventorySlotWidgets[i]) && InventorySlotWidgets[i]->IsValidSlotData() == false) return true;
	}

	return false;
}

int32 UDinoInventoryWidget::GetEmptySlotIndex() const
{
	for (size_t i = 0; i < InventorySlotWidgets.Num(); i++) {
		if (IsValid(InventorySlotWidgets[i]) && InventorySlotWidgets[i]->IsValidSlotData() == false) return i;
	}

	return -1;
}

UDinoInventorySlotWidget* UDinoInventoryWidget::GetEmptySlotToOccupy()
{
	for (size_t i = 0; i < InventorySlotWidgets.Num(); i++) {
		if (IsValid(InventorySlotWidgets[i]) && InventorySlotWidgets[i]->IsValidSlotData() == false) return InventorySlotWidgets[i];
	}

	return nullptr;
}
