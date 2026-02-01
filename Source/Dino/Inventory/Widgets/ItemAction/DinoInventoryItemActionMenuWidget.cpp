// Code By : Diar Ibrahim,  Contact :  https://www.linkedin.com/in/diar-ibrahim-ali/


#include "DinoInventoryItemActionMenuWidget.h"

#include "DinoInventoryItemActionWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"


void UDinoInventoryItemActionMenuWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	if(IsDesignTime())
	{
		GenerateActionList(TestActionData);
	}
	
}

void UDinoInventoryItemActionMenuWidget::SetItemData(const FDinoInventoryItemData& ItemData)
{

	ItemTag = ItemData.ItemTag;
	ActionData = ItemData.ItemActionData;

	GenerateActionList(ActionData);
	UE_LOG(LogTemp, Warning, TEXT("------- Generate Called Set Data"));

}

void UDinoInventoryItemActionMenuWidget::GenerateActionList(const FDinoInventoryItemActionContainer& Data)
{
	
	if(IsValid(ActionListBox) == false) return;

	ActionListBox->ClearChildren();

	// generate all actions and add them to the vertical box

	for(const FDinoInventoryItemActionData& ItemActionData : Data.Actions)
	{
		UDinoInventoryItemActionWidget* NewActionWidget = WidgetTree->ConstructWidget<UDinoInventoryItemActionWidget>(ItemActionWidgetClass);
		NewActionWidget->SetActionData(ItemActionData);
		
		UVerticalBoxSlot* NewSlot = ActionListBox->AddChildToVerticalBox(NewActionWidget);
		NewSlot->SetPadding(ActionPadding);
		
	}
	
}
