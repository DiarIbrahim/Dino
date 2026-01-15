// Code By : Diar Ibrahim,  Contact :  https://www.linkedin.com/in/diar-ibrahim-ali/


#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Dino/Inventory/DinoInventoryTypes.h"
#include "GameplayTagContainer.h"
#include "DinoInventoryWidget.generated.h"



class UDinoInventorySlotWidget_Item_Item;
class UDinoInventoryComponent;
class UGridPanel;


UCLASS()
class DINO_API UDinoInventoryWidget : public UUserWidget
{

public:

	GENERATED_BODY()

	// inventory columns
	UPROPERTY(EditAnywhere, Category = "Dino Inventory")
	int32 InventoryColumns = 3;

	// this will be overriden by InventoryComponent's Max slot count in beginplay (used only for editor preview)
	UPROPERTY(EditAnywhere, Category = "Dino Inventory")
	int32 TestInventoryRow = 3;

	UPROPERTY(EditAnywhere, Category = "Dino Inventory")
	FMargin InventorySlotPadding = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Dino Inventory")	
	TSubclassOf<UDinoInventorySlotWidget_Item> InventorySlotClass;

	UPROPERTY(meta = (BindWidget))
	UGridPanel* InventoryGridPanel;

	// a reference to the owning inventory component
	UPROPERTY(BlueprintReadWrite, Category = "Dino Inventory", meta = (ExposeOnSpawn = true))
	UDinoInventoryComponent* OwningInventoryComponent;


	// initialize the (for cases if the component not set during the spawn)
	UFUNCTION(BlueprintCallable, Category = "Dino Inventory")
	virtual void InitializeInventoryWidget(UDinoInventoryComponent* InOwningInventory);

	// call when an item dragged so we update the occupied slots map
	virtual void UpdateInventorySlotMap(const FGameplayTag& ItemTag, UDinoInventorySlotWidget_Item* NewSlotWidget);


protected:

	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;

	// initializes and generates the inventory Gird and widgets
	void InitializeSlots(const FDinoInventorySlotContainer& SlotContainer);



	// --- Inventory Events

	UFUNCTION()
	void OnInvenntoryItemAdded(const FDinoInventorySlotContainer& SlotContainer, const FGameplayTag& ItemTag, bool bFirstAddition);

	UFUNCTION()
	void OnInvenntoryItemRemoved(const FDinoInventorySlotContainer& SlotContainer, const FGameplayTag& ItemTag, bool bAllRemoved);


	UFUNCTION(BlueprintPure)
	bool HasEmptySlot() const;
	UFUNCTION(BlueprintCallable)
	int32 GetEmptySlotIndex() const;
	UFUNCTION(BlueprintCallable)
	UDinoInventorySlotWidget_Item* GetEmptySlotToOccupy();

private:

	UPROPERTY()
	TArray<UDinoInventorySlotWidget_Item*> InventorySlotWidgets;
	UPROPERTY()
	TMap<FGameplayTag, UDinoInventorySlotWidget_Item*> OccupiedSlots;


};
