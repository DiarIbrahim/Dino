// Code By : Diar Ibrahim,  Contact :  https://www.linkedin.com/in/diar-ibrahim-ali/


#include "DinoInventoryItemActionWidget.h"

void UDinoInventoryItemActionWidget::SetActionData(const FDinoInventoryItemActionData& InInventoryItemAction)
{
	InventoryItemActionData = InInventoryItemAction;

	BP_OnActionDataSet(InventoryItemActionData);
}
