// Code By : Diar Ibrahim,  Contact :  https://www.linkedin.com/in/diar-ibrahim-ali/


#include "DinoInventorySettings.h"



UDinoInventorySettings::UDinoInventorySettings() {

	CategoryName = "Dino";
}


UDataTable* UDinoInventorySettingsHelpers::GetDinoInventoryRegistryData()
{
	const UDinoInventorySettings* Settings = GetDefault<UDinoInventorySettings>();
	if (IsValid(Settings)) {
		return Settings->SoftInventoryRegistryData.LoadSynchronous();
	}
	return nullptr;
}
