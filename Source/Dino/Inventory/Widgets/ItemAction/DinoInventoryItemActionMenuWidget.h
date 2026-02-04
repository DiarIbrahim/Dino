// Code By : Diar Ibrahim,  Contact :  https://www.linkedin.com/in/diar-ibrahim-ali/

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "Dino/Inventory/DinoInventoryTypes.h"
#include "DinoInventoryItemActionMenuWidget.generated.h"

class UMenuAnchor;
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


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dino Inventory|Item Action")
	bool bCloseWhenMouseLeavesMenu = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dino Inventory|Item Action")
	bool bCloseWhenMouseLostCapture = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dino Inventory|Item Action")
	bool bCloseWhenAnyItemActionPressed = true;
	
	// how long should wait after mouse leaves the action menu before we close the menu
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dino Inventory|Item Action")
	float WaitDelayBeforeClose = .6f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dino Inventory|Item Action|Test")
	FDinoInventoryItemActionContainer TestActionData;
	
	UPROPERTY( meta=(BindWidget))
	UVerticalBox* ActionListBox;


	UPROPERTY(BlueprintReadOnly)
	FGameplayTag ItemTag;

	UPROPERTY(BlueprintReadOnly)
	FDinoInventoryItemActionContainer ActionData;

	UPROPERTY(BlueprintReadOnly)
	UDinoInventoryComponent* InventoryComponent;
	UPROPERTY(BlueprintReadOnly)
	UMenuAnchor* MenuAnchor;

	FTimerHandle MouseLostTimerHandle;
	
public:

	virtual void NativePreConstruct() override;

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseCaptureLost(const FCaptureLostEvent& CaptureLostEvent) override;

	void HideActionMenu();

	
	// what item to show the actions for !
	UFUNCTION(BlueprintCallable)
	void Init(UDinoInventoryComponent* OwningComponent, UMenuAnchor* OwningMenuAnchor,  const FDinoInventoryItemData& ItemData);

	void GenerateActionList(const FDinoInventoryItemActionContainer& Data);

	UFUNCTION()
	void HandleItemActionPressed(UDinoInventoryItemActionWidget* DinoInventoryItemActionWidget, const FGameplayTag& ActionTag);
};
