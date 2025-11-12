// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "DinoInventoryTypes.h"
#include "DinoInventoryComponent.generated.h"





DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryItemAddedDelegate, const FDinoInventorySlotContainer&, Items, const FGameplayTag& NewItemTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryItemRemovedDelegate, const FDinoInventorySlotContainer&, Items, const FGameplayTag& RemovedItemTag);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventorySlotCountUpdatedDelegate, int32, NewInventorySlotCount);



UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DINO_API UDinoInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDinoInventoryComponent();

	
	virtual void BeginPlay() override;

	//// ---- Add Item

	// to Add Item to the Inventory, if the item already exist we add up the quantity
	UFUNCTION(BlueprintCallable, Category = "Dino Inventory")
	void AddItemToInventory(FGameplayTag ItemTag, int32 Quantity = 1);

	UFUNCTION(Server, Reliable, Category = "Dino Inventory")
	void Server_AddItemToInventory(const FGameplayTag& ItemTag, int32 Quantity = 1);
	void Server_AddItemToInventory_Implementation(const FGameplayTag& ItemTag, int32 Quantity = 1);

	UFUNCTION(NetMulticast, Reliable, Category = "Dino Inventory")
	void Multi_AddItemToInventory(const FGameplayTag& ItemTag, int32 Quantity = 1);
	void Multi_AddItemToInventory_Implementation(const FGameplayTag& ItemTag, int32 Quantity = 1);
	
	// internal use only, to add actual item to the inventory after the networking logics
	void AddItemToInventory_Internal(const FGameplayTag& ItemTag, int32 Quantity = 1);

	//// ---- Remove Item

	// to Remove Item from the Inventory, if QuantityToRemove == -1 we remove the item regardless of the Current quantity of the item otherwise we subtract the quantity
	UFUNCTION(BlueprintCallable, Category = "Dino Inventory")
	void RemoveItemFromInventory(FGameplayTag ItemTag, int32 QuantityToRemove = -1);

	UFUNCTION(Server, Reliable, Category = "Dino Inventory")
	void Server_RemoveItemFromInventory(const FGameplayTag& ItemTag, int32 QuantityToRemove = -1);
	void Server_RemoveItemFromInventory_Implementation(const FGameplayTag& ItemTag, int32 QuantityToRemove = -1);

	UFUNCTION(NetMulticast, Reliable, Category = "Dino Inventory")
	void Multi_RemoveItemFromInventory(const FGameplayTag& ItemTag, int32 QuantityToRemove = -1);
	void Multi_RemoveItemFromInventory_Implementation(const FGameplayTag& ItemTag, int32 QuantityToRemove = -1);

	// internal use only, to remove actual item from the inventory after the networking logics
	void RemoveItemFromInventory_Internal(const FGameplayTag& ItemTag, int32 QuantityToRemove = -1);



	UPROPERTY(BlueprintAssignable)
	FOnInventoryItemsChangedDelegate OnItemsChanged;


private:
	FDinoInventorySlotContainer InventorySlotContainer;
	AController* OwningController;
};
