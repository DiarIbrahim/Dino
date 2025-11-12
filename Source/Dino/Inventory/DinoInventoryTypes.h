// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "gameplayTags.h"
#include "DinoInventoryTypes.generated.h"

UENUM(BlueprintType)
enum class EDinoInventoryItemType : uint8 {
	
	None UMETA(DisplayName = "Undefined"),
	Consumable UMETA(DisplayName = "Consumable"),
	Equipable UMETA(DisplayName = "Equipable")

};


// a slot represents an item that can be added to the inventory system
USTRUCT(BlueprintType)
struct FDinoInventorySlot
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameplayTag ItemTag = FGameplayTag();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 ItemQuantity = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 ItemCapacity = 10;

	bool IsSlotFull() const { return ItemQuantity == ItemCapacity;}

	bool operator==(const FDinoInventorySlot& Other) const {
		return ItemTag.MatchesTagExact(Other.ItemTag);
	}

	friend uint32 GetTypeHash(const FDinoInventorySlot& Other) {
		return GetTypeHash(Other.ItemTag.GetTagName());
	}

};

// list of slots that are available in the inventory
USTRUCT(BlueprintType)
struct FDinoInventorySlotContainer
{
	GENERATED_BODY()

public:

	FDinoInventorySlotContainer() {}
	FDinoInventorySlotContainer(int32 InMaxSlotCount, int32 InDefaultSlotItemCapacity) : MaxSlotCount(InMaxSlotCount), DefaultSlotItemCapacity(InDefaultSlotItemCapacity) {}


	// checks if an item is already added to one of the slots.
	bool ContainsItem(const FGameplayTag& InItemTag) const;
	// to add item to the inventory
	bool AddItem(const FGameplayTag& InItemTag, int32 QuantityToAdd = 1);
	// to remove a slot, if QuantityToRemove == -1 we remove the item, if not we subtract the quantity
	bool RemoveItem(const FGameplayTag& InItemTag, int32 QuantityToRemove = -1);
	// get index of the slot that holds an item with InItemTag
	int32 GetItemSlotIndex(const FGameplayTag& InItemTag) const;
	
	// Update the max slot, the maximum number of slots we can possibly have in the inventory
	void SetMaxSlotCount(int32 NewMaxSlotCount);
	// update the default max item capacity for each new slot
	void SetDefaultSlotItemCapacity(int32 NewDefaultSlotItemCapacity);

	// get a const reference of the slots
	const TArray<FDinoInventorySlot>& GetSlots() { return Slots; }

protected:

	// max number of slots we can add to this inventory, we can have this number of different items (each with its own stack count) in the inventory
	UPROPERTY(BlueprintReadOnly)
	int32 MaxSlotCount = 9;

	// the max capacity for each slot when we create new slots
	UPROPERTY(BlueprintReadOnly)
	int32 DefaultSlotItemCapacity = 10;

	// slots
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FDinoInventorySlot> Slots;
};
