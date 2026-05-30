#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "LongNoonCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class ULongNoonInventoryComponent;
class ULongNoonReclamationComponent;
class ULongNoonCraftingComponent;
class ULongNoonBuildingComponent;
class ULongNoonTendComponent;
class ALongNoonHUD;
class USoundBase;

/**
 * Third-person player character: the Newcomer. Camera boom + follow camera and
 * Enhanced Input movement. Mirrors the UE Third Person template structure so the
 * template's content (or new content) drops onto this C++ base.
 * Gameplay systems are attached as components (inventory, reclamation, ...).
 * See docs/characters/player-character.md and docs/tech/coding-standards-and-architecture.md.
 */
UCLASS()
class THELONGNOON_API ALongNoonCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ALongNoonCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// --- Camera ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> FollowCamera;

	// --- Gameplay systems (components) ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Systems")
	TObjectPtr<ULongNoonInventoryComponent> Inventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Systems")
	TObjectPtr<ULongNoonReclamationComponent> Reclamation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Systems")
	TObjectPtr<ULongNoonCraftingComponent> Crafting;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Systems")
	TObjectPtr<ULongNoonBuildingComponent> Building;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Systems")
	TObjectPtr<ULongNoonTendComponent> Tend;

	// --- Enhanced Input (assets authored in Content/Input, assigned per-Blueprint) ---
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> PruneAction;

	// Input handlers
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartSprint(const FInputActionValue& Value);
	void StopSprint(const FInputActionValue& Value);
	void Interact(const FInputActionValue& Value);
	void Prune(const FInputActionValue& Value);

	// --- HUD link (systems push display updates through ALongNoonHUD) ---
	/** Subscribe components/events to the HUD and start the interact-focus poll. */
	void SetupHUDLink();

	/** Resolve (and cache) the player HUD; re-resolves if it spawned after this pawn. */
	ALongNoonHUD* ResolveHUD();

	/** Tend meters changed -> refresh the HUD (Comfort is read from the component). */
	UFUNCTION()
	void HandleTendChanged(float Stamina, float Focus);

	/** A lore fragment was found -> gentle HUD toast. */
	UFUNCTION()
	void HandleLoreFound(FName FragmentId);

	/** A quest objective completed -> refresh the HUD objective tracker. */
	UFUNCTION()
	void HandleObjectiveCompleted(FName ObjectiveId);

	/** The active quest finished -> celebrate on the HUD. */
	UFUNCTION()
	void HandleQuestCompleted();

	/** A quest was seeded -> show its first objective. */
	UFUNCTION()
	void HandleQuestStarted();

	/** Push the current active quest objective to the HUD tracker. */
	void RefreshObjectiveDisplay();

	/** Poll the focus trace and push the interact prompt to the HUD. */
	void UpdateInteractFocus();

	UPROPERTY()
	TObjectPtr<ALongNoonHUD> HUD;

	FTimerHandle InteractFocusTimer;

	/** Last prompt pushed, so we only refresh the HUD on change. */
	FText LastInteractPrompt;

	/** Reclamation tool equipped on spawn, looked up from the Tools DataTable. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reclamation")
	FName StartingToolId = TEXT("tool_pruning_blade");

	/** Stinger played when a quest objective completes (CC0 pizzicato). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	TObjectPtr<USoundBase> ObjectiveCue;

	/** Stinger played when the region quest completes (CC0 sax flourish). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	TObjectPtr<USoundBase> QuestCompleteCue;

	/** Seconds between interact-focus polls (cozy game; no need for per-frame). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	float InteractFocusInterval = 0.15f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float WalkSpeed = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed = 650.0f;

	/** Interact line-trace distance (cm). Traced from the follow camera, so it must
	 *  exceed the camera boom length (400) to reach objects in front of the character. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	float InteractReach = 700.0f;
};
