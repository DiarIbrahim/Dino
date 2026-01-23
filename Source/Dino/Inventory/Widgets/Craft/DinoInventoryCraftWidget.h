// Code By : Diar Ibrahim,  Contact :  https://www.linkedin.com/in/diar-ibrahim-ali/

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Dino/Inventory/DinoInventoryTypes.h"
#include "Dino/Inventory/Craft/DinoInventoryCraftWorker.h"
#include "DinoInventoryCraftWidget.generated.h"


class UButton;
class UDinoInventoryCraftableDependencyWidget;
class UDinoInventoryCraftableItemWidget;
class UGridPanel;

UCLASS()
class DINO_API UDinoInventoryCraftWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	
protected:

	

	UPROPERTY(EditAnywhere, Category = "Dino Inventory")
	TSubclassOf<UDinoInventoryCraftableItemWidget> CraftableItemClass;
	
	UPROPERTY(EditAnywhere, Category = "Dino Inventory")
	TSubclassOf<UDinoInventoryCraftableDependencyWidget> CraftableItemDependencyClass;
	
	
	
	UPROPERTY(EditAnywhere, Category = "Dino Inventory")
	FMargin CraftableGridPadding = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Dino Inventory")
	FMargin DependencyGridPadding = 5.0f;

	
	// inventory columns
	UPROPERTY(EditAnywhere, Category = "Dino Inventory")
	int32 Columns = 3;


	// will be true only when we have all resources available to craft the currently selected craftable item
	UPROPERTY(BlueprintReadOnly)
	bool bCanCraftCurrentlySelectedCraftable = false;

	// quantity of an item we want to craft of the selected craftable item
	UPROPERTY(BlueprintReadOnly)
	int32 CraftingQuantity = 1;

	// list of items that can be crafted
	UPROPERTY(meta = (BindWidget))
	UGridPanel* CraftableGridPanel;
	
	UPROPERTY(meta = (BindWidget))
	UGridPanel* DependencyGridPanel;

	
	// list of items that can be crafted
	UPROPERTY(meta = (BindWidget))
	UButton* CraftItemButton;

	
	// a reference to the owning inventory component
	UPROPERTY(BlueprintReadWrite, Category = "Dino Inventory", meta = (ExposeOnSpawn = true))
	UDinoInventoryComponent* OwningInventoryComponent;

	UPROPERTY()
	TArray<UDinoInventoryCraftableItemWidget*> CraftableItemWidgets;

	UPROPERTY()
	UDinoInventoryCraftableItemWidget* SelectedCraftableWidget;

public:

	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;


	UFUNCTION(BlueprintCallable, Category = "Dino Inventory")
	void RegisterWithDinoInventoryComponent(UDinoInventoryComponent* InventoryComponent);


	// generates craftabe items and dependencies for editor
	void GenerateItemsForEditor();
	
	// initializes and generates the inventory Gird and widgets
	void InitializeCraftableSlots(const TArray<FDinoInventoryItemData>& CraftabeItems);

	void RefreshDependency();

	UFUNCTION()
	void StartCrafting();
	
	UFUNCTION()
	void OnCraftableSelected(UDinoInventoryCraftableItemWidget* NewSelectedCraftableWidget);
	
	UFUNCTION()
	void OnInvenntoryItemAdded(const FDinoInventorySlotContainer& SlotContainer, const FGameplayTag& ItemTag, bool bFirstAddition);

	UFUNCTION()
	void OnInvenntoryItemRemoved(const FDinoInventorySlotContainer& SlotContainer, const FGameplayTag& ItemTag, bool bAllRemoved);

	UFUNCTION()
	void CraftWorkerChanged(const FDinoInventoryCraftWorker& Worker);

	
};
