#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/Interactable.h"
#include "Systems/LongNoonEndings.h"
#include "LongNoonChoiceMarker.generated.h"

class UStaticMeshComponent;

/**
 * One of the three choice points at the First Garden. The player walks up to a marker
 * and interacts to tell the Gardener their decision; the ending is resolved via
 * ULongNoonEndingLibrary (Third Way only if the Rememberer was found). Uses the same
 * proven IInteractable trace as gather nodes/NPCs - no UMG. See docs/lore/endings.md.
 */
UCLASS()
class THELONGNOON_API ALongNoonChoiceMarker : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	ALongNoonChoiceMarker();

	/** Which choice this marker represents. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ending")
	ELongNoonChoice Choice = ELongNoonChoice::GiveItBack;

	/** The resolved ending after the player chooses (None until then). */
	UPROPERTY(BlueprintReadOnly, Category = "Ending")
	ELongNoonEnding Resolved = ELongNoonEnding::None;

	virtual void OnInteract_Implementation(AActor* Interactor) override;
	virtual FText GetInteractPrompt_Implementation() const override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ending")
	TObjectPtr<UStaticMeshComponent> Mesh;
};
