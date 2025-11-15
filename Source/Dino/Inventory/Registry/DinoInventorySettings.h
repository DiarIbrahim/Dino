// Code By : Diar Ibrahim,  Contact :  https://www.linkedin.com/in/diar-ibrahim-ali/


#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Engine/DataTable.h"
#include "DinoInventorySettings.generated.h"


// a settings class that live in the project settings, we use this for setttings related to the Dino Inventory

UCLASS(Config = DinoInventorySettings, DefaultConfig)
class DINO_API UDinoInventorySettings : public UDeveloperSettings {

	GENERATED_BODY()

	UDinoInventorySettings();

public:

	// Override 
	virtual FName GetCategoryName() const override { return TEXT("Dino"); }



	// registry of all items that can be recognised by Dino system
	UPROPERTY(EditAnywhere, Config, Category = "Dino Inventory Registry", meta = (DisplayName = "Inventory Item Registry Data"), meta = (RequiredAssetDataTags = "RowStructure=/Script/Dino.DinoInventoryItemData"))
	TSoftObjectPtr<UDataTable> SoftInventoryRegistryData;


};




// helper functions to help with Dino inventory settings, helps to make the function accessable to cpp/BP

UCLASS()
class DINO_API UDinoInventorySettingsHelpers : public UBlueprintFunctionLibrary {

	GENERATED_BODY()

public:

	// returns a pointer to the Dino Inventory Register Data asset.
	UFUNCTION(BlueprintCallable, Category = "Dino|Inventory")
	static UDataTable* GetDinoInventoryRegistryData();


};
