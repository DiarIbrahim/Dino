// Code By : Diar Ibrahim,  Contact :  https://www.linkedin.com/in/diar-ibrahim-ali/


#include "DinoInventoryComponent.h"

#include "IDetailTreeNode.h"
#include "Helpers/DinoInventoryFunctionLibrary.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UDinoInventoryComponent::UDinoInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UDinoInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UDinoInventoryComponent, InventorySlotContainer, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UDinoInventoryComponent, CraftWorkers, COND_OwnerOnly);
}

void UDinoInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// owning controller !
	OwningPawn = Cast<APawn>(GetOwner());
	
	Initialize();
}

void UDinoInventoryComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(GetOwner()->HasAuthority() && CraftWorkers.HasActiveWorker())
	{
		CraftWorkerTickTimeCounter += DeltaTime;

		if(CraftWorkerTickTimeCounter >= CraftWorkerTickInterval)
		{
			CraftWorkerTick(CraftWorkerTickInterval);
			CraftWorkerTickTimeCounter = 0.0f;
		}
	}
	
}

void UDinoInventoryComponent::Initialize()
{
	// initialize the inventory
	InventorySlotContainer = FDinoInventorySlotContainer(DefaultInventorySlotCount, DefaultSlotItemCapacity);
}

void UDinoInventoryComponent::AddItemToInventory(FGameplayTag ItemTag, int32 Quantity)
{
	
	if (GetOwner()->HasAuthority()) {
		// we are server
		AddItemToInventory_Internal(ItemTag, Quantity);
	}
	else if (GetOwnerRole() == ROLE_AutonomousProxy){
		// we have no authority, ask server
		Server_AddItemToInventory(ItemTag, Quantity);
	}

}

void UDinoInventoryComponent::Server_AddItemToInventory_Implementation(const FGameplayTag& ItemTag, int32 Quantity)
{
	AddItemToInventory_Internal(ItemTag, Quantity);
}

bool UDinoInventoryComponent::AddItemToInventory_Internal(const FGameplayTag& ItemTag, int32 Quantity)
{

	// should be called by server only
	if(GetOwner()->HasAuthority() == false) return false;
	

	const bool bItemAlreadyExist = InventorySlotContainer.ContainsItem(ItemTag);

	if (InventorySlotContainer.AddItem(ItemTag, Quantity)) {
		OnItemAdded.Broadcast(InventorySlotContainer, ItemTag, !bItemAlreadyExist);
		return true;
	}

	return false;
}

void UDinoInventoryComponent::RemoveItemFromInventory(FGameplayTag ItemTag, int32 Quantity)
{

	// networking
	if (GetOwner()->HasAuthority()) {
		// we are the server, multi cast
		RemoveItemFromInventory_Internal(ItemTag, Quantity);
	}
	else if (GetOwnerRole() == ROLE_AutonomousProxy){
		// ask server
		Server_RemoveItemFromInventory(ItemTag, Quantity);
	}
	
}

void UDinoInventoryComponent::Server_RemoveItemFromInventory_Implementation(const FGameplayTag& ItemTag, int32 Quantity)
{
	RemoveItemFromInventory_Internal(ItemTag, Quantity);
}


bool UDinoInventoryComponent::RemoveItemFromInventory_Internal(const FGameplayTag& ItemTag, int32 QuantityToRemove)
{
	// should be called by server only
	if(GetOwner()->HasAuthority() == false) return false;
	
	int32 CurrentQuantity = InventorySlotContainer.GetItemQuantity(ItemTag);
	const bool bRemoveAll = QuantityToRemove == -1 ? true : CurrentQuantity <= QuantityToRemove;

	if (InventorySlotContainer.RemoveItem(ItemTag, QuantityToRemove)) {
		OnItemRemoved.Broadcast(InventorySlotContainer, ItemTag, bRemoveAll);
		return true;
	}
	return false;
}

