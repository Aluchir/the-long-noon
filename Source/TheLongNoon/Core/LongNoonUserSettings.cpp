#include "Core/LongNoonUserSettings.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

ULongNoonUserSettings* ULongNoonUserSettings::Load()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName(), UserIndex))
	{
		if (USaveGame* Loaded = UGameplayStatics::LoadGameFromSlot(SlotName(), UserIndex))
		{
			if (ULongNoonUserSettings* Settings = Cast<ULongNoonUserSettings>(Loaded))
			{
				return Settings;
			}
		}
	}

	// No valid saved settings: hand back a fresh instance with class defaults.
	return Cast<ULongNoonUserSettings>(
		UGameplayStatics::CreateSaveGameObject(ULongNoonUserSettings::StaticClass()));
}

void ULongNoonUserSettings::Save(ULongNoonUserSettings* In)
{
	if (!In)
	{
		return;
	}

	UGameplayStatics::SaveGameToSlot(In, SlotName(), UserIndex);
}

void ULongNoonUserSettings::Apply(const UObject* WorldContext, ULongNoonUserSettings* In)
{
	if (!In || !WorldContext)
	{
		return;
	}

	const UWorld* World = WorldContext->GetWorld();
	if (!World)
	{
		return;
	}

	APlayerController* PC = World->GetFirstPlayerController();
	if (!PC)
	{
		return;
	}

	// Push master volume to the audio engine. MasterVolume is normalized 0..1.
	const float Volume = FMath::Clamp(In->MasterVolume, 0.0f, 1.0f);
	PC->ConsoleCommand(FString::Printf(TEXT("au.MasterVolume %f"), Volume), /*bWriteToLog=*/false);

	// Mouse sensitivity has no single engine sink here; callers/UI apply it.
}
