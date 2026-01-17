// Code By : Diar Ibrahim,  Contact :  https://www.linkedin.com/in/diar-ibrahim-ali/

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "DinoInventoryTypes.h"
#include "Craft/DinoInventoryCraftWorker.h"
#include "DinoInventoryComponent.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnInventoryItemAddedDelegate, const FDinoInventorySlotContainer&, Slots, const FGameplayTag&, NewItemTag, bool, bIsFirstAddition);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnInventoryItemRemovedDelegate, const FDinoInventorySlotContainer&, Slots, const FGameplayTag&, RemovedItemTag, bool, bAllStacksRemoved);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventorySlotCountUpdatedDelegate, int32, NewInventorySlotCount);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryCraftWorkerDelegate, UDinoInventoryCraftWorker*, Worker);


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


	// a new item added to the inventory (or an existing item's quantity added)
	UPROPERTY(BlueprintAssignable)
	FOnInventoryItemAddedDelegate OnItemAdded;

	// an item removed from the inventory (or an existing item's quantity subtracted)
	UPROPERTY(BlueprintAssignable)
	FOnInventoryItemRemovedDelegate OnItemRemoved;

	UPROPERTY(BlueprintAssignable)
	FOnInventoryCraftWorkerDelegate OnCraftWorkerAdded;
	UPROPERTY(BlueprintAssignable)
	FOnInventoryCraftWorkerDelegate OnCraftWorkerRemoved;
	
protected:
	
	UPROPERTY(ReplicatedUsing=OnRep_CraftedWorkers)
	TArray<UDinoInventoryCraftWorker*> CraftWorkers;

	UPROPERTY(ReplicatedUsing=OnRep_Inventory)
	FDinoInventorySlotContainer InventorySlotContainer;
	
	UPROPERTY()
	APawn* OwningPawn;


private:

	// Previous snapshot for diffing
	FDinoInventorySlotContainer PreviousInventoryItems;
	TArray<TWeakObjectPtr<UDinoInventoryCraftWorker>> PreviousCraftWorkers;


public:
	
	
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
	
	// internal use only, to add actual item to the inventory after the networking logics
	bool AddItemToInventory_Internal(const FGameplayTag& ItemTag, int32 Quantity = 1);
	
	//// ---- Add Item END

	
	//// ---- Remove Item

	// to Remove Item from the Inventory, if QuantityToRemove == -1 we remove the item regardless of the Current quantity of the item otherwise we subtract the quantity
	UFUNCTION(BlueprintCallable, Category = "Dino Inventory")
	void RemoveItemFromInventory(FGameplayTag ItemTag, int32 QuantityToRemove = -1);

	UFUNCTION(Server, Reliable, Category = "Dino Inventory")
	void Server_RemoveItemFromInventory(const FGameplayTag& ItemTag, int32 QuantityToRemove = -1);
	void Server_RemoveItemFromInventory_Implementation(const FGameplayTag& ItemTag, int32 QuantityToRemove = -1);

	// internal use only, to remove actual item from the inventory after the networking logics
	bool RemoveItemFromInventory_Internal(const FGameplayTag& ItemTag, int32 QuantityToRemove = -1);
	
	//// ---- Remove Item END


	//// ----- Craft Item

	// asks the component to start crafting an item if the dependencies of the item exist in the inventory
	UFUNCTION(BlueprintCallable, Category = "Dino Inventory")
	void CraftItem(FGameplayTag ItemTag, int32 QuantityToCraft = 1);

	UFUNCTION(Server, Reliable, Category = "Dino Inventory")
	void Server_CraftItem(const FGameplayTag& ItemTag, int32 QuantityToCraft = 1);
	void Server_CraftItem_Implementation(const FGameplayTag& ItemTag, int32 QuantityToCraft = 1);

	// returns ture if crafting started 
	bool CraftItem_Internal(const FGameplayTag& ItemTag, int32 QuantityToCraft = 1);
	
	UFUNCTION(BlueprintCallable, Category = "Dino Inventory")
	bool CanCraftItem(const FGameplayTag& ItemTag, int32 QuantityToCraft = 1);

	// Checks if we have enough of this dependency or not ? if so it will lock it (it will remove the dependency from the inventory to be used for crafting) (Only Called on Authority)
	UFUNCTION(BlueprintCallable, Category = "Dino Inventory")
	bool LockCraftingDependencyForItem(const FGameplayTag& ItemTag, int32 Quantity = 1);
	
	// releases the dependency to the component when crafting fails or canceled (Only Called on Authority)
	UFUNCTION(BlueprintCallable, Category = "Dino Inventory")
	bool ReleaseCraftingDependencyForItem(UDinoInventoryCraftWorker* CraftWorker, const FGameplayTag& ItemTag, int32 Quantity = 1);
	
	UFUNCTION(BlueprintCallable, Category = "Dino Inventory")
	bool IsItemCraftingInProgress(const FGameplayTag& ItemTag);

	// cancels and existing crafting process for an item and terminates the craft worker
	UFUNCTION(BlueprintCallable, Category = "Dino Inventory")
	void CancelCrafting(FGameplayTag ItemTag);

	UFUNCTION(Server, Reliable)
	void Server_CancelCrafting(const FGameplayTag& ItemTag);
	void Server_CancelCrafting_Implementation(const FGameplayTag& ItemTag);

	// returns true when canceling succeeded
	bool CancelCrafting_Internal(const FGameplayTag& ItemTag);

	
	UFUNCTION()
	void OnCraftWorkerDestroyed(UDinoInventoryCraftWorker* Worker);


	//// ----- Craft Item END


	
	// returns inventory  slots
	UFUNCTION(BlueprintCallable, Category = "Dino Inventory")
	FDinoInventorySlotContainer GetInventorySlots() const { return InventorySlotContainer; }
	

	// OnRep_Functions
	
	UFUNCTION()
	void OnRep_Inventory();

	UFUNCTION()
	void OnRep_CraftedWorkers();

	
};
