#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/ToolDef.h"
#include "LongNoonReclamationComponent.generated.h"

class UToolDef;

/**
 * The non-lethal "combat" system: prune, still, settle, seal, quiet the Bloom.
 * Nothing dies, including the player. "Failure" is setback, never death.
 * This stub traces ahead of the player and applies the equipped tool's verb.
 * See docs/design/reclamation-combat.md and docs/design/weapon-and-tool-tiers.md.
 */
UCLASS(ClassGroup = (LongNoon), meta = (BlueprintSpawnableComponent))
class THELONGNOON_API ULongNoonReclamationComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Currently equipped tool (sets available verbs + reach). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reclamation")
	TObjectPtr<UToolDef> EquippedTool;

	/** Trace ahead of the owner and reclaim the first Bloom target found. */
	UFUNCTION(BlueprintCallable, Category = "Reclamation")
	bool TryReclaimAhead();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Reclamation")
	bool CanPerform(EReclamationVerb Verb) const;
};
