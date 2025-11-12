// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "gameplayTags.h"
#include "DinoInventoryTypes.generated.h"





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

// all
USTRUCT(BlueprintType)
struct FDinoInventorySlotContainer
{
	GENERATED_BODY()

public:

	FDinoInventorySlotContainer() {}

	bool ContainsItem(const FGameplayTag& InItemTag) const;
	bool AddItem(const FGameplayTag& InItemTag, int32 QuantityToAdd = 1);
	// to remove a slot, if QuantityToRemove == -1 we remove the item, if not we subtract the quantity
	bool RemoveItem(const FGameplayTag& InItemTag, int32 QuantityToRemove = -1);
	int32 GetItemSlotIndex(const FGameplayTag& InItemTag) const;
	const TArray<FDinoInventorySlot>& GetSlots() { return Slots; }

protected:

	UPROPERTY()
	int32 DefaultSlotCapacity = 10;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FDinoInventorySlot> Slots;
};