void UDinoInventoryComponent::CraftItem(FGameplayTag ItemTag, int32 QuantityToCraft)
{

	// networking
	if (GetOwner()->HasAuthority()) {
		// we are the server, start crafting !
		CraftItem_Internal(ItemTag, QuantityToCraft);
	}
	else if (GetOwnerRole() == ROLE_AutonomousProxy){
		// ask server
		Server_CraftItem(ItemTag, QuantityToCraft);
	}
	
}

void UDinoInventoryComponent::Server_CraftItem_Implementation(const FGameplayTag& ItemTag, int32 QuantityToCraft)
{
	CraftItem_Internal(ItemTag, QuantityToCraft);
}

bool UDinoInventoryComponent::CraftItem_Internal(const FGameplayTag& ItemTag, int32 QuantityToCraft)
{
	// should be called by server only
	if(GetOwner()->HasAuthority() == false) return false;
	
	if(ItemTag.IsValid() == false) return false;

	// this item is in crafting progress, can not add more Worker !
	if(IsItemCraftingInProgress(ItemTag)) return false;


	// crafting denied
	if(CanCraftItem(ItemTag) == false) return false;
	
	// lock dependencies
	if(LockCraftingDependencyForItem(ItemTag, QuantityToCraft))
	{

		FDinoInventoryItemData OutItemData;
		UDinoInventoryFunctionLibrary::GetDinoInventoryItemData(ItemTag,OutItemData);
		
		const float CraftDuration = OutItemData.CraftingData.CraftingDuration * QuantityToCraft;
	
		FDinoInventoryCraftWorker NewWorker = FDinoInventoryCraftWorker(ItemTag, QuantityToCraft, CraftDuration);
		
		OnCraftWorkerAdded.Broadcast(NewWorker);
		CraftWorkers.AddWorker(NewWorker);
		return true;
	}

	return false;
}

bool UDinoInventoryComponent::CanCraftItem(const FGameplayTag& ItemTag, int32 QuantityToCraft)
{
	// this runs on authority
	FDinoInventoryItemData OutItemData;
	if(UDinoInventoryFunctionLibrary::GetDinoInventoryItemData(ItemTag,OutItemData))
	{

		// this is not a craftable item
		if(OutItemData.CraftingData.CraftingPolicy !=EDinoInventoryItemCraftingPolicy::Craftable) return false;

		// there is no dependency which means this item is free to make, do not allow this.
		if(OutItemData.CraftingData.CraftDependencies.IsEmpty()) return false;
		
		for(FDinoInventoryItemCraftingDependency Dependency  : OutItemData.CraftingData.CraftDependencies)
		{
			// make sure we have the dependency with right Quantity
			if(InventorySlotContainer.ContainsItem(Dependency.ItemTag) == false) return false;

			// quantity 
			if(InventorySlotContainer.ItemQuantity(Dependency.ItemTag) < Dependency.RequiredQuantity * QuantityToCraft) return false;
		}

		// return true when all dependencies are available
		return true;
	}
	return false;
}

bool UDinoInventoryComponent::IsItemCraftingInProgress(const FGameplayTag& ItemTag)
{
	if(FDinoInventoryCraftWorker* Worker = CraftWorkers.FindItemWorker(ItemTag))
	{
		// there is an active progress for this item that is not completed yet
		return Worker->IsCompleted() == false;
	}

	return false;
}

float UDinoInventoryComponent::GetCraftProgressForItem(const FGameplayTag& ItemTag)
{
	if(const FDinoInventoryCraftWorker* ActiveWorker = CraftWorkers.FindItemWorker(ItemTag))
	{
		return ActiveWorker->GetProgressPercent();
	}
	return 0.0f;
}

FDinoInventoryCraftWorker UDinoInventoryComponent::GetActiveCraftWorkerData(const FGameplayTag& ItemTag)
{
	if(const FDinoInventoryCraftWorker* ActiveWorker = CraftWorkers.FindItemWorker(ItemTag))
	{
		return *ActiveWorker;
	}
	return FDinoInventoryCraftWorker();
}

