#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "LongNoonGameInstance.generated.h"

/**
 * Cross-level game state. Persists across map loads (unlike GameMode/GameState):
 * active save slot, progression flags, and the player's forgotten-script literacy.
 * Gameplay logic lives in per-system components/subsystems; this stays thin.
 * See docs/tech/coding-standards-and-architecture.md.
 */
UCLASS()
class THELONGNOON_API ULongNoonGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	/** 0 = illiterate ... 4 = fluent. Drives lore-fragment readability. */
	UPROPERTY(BlueprintReadOnly, Category = "Progression")
	int32 ScriptLiteracyTier = 0;

	/** Whether the Rememberer has been found (gates the Third Way ending). */
	UPROPERTY(BlueprintReadOnly, Category = "Progression")
	bool bRemembererFound = false;

	/** Highest reclamation tool tier the player has crafted (gates regions). */
	UPROPERTY(BlueprintReadOnly, Category = "Progression")
	int32 MaxToolTier = 1;

	/** Region ids the player has unlocked. */
	UPROPERTY(BlueprintReadOnly, Category = "Progression")
	TSet<FName> UnlockedRegions;

	/** Raise literacy and notify the event subsystem. */
	UFUNCTION(BlueprintCallable, Category = "Progression")
	void SetScriptLiteracyTier(int32 NewTier);

	/** Mark the Rememberer found (unlocks the Third Way ending). Idempotent. */
	UFUNCTION(BlueprintCallable, Category = "Progression")
	void SetRemembererFound();
};
