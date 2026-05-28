#include "Systems/LongNoonSaveService.h"
#include "Systems/LongNoonSaveGame.h"
#include "Core/LongNoonGameInstance.h"
#include "Kismet/GameplayStatics.h"

namespace
{
	constexpr int32 CurrentSaveVersion = 1;
	constexpr int32 UserIndex = 0;
}

bool ULongNoonSaveService::DoesSlotExist(const FString& SlotName) const
{
	return UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex);
}

bool ULongNoonSaveService::SaveToSlot(const FString& SlotName)
{
	ULongNoonSaveGame* Save = Cast<ULongNoonSaveGame>(
		UGameplayStatics::CreateSaveGameObject(ULongNoonSaveGame::StaticClass()));
	if (!Save)
	{
		return false;
	}

	Save->SaveVersion = CurrentSaveVersion;

	if (const ULongNoonGameInstance* GI = Cast<ULongNoonGameInstance>(GetGameInstance()))
	{
		Save->ScriptLiteracyTier = GI->ScriptLiteracyTier;
		Save->bRemembererFound = GI->bRemembererFound;
		Save->MaxToolTier = GI->MaxToolTier;
		Save->UnlockedRegions = GI->UnlockedRegions.Array();
	}

	// TODO: gather inventory stacks and found fragments from their owning systems.

	return UGameplayStatics::SaveGameToSlot(Save, SlotName, UserIndex);
}

bool ULongNoonSaveService::LoadFromSlot(const FString& SlotName)
{
	if (!DoesSlotExist(SlotName))
	{
		return false;
	}

	ULongNoonSaveGame* Save = Cast<ULongNoonSaveGame>(
		UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));
	if (!Save)
	{
		return false;
	}

	ApplyLoadedSave(Save);
	return true;
}

void ULongNoonSaveService::ApplyLoadedSave(ULongNoonSaveGame* Save)
{
	if (!Save)
	{
		return;
	}

	// Migration hook: if Save->SaveVersion < CurrentSaveVersion, upgrade field by field.
	if (Save->SaveVersion < CurrentSaveVersion)
	{
		UE_LOG(LogTemp, Log, TEXT("[Save] Migrating save v%d -> v%d."), Save->SaveVersion, CurrentSaveVersion);
		// TODO: per-version migration steps.
		Save->SaveVersion = CurrentSaveVersion;
	}

	if (ULongNoonGameInstance* GI = Cast<ULongNoonGameInstance>(GetGameInstance()))
	{
		GI->ScriptLiteracyTier = Save->ScriptLiteracyTier;
		GI->bRemembererFound = Save->bRemembererFound;
		GI->MaxToolTier = Save->MaxToolTier;
		GI->UnlockedRegions = TSet<FName>(Save->UnlockedRegions);
	}

	// TODO: restore inventory stacks and found fragments into their owning systems.
}
