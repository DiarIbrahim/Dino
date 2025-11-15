// Code By : Diar Ibrahim,  Contact :  https://www.linkedin.com/in/diar-ibrahim-ali/

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "DinoInventoryTypes.h"
#include "DinoInventoryComponent.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnInventoryItemAddedDelegate, const FDinoInventorySlotContainer&, Slots, const FGameplayTag&, NewItemTag, bool, bIsFirstAddition);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnInventoryItemRemovedDelegate, const FDinoInventorySlotContainer&, Slots, const FGameplayTag&, RemovedItemTag, bool, bAllStacksRemoved);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventorySlotCountUpdatedDelegate, int32, NewInventorySlotCount);





UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DINO_API UDinoInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	// num of the slots this inventory can have, each slot can hold one item type (each item can has it's own quantity)
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Dino Inventory")
	int32 DefaultInventorySlotCount = 9;
	// item capacity, how many item can fit in one slot by default (how many stack counts of one item can fit in one slot)
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Dino Inventory")
	int32 DefaultSlotItemCapacity = 10;



	// Sets default values for this component's properties
	UDinoInventoryComponent();

	
	virtual void BeginPlay() override;


	void Initialize();

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


	// returns inventory  slots
	UFUNCTION(BlueprintCallable, Category = "Dino Inventory")
	FDinoInventorySlotContainer GetInventorySlots() const { return InventorySlotContainer; }

	// a new item added to the inventory (or an existing item's quantity added)
	UPROPERTY(BlueprintAssignable)
	FOnInventoryItemAddedDelegate OnItemAdded;

	// an item removed from the inventory (or an existing item's quantity subtracted)
	UPROPERTY(BlueprintAssignable)
	FOnInventoryItemRemovedDelegate OnItemRemoved;



private:

	FDinoInventorySlotContainer InventorySlotContainer;
	AController* OwningController;

};
