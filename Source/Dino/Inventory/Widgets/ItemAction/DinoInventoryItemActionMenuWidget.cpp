// Code By : Diar Ibrahim,  Contact :  https://www.linkedin.com/in/diar-ibrahim-ali/


#include "DinoInventoryItemActionMenuWidget.h"

#include "DinoInventoryItemActionWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/MenuAnchor.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Dino/Inventory/DinoInventoryComponent.h"


void UDinoInventoryItemActionMenuWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	if(IsDesignTime())
	{
		GenerateActionList(TestActionData);
	}
	
}

void UDinoInventoryItemActionMenuWidget::NativeOnMouseEnter(const FGeometry& InGeometry,
	const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	GetWorld()->GetTimerManager().ClearTimer(MouseLostTimerHandle);
}

void UDinoInventoryItemActionMenuWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	GetWorld()->GetTimerManager().SetTimer(MouseLostTimerHandle, this, &UDinoInventoryItemActionMenuWidget::HideActionMenu, WaitDelayBeforeClose);
}

void UDinoInventoryItemActionMenuWidget::NativeOnMouseCaptureLost(const FCaptureLostEvent& CaptureLostEvent)
{
	Super::NativeOnMouseCaptureLost(CaptureLostEvent);
	GetWorld()->GetTimerManager().SetTimer(MouseLostTimerHandle, this, &UDinoInventoryItemActionMenuWidget::HideActionMenu, WaitDelayBeforeClose);

}

void UDinoInventoryItemActionMenuWidget::HideActionMenu()
{
	GetWorld()->GetTimerManager().ClearTimer(MouseLostTimerHandle);
	if(IsValid(MenuAnchor))
	{
		MenuAnchor->Close();
	}
}

void UDinoInventoryItemActionMenuWidget::Init(UDinoInventoryComponent* OwningComponent, UMenuAnchor* OwningMenuAnchor,  const FDinoInventoryItemData& ItemData)
{

	ItemTag = ItemData.ItemTag;
	ActionData = ItemData.ItemActionData;

	InventoryComponent = OwningComponent;
	MenuAnchor = OwningMenuAnchor;
	
	GenerateActionList(ActionData);

	

}

void UDinoInventoryItemActionMenuWidget::GenerateActionList(const FDinoInventoryItemActionContainer& Data)
{
	
	if(IsValid(ActionListBox) == false) return;

	ActionListBox->ClearChildren();

	// generate all actions and add them to the vertical box

	for(const auto& Pair : Data.Actions)
	{
		UDinoInventoryItemActionWidget* NewActionWidget = WidgetTree->ConstructWidget<UDinoInventoryItemActionWidget>(ItemActionWidgetClass);
		NewActionWidget->SetActionData(Pair.Key, Pair.Value);
		
		NewActionWidget->OnItemPressed.AddDynamic(this, &UDinoInventoryItemActionMenuWidget::HandleItemActionPressed);
		
		UVerticalBoxSlot* NewSlot = ActionListBox->AddChildToVerticalBox(NewActionWidget);
		NewSlot->SetPadding(ActionPadding);
		
	}
	
}

void UDinoInventoryItemActionMenuWidget::HandleItemActionPressed( UDinoInventoryItemActionWidget* DinoInventoryItemActionWidget, const FGameplayTag& ActionTag)
{

	// send the request to the inventory component

	if(IsValid(InventoryComponent))
	{
		InventoryComponent->HandleInventoryItemAction(ItemTag, ActionTag);
	}

	// close menu
	if(bCloseWhenAnyItemActionPressed)
	{
		HideActionMenu();
	}
}
