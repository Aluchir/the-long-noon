#include "Systems/LongNoonSaveService.h"
#include "Systems/LongNoonSaveGame.h"
#include "Systems/LongNoonInventoryComponent.h"
#include "Systems/LongNoonCodexSubsystem.h"
#include "Core/LongNoonGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"

namespace
{
	/** Find the local player's inventory component, if a pawn exists. */
	ULongNoonInventoryComponent* FindPlayerInventory(const UWorld* World)
	{
		if (!World) { return nullptr; }
		const APlayerController* PC = World->GetFirstPlayerController();
		const APawn* Pawn = PC ? PC->GetPawn() : nullptr;
		return Pawn ? Pawn->FindComponentByClass<ULongNoonInventoryComponent>() : nullptr;
	}
}

namespace
{
	constexpr int32 CurrentSaveVersion = 1;
	constexpr int32 SaveUserIndex = 0;
}

bool ULongNoonSaveService::DoesSlotExist(const FString& SlotName) const
{
	return UGameplayStatics::DoesSaveGameExist(SlotName, SaveUserIndex);
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

	if (const ULongNoonInventoryComponent* Inv = FindPlayerInventory(GetGameInstance()->GetWorld()))
	{
		Save->InventoryStacks = Inv->GetStacks();
	}
	if (const ULongNoonCodexSubsystem* Codex = GetGameInstance()->GetSubsystem<ULongNoonCodexSubsystem>())
	{
		Save->FoundFragments = Codex->GetFoundFragments();
	}

	return UGameplayStatics::SaveGameToSlot(Save, SlotName, SaveUserIndex);
}

bool ULongNoonSaveService::LoadFromSlot(const FString& SlotName)
{
	if (!DoesSlotExist(SlotName))
	{
		return false;
	}

	ULongNoonSaveGame* Save = Cast<ULongNoonSaveGame>(
		UGameplayStatics::LoadGameFromSlot(SlotName, SaveUserIndex));
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

	if (ULongNoonInventoryComponent* Inv = FindPlayerInventory(GetGameInstance()->GetWorld()))
	{
		Inv->RestoreStacks(Save->InventoryStacks);
	}
	if (ULongNoonCodexSubsystem* Codex = GetGameInstance()->GetSubsystem<ULongNoonCodexSubsystem>())
	{
		Codex->RestoreFound(Save->FoundFragments);
	}
}
