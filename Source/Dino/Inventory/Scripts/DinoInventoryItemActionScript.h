// Code By : Diar Ibrahim,  Contact :  https://www.linkedin.com/in/diar-ibrahim-ali/

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "DinoInventoryItemActionScript.generated.h"

class UDinoInventoryComponent;
/**
 * 
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class DINO_API UDinoInventoryItemActionScript : public UObject
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag OwningItemTag;
	
	UPROPERTY(BlueprintReadOnly)
	UDinoInventoryComponent* OwningInventoryComponent;
	
public:

	// initialize this Action
	void InitAction(UDinoInventoryComponent* InInventoryComponent, const FGameplayTag& InItemTag);
	// call when action ended to clean up
	UFUNCTION(BlueprintCallable)	
	void EndAction();
	
protected:
	
	UFUNCTION(BlueprintNativeEvent)
	void OnActionStarted(UDinoInventoryComponent* InventoryComponent, const FGameplayTag& ItemTag);
	virtual void OnActionStarted_Implementation(UDinoInventoryComponent* OwningComponent, const FGameplayTag& ItemTag);


	UFUNCTION(BlueprintNativeEvent)
	void OnActionEnded();
	virtual void OnActionEnded_Implementation();

	UFUNCTION(BlueprintCallable)
	UDinoInventoryComponent* GetOwningInventoryComponent() const {return OwningInventoryComponent;}

	UFUNCTION(BlueprintCallable)
	FGameplayTag GetItemTag() const {return OwningItemTag;}
	
};
