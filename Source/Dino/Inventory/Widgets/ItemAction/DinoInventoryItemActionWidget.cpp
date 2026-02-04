// Code By : Diar Ibrahim,  Contact :  https://www.linkedin.com/in/diar-ibrahim-ali/


#include "DinoInventoryItemActionWidget.h"

void UDinoInventoryItemActionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(IsValid(ActionButton))
	{
		ActionButton->OnPressed.AddDynamic(this, &UDinoInventoryItemActionWidget::ActionButtonPressed);
	}
}

void UDinoInventoryItemActionWidget::SetActionData(const FGameplayTag& InActionTag, const FDinoInventoryItemActionData& InInventoryItemAction)
{
	InventoryItemActionData = InInventoryItemAction;
	ActionTag = InActionTag;
	
	BP_OnActionDataSet(InventoryItemActionData);
}

void UDinoInventoryItemActionWidget::ActionButtonPressed()
{
	OnItemPressed.Broadcast(this, ActionTag);
}
