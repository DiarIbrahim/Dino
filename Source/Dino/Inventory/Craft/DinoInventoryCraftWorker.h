// Code By : Diar Ibrahim,  Contact :  https://www.linkedin.com/in/diar-ibrahim-ali/

#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "UObject/Object.h"
#include "GameplayTagContainer.h"
#include "DinoInventoryCraftWorker.generated.h"

class UDinoInventoryComponent;


USTRUCT(BlueprintType)
struct FDinoInventoryCraftWorker : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FDinoInventoryCraftWorker(){}
	FDinoInventoryCraftWorker(const FGameplayTag&  InItemTag, const int32 InQuantityToCraft, float InDuration) : ItemToCraft(InItemTag), QuantityToCraft(InQuantityToCraft), Duration(InDuration){}

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag ItemToCraft;
	
	UPROPERTY(BlueprintReadOnly)
	int32 QuantityToCraft = 1;
	
	UPROPERTY(BlueprintReadOnly)
	bool bResourcesLocked = false;

	// over all progress for this worker (time spent working on crafting this item)
	UPROPERTY(BlueprintReadOnly)
	float Progress = 0.0f;

	// over all duration to craft @QuantityToCraft amount of this item
	UPROPERTY(BlueprintReadOnly)
	float Duration = 0.0f;


	float GetProgressPercent() const;
	// if this item is already completed
	bool IsCompleted() const;
	// if this item will completed next tick (we need this to deny request to cancel items when they are just very close to be completed)
	bool WillCompletedNextTick(float TickInterval = 1.f/10.f) const;
	bool IsValid() const;
	

	FORCEINLINE bool operator ==(const FDinoInventoryCraftWorker& Other) const
	{
		return ItemToCraft.MatchesTagExact(Other.ItemToCraft);
	}

	FORCEINLINE friend int32 GetTypeHash(const FDinoInventoryCraftWorker& InventoryCraftWorker)
	{
		return GetTypeHash(InventoryCraftWorker.ItemToCraft);
	}
};


USTRUCT(BlueprintType)
struct FDinoInventoryCraftWorkerContainer : public FFastArraySerializer
{
	GENERATED_BODY()
	
	bool AddWorker(FDinoInventoryCraftWorker NewWorker);
	bool RemoveWorker(const FDinoInventoryCraftWorker& Worker);
	bool WorkerExists(const FDinoInventoryCraftWorker& Worker);
	bool WorkerExists(const FGameplayTag& Worker);
	bool HasActiveWorker() const;
	FDinoInventoryCraftWorker* FindItemWorker(const FGameplayTag& InItemTag);
	
	void Clear();

	void ClearAll();

	
	UPROPERTY(BlueprintReadOnly)
	TArray<FDinoInventoryCraftWorker> Items;
};



