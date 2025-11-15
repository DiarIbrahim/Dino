// Code By : Diar Ibrahim,  Contact :  https://www.linkedin.com/in/diar-ibrahim-ali/


#include "DinoInventoryDragVisual.h"
#include "Dino/Inventory/Widgets/DinoInventorySlotWidget.h"

void UDinoInventoryDragVisual::SetDragSourceSlot(UDinoInventorySlotWidget* SourceSlotWidget)
{
	if (IsValid(SourceSlotWidget) == false || IsValid(SourceSlotWidget->Item_Image) == false || IsValid(DraggedItemImage) == false) return;

	// set the brush
	DraggedItemImage->SetBrush(SourceSlotWidget->Item_Image->GetBrush());
}
