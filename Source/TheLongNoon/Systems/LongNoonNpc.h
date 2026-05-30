#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/Interactable.h"
#include "LongNoonNpc.generated.h"

class UStaticMeshComponent;
class ULongNoonDialogueComponent;

/**
 * A named NPC the player can talk to. Carries a dialogue component whose lines are
 * loaded from data (NPCs.csv / DialogueLines.csv) by NpcId on BeginPlay. Interacting
 * starts the conversation. See docs/lore/dialogue-scripts.md and the data guide.
 */
UCLASS()
class THELONGNOON_API ALongNoonNpc : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	ALongNoonNpc();

	/** Roster id (matches NPCs.csv), e.g. "npc_tinker". Drives data-loaded dialogue. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPC")
	FName NpcId;

	// IInteractable
	virtual void OnInteract_Implementation(AActor* Interactor) override;
	virtual FText GetInteractPrompt_Implementation() const override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC")
	TObjectPtr<ULongNoonDialogueComponent> Dialogue;
};
