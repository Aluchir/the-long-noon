#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/Interactable.h"
#include "LongNoonNpc.generated.h"

class USkeletalMeshComponent;
class UAnimSequence;
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

	/** Show a dialogue line on the player's HUD. */
	UFUNCTION()
	void HandleDialogueLine(const FText& Line);

	/** Clear the dialogue line when the conversation ends. */
	UFUNCTION()
	void HandleDialogueEnded();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC")
	TObjectPtr<USkeletalMeshComponent> Mesh;

	/** Looping idle animation played on BeginPlay (CC0 Kenney clip). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPC")
	TObjectPtr<UAnimSequence> IdleAnim;

	/** Target on-screen height (cm); the mesh auto-scales to it on BeginPlay. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPC")
	float TargetHeight = 180.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC")
	TObjectPtr<ULongNoonDialogueComponent> Dialogue;
};
