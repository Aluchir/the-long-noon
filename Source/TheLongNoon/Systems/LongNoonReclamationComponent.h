#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/ToolDef.h"
#include "LongNoonReclamationComponent.generated.h"

class UToolDef;

/** Fires when the equipped tool changes or its durability moves (HUD/UI hook). */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnEquippedToolChanged, FName, ToolId, int32, Durability, int32, MaxDurability);

/**
 * Resolved runtime state of the equipped tool. Populated from a UToolDef asset or
 * a data row (FToolRow) so the component is source-agnostic. Durability is tracked
 * here (not on the shared asset) since it is per-instance wear.
 */
USTRUCT(BlueprintType)
struct FEquippedTool
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Reclamation") FName ToolId;
	UPROPERTY(BlueprintReadOnly, Category = "Reclamation") TArray<EReclamationVerb> Verbs;
	UPROPERTY(BlueprintReadOnly, Category = "Reclamation") float Reach = 2.0f;
	UPROPERTY(BlueprintReadOnly, Category = "Reclamation") float StaminaCost = 5.0f;
	UPROPERTY(BlueprintReadOnly, Category = "Reclamation") int32 MaxDurability = 100;
	UPROPERTY(BlueprintReadOnly, Category = "Reclamation") int32 CurrentDurability = 100;

	bool IsValid() const { return !ToolId.IsNone(); }
	/** A worn tool needs mending before it can be used again (a setback, never destruction). */
	bool IsWorn() const { return CurrentDurability <= 0; }
};

/**
 * The non-lethal "combat" system: prune, still, settle, seal, quiet the Bloom.
 * Nothing dies, including the player. "Failure" is setback, never death.
 * This traces ahead of the player and applies the equipped tool's verb, spending
 * stamina on the Tend component and wearing the tool's durability per use.
 * See docs/design/reclamation-combat.md and docs/design/weapon-and-tool-tiers.md.
 */
UCLASS(ClassGroup = (LongNoon), meta = (BlueprintSpawnableComponent))
class THELONGNOON_API ULongNoonReclamationComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Broadcast when the tool or its durability changes (for the HUD). */
	UPROPERTY(BlueprintAssignable, Category = "Reclamation")
	FOnEquippedToolChanged OnEquippedToolChanged;

	/** Durability spent per successful reclamation (the main upkeep pressure). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reclamation", meta = (ClampMin = "0"))
	int32 DurabilityCostPerUse = 1;

	/** Equip from a UToolDef asset (Blueprint/editor path). */
	UFUNCTION(BlueprintCallable, Category = "Reclamation")
	void EquipTool(UToolDef* Tool);

	/** Equip by id from the data registry (the data-driven path; mirrors CraftById). */
	UFUNCTION(BlueprintCallable, Category = "Reclamation")
	bool EquipToolById(FName ToolId);

	/** Trace ahead of the owner and reclaim the first Bloom target found. */
	UFUNCTION(BlueprintCallable, Category = "Reclamation")
	bool TryReclaimAhead();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Reclamation")
	bool CanPerform(EReclamationVerb Verb) const;

	/** Restore durability (a repair kit). Clamped to the tool's max. */
	UFUNCTION(BlueprintCallable, Category = "Reclamation")
	void Repair(int32 Amount);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Reclamation")
	bool HasTool() const { return Equipped.IsValid(); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Reclamation")
	FEquippedTool GetEquippedTool() const { return Equipped; }

protected:
	/** Runtime state of the currently equipped tool. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Reclamation")
	FEquippedTool Equipped;
};
