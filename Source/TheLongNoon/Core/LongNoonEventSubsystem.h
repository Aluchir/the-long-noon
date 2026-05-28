#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LongNoonEventSubsystem.generated.h"

/**
 * Global event hub (GameInstance subsystem). Systems communicate through these
 * dispatchers instead of holding hard references, so e.g. the codex can react to
 * a found fragment without the world knowing the codex exists.
 * See docs/tech/coding-standards-and-architecture.md.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoreFragmentFound, FName, FragmentId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScriptLiteracyChanged, int32, NewTier);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRegionUnlocked, FName, RegionId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemCrafted, FName, ItemId);

UCLASS()
class THELONGNOON_API ULongNoonEventSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnLoreFragmentFound OnLoreFragmentFound;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnScriptLiteracyChanged OnScriptLiteracyChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnRegionUnlocked OnRegionUnlocked;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnItemCrafted OnItemCrafted;

	UFUNCTION(BlueprintCallable, Category = "Events")
	void BroadcastLoreFragmentFound(FName FragmentId) { OnLoreFragmentFound.Broadcast(FragmentId); }

	UFUNCTION(BlueprintCallable, Category = "Events")
	void BroadcastScriptLiteracyChanged(int32 NewTier) { OnScriptLiteracyChanged.Broadcast(NewTier); }

	UFUNCTION(BlueprintCallable, Category = "Events")
	void BroadcastRegionUnlocked(FName RegionId) { OnRegionUnlocked.Broadcast(RegionId); }

	UFUNCTION(BlueprintCallable, Category = "Events")
	void BroadcastItemCrafted(FName ItemId) { OnItemCrafted.Broadcast(ItemId); }
};
