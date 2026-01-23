// Code By : Diar Ibrahim,  Contact :  https://www.linkedin.com/in/diar-ibrahim-ali/

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "Dino/Inventory/Craft/DinoInventoryCraftWorker.h"
#include "DinoInventoryActiveCraftInProgressWidget.generated.h"


class UDinoInventoryComponent;
class UDinoInventoryCraftWorker;

UCLASS()
class DINO_API UDinoInventoryActiveCraftInProgressWidget : public UUserWidget
{
	GENERATED_BODY()



protected:

	UPROPERTY(BlueprintReadOnly, meta=(ExposeOnSpawn))
	UDinoInventoryComponent* OwningInventoryComponent;
	UPROPERTY(BlueprintReadOnly, meta=(ExposeOnSpawn))
	FDinoInventoryCraftWorker WorkerData;
	

	UPROPERTY(EditAnywhere, Category="Dino Inventory|Crafting")
	bool bAllowCanceling = true;


	bool bIsCancelButtonShown = false;

public:

	virtual void NativeConstruct() override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseCaptureLost(const FCaptureLostEvent& CaptureLostEvent) override;

	UFUNCTION(BlueprintCallable)
	void SetCraftWorker(UDinoInventoryComponent* InInventoryComponent, FDinoInventoryCraftWorker InWorker);
	
	void InitCraftingWidget();

	UFUNCTION(BlueprintCallable)
	void CancelCrafting();
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName="Init Crafting Widget")
	void BP_InitCraftingWidget(const FDinoInventoryCraftWorker& Worker);
	
	UFUNCTION()
	void OnCraftingProgress(const FDinoInventoryCraftWorker& Worker);
	UFUNCTION(BlueprintImplementableEvent,  DisplayName="On Crafting On Progress")
	void BP_OnCraftingProgress(float Progress);

	UFUNCTION()
	void OnCraftingCompleted(const FDinoInventoryCraftWorker& Worker);
	UFUNCTION(BlueprintImplementableEvent,  DisplayName="Crafting Completed")
	void BP_OnCraftingCompleted();

	UFUNCTION()
	void OnCraftingCanceled(const FDinoInventoryCraftWorker& Worker);
	UFUNCTION(BlueprintImplementableEvent, DisplayName="Crafting Canceled")
	void BP_OnCraftingCanceled();


	UFUNCTION()
	void ShowCancelButton();
	UFUNCTION(BlueprintImplementableEvent, DisplayName="Show Cancel Button")
	void BP_OnShowCancelButton();

	UFUNCTION()
	void HideCancelButton();
	UFUNCTION(BlueprintImplementableEvent, DisplayName="Hide Cancel Button")
	void BP_OnHideCancelButton();

	
};