void UDinoInventoryComponent::CancelCrafting(FGameplayTag ItemTag)
{
	if(GetOwner()->HasAuthority())
	{
		CancelCrafting_Internal(ItemTag);
	}else if (GetOwnerRole() == ROLE_AutonomousProxy)
	{
		Server_CancelCrafting(ItemTag);
	}
}

void UDinoInventoryComponent::Server_CancelCrafting_Implementation(const FGameplayTag& ItemTag)
{
	CancelCrafting_Internal(ItemTag);
}

bool UDinoInventoryComponent::CancelCrafting_Internal(const FGameplayTag& ItemTag)
{
	// should be called by server only
	if(GetOwner()->HasAuthority() == false) return false;

	if(FDinoInventoryCraftWorker* Worker = CraftWorkers.FindItemWorker(ItemTag))
	{
		
		FDinoInventoryCraftWorker WorkerData = *Worker;

		// allow cancel if only crafting is not close to be completed
		if(WorkerData.WillCompletedNextTick(CraftWorkerTickInterval) == false)
		{
			// release resources
			ReleaseCraftingDependencyForItem(WorkerData.ItemToCraft, WorkerData.QuantityToCraft);
			// cancel this
			CraftWorkers.RemoveWorker(WorkerData);
			// cancel
			OnCraftWorkerCanceled.Broadcast(WorkerData);
		}
	}
	return false;
	
}

void UDinoInventoryComponent::CraftWorkerTick(float DeltaTime)
{
	TArray<FDinoInventoryCraftWorker> CompletedWorkers;
	
	for (FDinoInventoryCraftWorker& Worker  : CraftWorkers.Items)
	{
		// add progress
		Worker.Progress+=DeltaTime;

		OnCraftWorkerProgress.Broadcast(Worker);

		if(Worker.IsCompleted())
		{
			// Add worker's data
			AddItemToInventory(Worker.ItemToCraft, Worker.QuantityToCraft);
			// Pin worker to be removed later
			CompletedWorkers.Add(Worker);
			// 
			OnCraftWorkerCompleted.Broadcast(Worker);
		}
	}

	// remove workers that are completed
	for(FDinoInventoryCraftWorker& Worker : CompletedWorkers)
	{
		CraftWorkers.RemoveWorker(Worker);
	}
}

bool UDinoInventoryComponent::IsLocallyControlled() const
{
	if(IsValid(OwningPawn))
	{
		return OwningPawn->IsLocallyControlled();
	}

	return false;
}


bool UDinoInventoryComponent::LockCraftingDependencyForItem(const FGameplayTag& ItemTag, int32 Quantity)
{
	// if called on clients
	if( GetOwner()->HasAuthority() == false) return false;

	// get item data
	FDinoInventoryItemData OutItemData;
	if(UDinoInventoryFunctionLibrary::GetDinoInventoryItemData(ItemTag,OutItemData) == false) return false;
	// not craftable
	if(OutItemData.CraftingData.CraftingPolicy != EDinoInventoryItemCraftingPolicy::Craftable) return false;
	
	for(FDinoInventoryItemCraftingDependency Dependency  : OutItemData.CraftingData.CraftDependencies)
	{
		const int32 QuantityToLock = Dependency.RequiredQuantity * Quantity; 
		
		// is valid dependency
		if(Dependency.ItemTag.IsValid() == false || QuantityToLock < 1) return false;
		// check if we have dependency
		if(InventorySlotContainer.ContainsItem(Dependency.ItemTag) == false) return false;
		// quantity
		if(InventorySlotContainer.ItemQuantity(Dependency.ItemTag)  < QuantityToLock) return false;
		
		// try Remove the dependency from the component using the internal function to allow delegates to fire 
		 RemoveItemFromInventory_Internal(Dependency.ItemTag , QuantityToLock);
	}

	return true;
}

