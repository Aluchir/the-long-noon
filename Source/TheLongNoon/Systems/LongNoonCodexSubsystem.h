#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LongNoonCodexSubsystem.generated.h"

class ULoreFragmentDef;

/**
 * The codex: the player is the world's returning memory. Tracks found lore fragments
 * and resolves readability against the current script-literacy tier. Broadcasts
 * discovery through the event subsystem. See docs/lore/codex-catalogue.md.
 */
UCLASS()
class THELONGNOON_API ULongNoonCodexSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/** Record a fragment as discovered; fires the global OnLoreFragmentFound event once. */
	UFUNCTION(BlueprintCallable, Category = "Codex")
	void RegisterFound(FName FragmentId);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Codex")
	bool IsFound(FName FragmentId) const { return FoundFragments.Contains(FragmentId); }

	/** True if the player's literacy is high enough to read this fragment's translation. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Codex")
	bool CanRead(const ULoreFragmentDef* Fragment) const;

	/** Returns translated text if readable, otherwise the untranslated glyph text. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Codex")
	FText GetReadableText(const ULoreFragmentDef* Fragment) const;

	/** Save/restore hooks for the SaveService. */
	const TSet<FName>& GetFoundFragments() const { return FoundFragments; }
	void RestoreFound(const TSet<FName>& InFound) { FoundFragments = InFound; }

private:
	UPROPERTY()
	TSet<FName> FoundFragments;
};
