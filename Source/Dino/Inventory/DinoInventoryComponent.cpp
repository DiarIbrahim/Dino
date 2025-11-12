// Fill out your copyright notice in the Description page of Project Settings.


#include "DinoInventoryComponent.h"


// Sets default values for this component's properties
UDinoInventoryComponent::UDinoInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UDinoInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// owning controller !
	if (APawn* OwnerPawn = Cast<APawn>(GetOwner())) {
		OwningController = OwnerPawn->GetController();
	}

}

void UDinoInventoryComponent::AddItemToInventory(FGameplayTag ItemTag, int32 Quantity)
{
	// standalone
	if (GetWorld()->GetNetMode() == NM_Standalone) {
		AddItemToInventory_Internal(ItemTag, Quantity);
	}
	else {
		// network
		if (GetOwner()->HasAuthority()) {
			// we are server
			Multi_AddItemToInventory(ItemTag, Quantity);
		}
		else {
			// we have no authority, ask server
			Server_AddItemToInventory(ItemTag, Quantity);
		}
	}
}

void UDinoInventoryComponent::Server_AddItemToInventory_Implementation(const FGameplayTag& ItemTag, int32 Quantity)
{
	Multi_AddItemToInventory(ItemTag, Quantity);
}

void UDinoInventoryComponent::Multi_AddItemToInventory_Implementation(const FGameplayTag& ItemTag, int32 Quantity)
{
	AddItemToInventory_Internal(ItemTag, Quantity);
}

void UDinoInventoryComponent::AddItemToInventory_Internal(const FGameplayTag& ItemTag, int32 Quantity)
{
	if (InventorySlotContainer.AddItem(ItemTag, Quantity)) {
		OnItemsChanged.Broadcast(InventorySlotContainer);

	}

}

void UDinoInventoryComponent::RemoveItemFromInventory(FGameplayTag ItemTag, int32 Quantity)
{
	// standalone
	if (GetWorld()->GetNetMode() == NM_Standalone) {
		RemoveItemFromInventory_Internal(ItemTag, Quantity);
	}
	else {
		// networking
		if (GetOwner()->HasAuthority()) {
			// we are the server, multi cast
			Multi_RemoveItemFromInventory(ItemTag, Quantity);
		}
		else {
			// ask server
			Server_RemoveItemFromInventory(ItemTag, Quantity);
		}
	}
}

void UDinoInventoryComponent::Server_RemoveItemFromInventory_Implementation(const FGameplayTag& ItemTag, int32 Quantity)
{
	Multi_RemoveItemFromInventory(ItemTag, Quantity);
}

void UDinoInventoryComponent::Multi_RemoveItemFromInventory_Implementation(const FGameplayTag& ItemTag, int32 Quantity)
{
	RemoveItemFromInventory_Internal(ItemTag, Quantity);
}

void UDinoInventoryComponent::RemoveItemFromInventory_Internal(const FGameplayTag& ItemTag, int32 QuantityToRemove)
{
	InventorySlotContainer.RemoveItem(ItemTag, QuantityToRemove);
	OnItemsChanged.Broadcast(InventorySlotContainer);

}
