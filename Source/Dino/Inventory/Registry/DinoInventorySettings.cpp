// Fill out your copyright notice in the Description page of Project Settings.


#include "DinoInventorySettings.h"



UDinoInventorySettings::UDinoInventorySettings() {

	CategoryName = "Dino";
}


UDinoInventoryRegistryData* UDinoInventorySettingsHelpers::GetDinoInventoryRegistryData()
{
	const UDinoInventorySettings* Settings = GetDefault<UDinoInventorySettings>();
	if (IsValid(Settings)) {
		return Settings->SoftInventoryRegistryData.LoadSynchronous();
	}
	return nullptr;
}
