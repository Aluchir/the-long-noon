#include "Player/LongNoonCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

#include "Systems/LongNoonInventoryComponent.h"
#include "Systems/LongNoonReclamationComponent.h"

ALongNoonCharacter::ALongNoonCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	// The body yaws to face movement; the camera controls aim direction.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->bOrientRotationToMovement = true;
		Move->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
		Move->MaxWalkSpeed = WalkSpeed;
		Move->JumpZVelocity = 450.0f;
		Move->AirControl = 0.35f;
	}

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 60.0f);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	Inventory = CreateDefaultSubobject<ULongNoonInventoryComponent>(TEXT("Inventory"));
	Reclamation = CreateDefaultSubobject<ULongNoonReclamationComponent>(TEXT("Reclamation"));
}

void ALongNoonCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (const APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}

void ALongNoonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction)     { Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ALongNoonCharacter::Move); }
		if (LookAction)     { Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &ALongNoonCharacter::Look); }
		if (JumpAction)     { Input->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		                       Input->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping); }
		if (SprintAction)   { Input->BindAction(SprintAction, ETriggerEvent::Started, this, &ALongNoonCharacter::StartSprint);
		                       Input->BindAction(SprintAction, ETriggerEvent::Completed, this, &ALongNoonCharacter::StopSprint); }
		if (InteractAction) { Input->BindAction(InteractAction, ETriggerEvent::Started, this, &ALongNoonCharacter::Interact); }
		if (PruneAction)    { Input->BindAction(PruneAction, ETriggerEvent::Started, this, &ALongNoonCharacter::Prune); }
	}
}

void ALongNoonCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D Axis = Value.Get<FVector2D>();
	const AController* Ctrl = GetController();
	if (!Ctrl)
	{
		return;
	}

	const FRotator YawRotation(0.0f, Ctrl->GetControlRotation().Yaw, 0.0f);
	const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(Forward, Axis.Y);
	AddMovementInput(Right, Axis.X);
}

void ALongNoonCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D Axis = Value.Get<FVector2D>();
	AddControllerYawInput(Axis.X);
	AddControllerPitchInput(Axis.Y);
}

void ALongNoonCharacter::StartSprint(const FInputActionValue& /*Value*/)
{
	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->MaxWalkSpeed = SprintSpeed;
	}
}

void ALongNoonCharacter::StopSprint(const FInputActionValue& /*Value*/)
{
	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->MaxWalkSpeed = WalkSpeed;
	}
}

void ALongNoonCharacter::Interact(const FInputActionValue& /*Value*/)
{
	// TODO: line-trace for interactables (NPCs, gather nodes, lore fragments).
	// See docs/design/ and docs/lore/codex-catalogue.md.
}

void ALongNoonCharacter::Prune(const FInputActionValue& /*Value*/)
{
	if (Reclamation)
	{
		Reclamation->TryReclaimAhead();
	}
}
