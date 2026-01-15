// Code By : Diar Ibrahim,  Contact :  https://www.linkedin.com/in/diar-ibrahim-ali/

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "DinoInventoryActiveCraftInProgressWidget.generated.h"


class UDinoInventoryCraftWorker;

UCLASS()
class DINO_API UDinoInventoryActiveCraftInProgressWidget : public UUserWidget
{
	GENERATED_BODY()



protected:
	
	UPROPERTY(BlueprintReadOnly, meta=(ExposeOnSpawn))
	UDinoInventoryCraftWorker* CraftWorker;



public:

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void SetCraftWorker(UDinoInventoryCraftWorker* InCraftWorker);
	

	void InitCraftingWidget();
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName="Init Crafting Widget")
	void BP_InitCraftingWidget(const FGameplayTag& ItemTag, int32 Quantity);
	
	UFUNCTION()
	void OnCraftingProgress(float Progress);
	UFUNCTION(BlueprintImplementableEvent,  DisplayName="On Crafting On Progress")
	void BP_OnCraftingProgress(float Progress);

	UFUNCTION()
	void OnCraftingCompleted();
	UFUNCTION(BlueprintImplementableEvent,  DisplayName="Crafting Completed")
	void BP_OnCraftingCompleted();

	UFUNCTION()
	void OnCraftingCanceled();
	UFUNCTION(BlueprintImplementableEvent, DisplayName="Crafting Canceled")
	void BP_OnCraftingCanceled();

};
