// Code By : Diar Ibrahim,  Contact :  https://www.linkedin.com/in/diar-ibrahim-ali/

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "Dino/Inventory/DinoInventoryTypes.h"
#include "DinoInventoryItemActionMenuWidget.generated.h"

class UDinoInventoryItemActionWidget;
class UVerticalBox;
/**
 * 
 */
UCLASS()
class DINO_API UDinoInventoryItemActionMenuWidget : public UUserWidget
{
	GENERATED_BODY()


public:


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dino Inventory|Item Action")
	TSubclassOf<UDinoInventoryItemActionWidget> ItemActionWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dino Inventory|Item Action")
	FMargin ActionPadding = FMargin(0,10);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dino Inventory|Item Action|Test")
	FDinoInventoryItemActionContainer TestActionData;
	
	UPROPERTY( meta=(BindWidget))
	UVerticalBox* ActionListBox;


	UPROPERTY(BlueprintReadOnly)
	FGameplayTag ItemTag;

	UPROPERTY(BlueprintReadOnly)
	FDinoInventoryItemActionContainer ActionData;
	
public:

	virtual void NativePreConstruct() override;
	

	// what item to show the actions for !
	UFUNCTION(BlueprintCallable)
	void SetItemData(const FDinoInventoryItemData& ItemData);

	void GenerateActionList(const FDinoInventoryItemActionContainer& Data);
	
};
