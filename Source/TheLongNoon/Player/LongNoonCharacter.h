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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float WalkSpeed = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed = 650.0f;

	/** Interact line-trace distance (cm). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	float InteractReach = 300.0f;
};
