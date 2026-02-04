// Code By : Diar Ibrahim,  Contact :  https://www.linkedin.com/in/diar-ibrahim-ali/


#include "DinoInventoryItemActionScript.h"

#include "Dino/Inventory/DinoInventoryComponent.h"


void UDinoInventoryItemActionScript::InitAction(UDinoInventoryComponent* InInventoryComponent,const FGameplayTag& InItemTag)
{
	if(IsValid(InInventoryComponent) == false) return;
	
	OwningInventoryComponent = InInventoryComponent;
	OwningItemTag = InItemTag;
	
	OnActionStarted(OwningInventoryComponent, OwningItemTag);
}

void UDinoInventoryItemActionScript::EndAction()
{
	// Do Clean up
	OnActionEnded();

	// notify owning component
	if(IsValid(OwningInventoryComponent))
	{
		OwningInventoryComponent->HandleInventoryItemActionScriptEnded(this);
	}

	OwningInventoryComponent = nullptr;
}

void UDinoInventoryItemActionScript::OnActionStarted_Implementation(UDinoInventoryComponent* InventoryComponent, const FGameplayTag& ItemTag)
{
	// do Actions here
}

void UDinoInventoryItemActionScript::OnActionEnded_Implementation()
{
	// do clean up here
}
