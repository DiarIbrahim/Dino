// Code By : Diar Ibrahim,  Contact :  https://www.linkedin.com/in/diar-ibrahim-ali/

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Dino/Inventory/DinoInventoryTypes.h"
#include "DinoInventoryCraftableItemWidget.generated.h"


class UButton;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDinoInventoryCraftableItemDelegate, UDinoInventoryCraftableItemWidget*,
                                            CraftableItemWidget);




/**
 *   this is a widget shown  in UDinoInventoryCraftWidget as a craftable item  widget (in a list ), can be selected and deselected to be crafted if dependencies match
 */


UCLASS()
class DINO_API UDinoInventoryCraftableItemWidget : public UUserWidget
{
	GENERATED_BODY()



public:

	UPROPERTY(BlueprintAssignable)
	FDinoInventoryCraftableItemDelegate OnSelected;
	
	
protected:

	
	UPROPERTY(BlueprintReadOnly)
	FDinoInventoryItemData CraftableItemData;
	
	UPROPERTY(meta=(BindWidget))
	UButton* Button;


	bool bSelected = false;

	
public:

	virtual void NativeConstruct() override;
	

	const FDinoInventoryItemData& GetItemData(){ return CraftableItemData;}
	
	UFUNCTION(BlueprintCallable)
	void SetCraftableItemData(const FDinoInventoryItemData& InCraftableItemData);
	UFUNCTION(BlueprintImplementableEvent, DisplayName="On Craftable Data Set")
	void BP_OnCraftableDataSet(const FDinoInventoryItemData& ItemData);
	

	UFUNCTION()
	void Select();
	UFUNCTION(BlueprintImplementableEvent, DisplayName="On Selected")
	void BP_Select();

	UFUNCTION()
	void Deselect();
	UFUNCTION(BlueprintImplementableEvent, displayName="On Deselected")
	void BP_Deselect();
	
};
