// Code By : Diar Ibrahim,  Contact :  https://www.linkedin.com/in/diar-ibrahim-ali/

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Dino/Inventory/DinoInventoryTypes.h"
#include "DinoInventoryItemActionWidget.generated.h"



// item action


UCLASS()
class DINO_API UDinoInventoryItemActionWidget : public UUserWidget
{
	GENERATED_BODY()

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDinoInventoryItemActionDelegate, UDinoInventoryItemActionWidget*, ActionWidget, const FGameplayTag&, ActionTag);

public:

	FOnDinoInventoryItemActionDelegate OnItemPressed;
	
protected:

	UPROPERTY(meta=(BindWidget))
	UButton* ActionButton;

	UPROPERTY(BlueprintReadOnly)
	FDinoInventoryItemActionData InventoryItemActionData;

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag ActionTag;
	

public:
	virtual void NativeConstruct() override;

	
	void SetActionData(const FGameplayTag& InActionTag, const FDinoInventoryItemActionData& InInventoryItemAction);

	UFUNCTION(BlueprintImplementableEvent,  DisplayName="On Item Action Data Set")
	void BP_OnActionDataSet(const FDinoInventoryItemActionData& ItemActionData);
	
	UFUNCTION()
	void ActionButtonPressed();
	
	
};
