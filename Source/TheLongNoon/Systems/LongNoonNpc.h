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

	/** Whether this NPC gently wanders around its spawn point when idle. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPC|Wander")
	bool bWander = true;

	/** Max distance (cm) from the spawn/home point a wander target may be picked. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPC|Wander")
	float WanderRadius = 200.0f;

	/** Movement speed (cm/sec) while walking to a wander target. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPC|Wander")
	float WalkSpeed = 60.0f;

	/** How long (sec) the NPC stands idle between walks. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPC|Wander")
	float PauseSeconds = 3.0f;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

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

	/** Looping locomotion clip (a run, played slow to read as a walk) used while wandering. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPC")
	TObjectPtr<UAnimSequence> WalkAnim;

	/** Target on-screen height (cm); the mesh auto-scales to it on BeginPlay. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPC")
	float TargetHeight = 180.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC")
	TObjectPtr<ULongNoonDialogueComponent> Dialogue;

private:
	/** Wander state machine phases. */
	enum class EWanderState : uint8
	{
		Pausing,
		Walking
	};

	/** Spawn point; all wander targets are picked within WanderRadius of this. */
	FVector HomeLocation = FVector::ZeroVector;

	/** Current wander destination (valid while Walking). */
	FVector WanderTarget = FVector::ZeroVector;

	/** Current phase of the wander loop. */
	EWanderState WanderState = EWanderState::Pausing;

	/** Seconds remaining in the current pause. */
	float PauseTimer = 0.0f;

	/** Accumulated time, used to vary deterministic target picks. */
	float WanderClock = 0.0f;

	/** True while the idle clip is the active animation (avoids re-playing every tick). */
	bool bIdlePlaying = false;

	/** Switch the mesh to the looping idle clip (idempotent). */
	void PlayIdle();

	/** Pick a fresh wander target around HomeLocation and start walking to it. */
	void BeginWalk();
};
