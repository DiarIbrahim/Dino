// Code By : Diar Ibrahim,  Contact :  https://www.linkedin.com/in/diar-ibrahim-ali/


#include "DinoInventoryCraftWorker.h"

#include "Dino/Inventory/DinoInventoryComponent.h"
#include "Dino/Inventory/Helpers/DinoInventoryFunctionLibrary.h"
#include "Net/UnrealNetwork.h"


void UDinoInventoryCraftWorker::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UDinoInventoryCraftWorker ,WorkerData , COND_OwnerOnly);
}


void UDinoInventoryCraftWorker::StartCrafting(UDinoInventoryComponent* InOwningComponent,const FGameplayTag& InItemTag, int32 InQuantityToCraft)
{
	// component invalid
	if(IsValid(InOwningComponent) == false)
	{
		CancelCrafting();
		return;
	}

	// worker should dbe started only on server
	if(InOwningComponent->GetOwner()->HasAuthority() == false) return;

	// item invalid
	FDinoInventoryItemData ItemData;
	if(UDinoInventoryFunctionLibrary::GetDinoInventoryItemData(InItemTag, ItemData) == false)
	{
		CancelCrafting();
		return;
	}

	ItemCraftData = ItemData.CraftingData;
	OwningComponent = InOwningComponent;
	
	WorkerData.QuantityToCraft = InQuantityToCraft;
	WorkerData.ItemToCraft = InItemTag;
	
	// start crafting
	
	OwningComponent->GetWorld()->GetTimerManager()
	.SetTimer(WorkerTickTimerHandle,this,&UDinoInventoryCraftWorker::OnTick_Internal, GetTickInterval(),true, GetTickInterval());
}

void UDinoInventoryCraftWorker::CancelCrafting()
{
	if(OwningComponent->GetOwner()->HasAuthority() == false) return;
	
	ReleaseResources();
	
	// notify on server
	OnCanceled.Broadcast();

	// notify client !
	Client_CancelCrafting();

	DestroyWorker();
}

void UDinoInventoryCraftWorker::Client_CancelCrafting_Implementation()
{
	OnCanceled.Broadcast();
}

FGameplayTag UDinoInventoryCraftWorker::GetCraftingItem() const
{
	return WorkerData.ItemToCraft;
}

int32 UDinoInventoryCraftWorker::GetRemainingTimeInSeconds() const
{
	const float OverAllDuration = ItemCraftData.CraftingDuration * WorkerData.QuantityToCraft;
	const float RemainingTime = OverAllDuration  - (OverAllDuration * GetProgress());
	return FMath::TruncToInt(RemainingTime);
}

bool UDinoInventoryCraftWorker::IsCompleted() const
{
	return WorkerData.OverAllProgress >= 1.0f;
}

float UDinoInventoryCraftWorker::GetProgress() const
{
	return WorkerData.OverAllProgress;
}

void UDinoInventoryCraftWorker::OnTick_Internal()
{
	WorkerTick(GetTickInterval());
}

void UDinoInventoryCraftWorker::WorkerTick(float DeltaTime)
{
	// Update the progress here !
	TimeProgress += DeltaTime;

	// update replicated worker data
	WorkerData.OverAllProgress = TimeProgress / (static_cast<float>(WorkerData.QuantityToCraft)  * ItemCraftData.CraftingDuration);

	// notify on server
	OnProgress.Broadcast(WorkerData.OverAllProgress);

	if(IsCompleted())
	{
		CraftingCompleted();
	}

	
}

float UDinoInventoryCraftWorker::GetTickInterval() const
{
	return ItemCraftData.CraftingDuration / static_cast<float>(ItemCraftData.CraftingDurationSteps);
}


void UDinoInventoryCraftWorker::CraftingCompleted()
{
	// crafting completed add the requested it
	if(IsValid(OwningComponent))
	{
		OwningComponent->AddItemToInventory(WorkerData.ItemToCraft, WorkerData.QuantityToCraft);
	}

	OnCompleted.Broadcast();
	
	DestroyWorker();
}

void UDinoInventoryCraftWorker::DestroyWorker()
{
	// Notify the owner of our Destraction
	OwningComponent->GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

	OnDestroy.Broadcast(this);
}

void UDinoInventoryCraftWorker::ReleaseResources()
{
	if(IsValid(OwningComponent))
	{
		OwningComponent->ReleaseCraftingDependencyForItem(this,WorkerData.ItemToCraft, WorkerData.QuantityToCraft);
	}
}

void UDinoInventoryCraftWorker::OnRep_WorkerData()
{
	// worker data replicated !
	
	if(OwningComponent->GetOwnerRole() == ROLE_AutonomousProxy)
	{
		// we are the locally controlled client, fire delegates if needed

		OnProgress.Broadcast(WorkerData.OverAllProgress);
		
		if(IsCompleted())
		{
			OnCanceled.Broadcast();
		}
	}
}
