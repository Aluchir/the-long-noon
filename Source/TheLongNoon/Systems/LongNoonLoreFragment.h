#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/Interactable.h"
#include "LongNoonLoreFragment.generated.h"

class UStaticMeshComponent;

/**
 * A findable forgotten-script fragment in the world. Interacting registers it with
 * the codex (raising literacy past thresholds, which retroactively unlocks readable
 * translations), then the pickup hides. Uses the proven IInteractable trace.
 * See docs/lore/codex-catalogue.md and forgotten-script.md.
 */
UCLASS()
class THELONGNOON_API ALongNoonLoreFragment : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	ALongNoonLoreFragment();

	/** Fragment id granted on read (matches LoreFragments.csv). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lore")
	FName FragmentId;

	/** If true, reading this also marks the Rememberer found (unlocks the Third Way). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lore")
	bool bGrantsRememberer = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lore")
	bool bFound = false;

	virtual void OnInteract_Implementation(AActor* Interactor) override;
	virtual FText GetInteractPrompt_Implementation() const override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lore")
	TObjectPtr<UStaticMeshComponent> Mesh;
};
