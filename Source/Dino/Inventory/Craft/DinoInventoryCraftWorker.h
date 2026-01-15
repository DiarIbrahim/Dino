// Code By : Diar Ibrahim,  Contact :  https://www.linkedin.com/in/diar-ibrahim-ali/

#pragma once

#include "CoreMinimal.h"
#include "Dino/Inventory/DinoInventoryTypes.h"
#include "UObject/Object.h"
#include "DinoInventoryCraftWorker.generated.h"

class UDinoInventoryComponent;



USTRUCT(BlueprintType)
struct FDinoInventoryCraftWorkerData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag ItemToCraft = {};

	UPROPERTY(BlueprintReadOnly)
	int32 QuantityToCraft = 1;
	
	UPROPERTY(BlueprintReadOnly)
	bool bResourcesLocked = false;

	UPROPERTY(BlueprintReadOnly)
	float OverAllProgress = 0.0f;

};


/**
 *   this object is responsible for crafting craftable items in DinoInventoryComponent
 *   this will lock Resources needed for crafting a specific item, will add the crafted item to the Inventory when completed.
 */
UCLASS()
class DINO_API UDinoInventoryCraftWorker : public UObject
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDinoInventoryCraftWorkerDestroyDelegate, UDinoInventoryCraftWorker*, Worker);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDinoInventoryCraftWorkerDelegate);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDinoInventoryCraftWorkerProgressDelegate, float , Progress);
	
public:
	
	UPROPERTY(BlueprintAssignable)
	FDinoInventoryCraftWorkerDelegate OnCompleted;
	UPROPERTY(BlueprintAssignable)
	FDinoInventoryCraftWorkerDelegate OnCanceled;
	UPROPERTY(BlueprintAssignable)
	FDinoInventoryCraftWorkerProgressDelegate OnProgress;

	UPROPERTY(BlueprintAssignable)
	FDinoInventoryCraftWorkerDestroyDelegate OnDestroy;

	
protected:
	
	UPROPERTY(BlueprintReadOnly)
	UDinoInventoryComponent* OwningComponent;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_WorkerData)
	FDinoInventoryCraftWorkerData WorkerData;
	
	UPROPERTY(BlueprintReadOnly)
	FDinoInventoryItemCraftingData ItemCraftData;
	
	UPROPERTY(BlueprintReadOnly)
	FTimerHandle WorkerTickTimerHandle;


	// this is over all time passed since we started crafting, this is sever-only value and only changing on server
	float TimeProgress = 0.0f;

public:

	
	// start crafting an item
	virtual void StartCrafting(UDinoInventoryComponent* InOwningComponent,const FGameplayTag& InItemTag, int32 InQuantityToCraft);
	// called when we explicitly cancel a crafting worker
	virtual void CancelCrafting();

	UFUNCTION(Client, Reliable)
	void Client_CancelCrafting();

	UFUNCTION(BlueprintPure)
	FGameplayTag GetCraftingItem() const;

	UFUNCTION(BlueprintPure)
	int32 GetRemainingTimeInSeconds() const;

	UFUNCTION(BlueprintPure)
	int32 GetCraftingItemQuantity() const {return WorkerData.QuantityToCraft;};

	UFUNCTION(BlueprintPure)
	bool IsCompleted() const;
	
	UFUNCTION(BlueprintPure)
	float GetProgress() const;
	
protected:

	// tick
	void OnTick_Internal();
	virtual void WorkerTick(float DeltaTime);
	float GetTickInterval() const;

	
	// called when crafting successfully completed and crafted item added to the inventory
	virtual void CraftingCompleted();
	// called when worker is about to destroy
	virtual void DestroyWorker();

	// to release the resources when the crafting is failing or cancels
	void ReleaseResources();

	UFUNCTION()
	void OnRep_WorkerData();
	

	
};
