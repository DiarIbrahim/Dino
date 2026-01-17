// Code By : Diar Ibrahim,  Contact :  https://www.linkedin.com/in/diar-ibrahim-ali/


#include "DinoInventoryCraftableDependencyWidget.h"

void UDinoInventoryCraftableDependencyWidget::UpdateData(const FDinoInventoryItemData& Item, int32 RequiredQuantity,
	int32 AvailableQuantity)
{
	// Do Cpp stuff here
	BP_OnUpdateData(Item, RequiredQuantity, AvailableQuantity);
}
