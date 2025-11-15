// Code By : Diar Ibrahim,  Contact :  https://www.linkedin.com/in/diar-ibrahim-ali/

#pragma once

#include "CoreMinimal.h"
#include "gameplayTags.h"
#include "Engine/DataTable.h"
#include "DinoInventoryTypes.generated.h"

UENUM(BlueprintType)
enum class EDinoInventoryItemType : uint8 {
	
	None       UMETA(DisplayName = "Undefined"),
	Consumable UMETA(DisplayName = "Consumable"),
	Equipable  UMETA(DisplayName = "Equipable")

};

UENUM(BlueprintType)
enum class EDinoInventoryCraftingType : uint8 {

	// the item can not be crafted and it only can be found and collected
	NaturalResource UMETA(DisplayName = "Natural Resourse"),
	// this item can be crafted using other items (e.g. wood and Iron to craft axe)
	Craftable       UMETA(DisplayName = "Craftable")

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
	/*
	* returns a copy of the slot that holds the item
	* returns true if item found and false if not found
	*/
	bool GetItemSlot(const FGameplayTag& InItemTag, FDinoInventorySlot& OutFoundSlot) const;
	
	// retruns items current quantity in the inventory, returns -1 if item not found
	int32 GetItemQuantity(const FGameplayTag& InItemTag) const;
	
	// Update the max slot, the maximum number of slots we can possibly have in the inventory
	void SetMaxSlotCount(int32 NewMaxSlotCount);
	// max slots
	int32 GetMaxSlotCount() const { return MaxSlotCount; }
	// update the default max item capacity for each new slot
	void SetDefaultSlotItemCapacity(int32 NewDefaultSlotItemCapacity);

	// get a const reference of the slots
	const TArray<FDinoInventorySlot>& GetSlots() const { return Slots; }

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



// Dino Inventtory Item Structure
USTRUCT(BlueprintType)
struct FDinoInventoryItemData : public FTableRowBase {
	GENERATED_BODY();


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameplayTag ItemTag = FGameplayTag();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText ItemName = {};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText ItemDescription = {};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EDinoInventoryItemType ItemType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftObjectPtr<UTexture2D> ItemImage = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ItemWeight = 1.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText ItemWeightUnit = {};

	// crafting policy for this item, items can be craftable or not
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Crafting")
	EDinoInventoryCraftingType CraftingPolicy;

	// UPROPERTY(EditAnywhere, meta = (EditCondition = "CraftingPolicy == EDinoInventoryCraftingType::Craftable"))


};