bool UDinoInventoryComponent::ReleaseCraftingDependencyForItem(const  FGameplayTag& ItemTag, int32 Quantity)
{
	// if called on clients
	if(GetOwner()->HasAuthority() == false) return false;
	// get item data
	FDinoInventoryItemData OutItemData;
	if(UDinoInventoryFunctionLibrary::GetDinoInventoryItemData(ItemTag,OutItemData) == false) return false;
	// not craftable
	if(OutItemData.CraftingData.CraftingPolicy != EDinoInventoryItemCraftingPolicy::Craftable) return false;
	

	for(FDinoInventoryItemCraftingDependency Dependency  : OutItemData.CraftingData.CraftDependencies)
	{
		const int32 QuantityToRelease = Dependency.RequiredQuantity * Quantity;
		
		// is valid dependency
		if(Dependency.ItemTag.IsValid() == false || Dependency.RequiredQuantity < 1) return false;
		// try Add the dependency to the component using the internal function to allow delegates to fire 
		AddItemToInventory_Internal(Dependency.ItemTag ,QuantityToRelease);
	}
	return true;
}


void UDinoInventoryComponent::OnRep_Inventory()
{

	if(IsLocallyControlled())
	{
		// detect if an item added (or stack count changed for existing items)
		for (const FDinoInventorySlot& Item : InventorySlotContainer.GetSlots()) // assuming Items is TArray<UDinoInventoryItem*>
		{
			if (!PreviousInventoryItems.ContainsItem(Item.ItemTag))
			{
				// New item added
				OnItemAdded.Broadcast(InventorySlotContainer, Item.ItemTag, true);
			}else
			{
				// check if Quantity changed
				FDinoInventorySlot FoundItem;
				if(PreviousInventoryItems.GetItemSlot(Item.ItemTag, FoundItem))
				{
					if(Item.ItemQuantity > FoundItem.ItemQuantity)
					{
						// the quantity of this item is increased !
						OnItemAdded.Broadcast(InventorySlotContainer, Item.ItemTag, false);
					}else if (Item.ItemQuantity < FoundItem.ItemQuantity)
					{
						OnItemRemoved.Broadcast(InventorySlotContainer, Item.ItemTag, false);
					}
				}
			}
		}

		// Detect removed items
		for (const FDinoInventorySlot& OldItem : PreviousInventoryItems.GetSlots())
		{
			if (OldItem.IsValidSlot() && !InventorySlotContainer.ContainsItem(OldItem.ItemTag))
			{
				// Item removed
				OnItemRemoved.Broadcast(InventorySlotContainer, OldItem.ItemTag, true);
			}
		}

		// Update snapshot
		PreviousInventoryItems = FDinoInventorySlotContainer();
		for (const FDinoInventorySlot& Item : InventorySlotContainer.GetSlots())
		{
			PreviousInventoryItems.AddItem(Item.ItemTag, Item.ItemQuantity);
		}
	}
	
}

void UDinoInventoryComponent::OnRep_CraftedWorkers()
{

	if(IsLocallyControlled())
	{
		for (const FDinoInventoryCraftWorker& Worker : CraftWorkers.Items)
		{
			if (!PreviousCraftWorkers.WorkerExists(Worker))
			{
				// New worker added
				OnCraftWorkerAdded.Broadcast(Worker);
			}
		}

		// Detect removed workers
		for (const FDinoInventoryCraftWorker& OldWorker : PreviousCraftWorkers.Items)
		{
			if (OldWorker.IsValid() && !CraftWorkers.WorkerExists(OldWorker))
			{
				if(OldWorker.WillCompletedNextTick(CraftWorkerTickInterval))
				{
					// worker completed
					OnCraftWorkerCompleted.Broadcast(OldWorker);
				}else
				{
					// Worker removed 
					OnCraftWorkerCanceled.Broadcast(OldWorker);	
				}
				
			}else
			{
				// workers that not removed yet, are in progress
				OnCraftWorkerProgress.Broadcast(OldWorker);
			}
		}
	
		PreviousCraftWorkers.Clear();
		for (const FDinoInventoryCraftWorker& Worker : CraftWorkers.Items)
		{
			PreviousCraftWorkers.AddWorker(Worker);
		}
	}
	
}
