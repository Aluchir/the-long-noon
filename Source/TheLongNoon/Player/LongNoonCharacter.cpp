#include "Player/LongNoonCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

#include "Systems/LongNoonInventoryComponent.h"
#include "Systems/LongNoonReclamationComponent.h"
#include "Systems/LongNoonCraftingComponent.h"
#include "Systems/LongNoonBuildingComponent.h"
#include "Systems/LongNoonTendComponent.h"
#include "Core/Interactable.h"
#include "Core/LongNoonEventSubsystem.h"
#include "Core/LongNoonLog.h"
#include "UI/LongNoonHUD.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "GameFramework/PlayerController.h"
#include "TimerManager.h"

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
	Crafting = CreateDefaultSubobject<ULongNoonCraftingComponent>(TEXT("Crafting"));
	Building = CreateDefaultSubobject<ULongNoonBuildingComponent>(TEXT("Building"));
	Tend = CreateDefaultSubobject<ULongNoonTendComponent>(TEXT("Tend"));
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

	// Equip the starting reclamation tool from the Tools DataTable.
	if (Reclamation && !StartingToolId.IsNone())
	{
		Reclamation->EquipToolById(StartingToolId);
	}

	SetupHUDLink();
}

ALongNoonHUD* ALongNoonCharacter::ResolveHUD()
{
	// The HUD actor may spawn after this pawn's BeginPlay, so resolve lazily.
	if (!HUD)
	{
		if (const APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			HUD = Cast<ALongNoonHUD>(PC->GetHUD());
		}
	}
	return HUD;
}

void ALongNoonCharacter::SetupHUDLink()
{
	ResolveHUD();

	// Tend meters -> HUD.
	if (Tend)
	{
		Tend->OnTendChanged.AddUniqueDynamic(this, &ALongNoonCharacter::HandleTendChanged);
		HandleTendChanged(Tend->Stamina, Tend->Focus); // seed the initial display
	}

	// Lore fragments found -> HUD toast (routed through the global event hub).
	if (const UWorld* World = GetWorld())
	{
		if (const UGameInstance* GI = World->GetGameInstance())
		{
			if (ULongNoonEventSubsystem* Events = GI->GetSubsystem<ULongNoonEventSubsystem>())
			{
				Events->OnLoreFragmentFound.AddUniqueDynamic(this, &ALongNoonCharacter::HandleLoreFound);
			}
		}

		// Poll the interact focus on a gentle cadence and push the prompt to the HUD.
		World->GetTimerManager().SetTimer(InteractFocusTimer, this,
			&ALongNoonCharacter::UpdateInteractFocus, InteractFocusInterval, true);
	}
}

void ALongNoonCharacter::HandleTendChanged(float Stamina, float Focus)
{
	if (ALongNoonHUD* H = ResolveHUD())
	{
		H->UpdateTend(Stamina, Focus, Tend ? Tend->Comfort : 0.0f);
	}
}

void ALongNoonCharacter::HandleLoreFound(FName FragmentId)
{
	if (ALongNoonHUD* H = ResolveHUD())
	{
		H->NotifyLoreFound(FragmentId);
	}
}

void ALongNoonCharacter::UpdateInteractFocus()
{
	ALongNoonHUD* H = ResolveHUD();
	if (!H || !FollowCamera)
	{
		return;
	}

	FText Prompt = FText::GetEmpty();

	const FVector Start = FollowCamera->GetComponentLocation();
	const FVector End = Start + FollowCamera->GetForwardVector() * InteractReach;

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (GetWorld() && GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor && HitActor->Implements<UInteractable>())
		{
			Prompt = IInteractable::Execute_GetInteractPrompt(HitActor);
		}
	}

	// Only refresh the HUD when the prompt actually changes.
	if (!Prompt.EqualTo(LastInteractPrompt))
	{
		LastInteractPrompt = Prompt;
		H->SetInteractPrompt(Prompt);
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
	UE_LOG(LogLongNoon, Log, TEXT("[Input] Interact pressed"));
	if (!FollowCamera)
	{
		return;
	}

	// Trace ahead from the camera; interact with the first IInteractable hit
	// (NPCs, gather nodes, lore fragments, gate-builds).
	const FVector Start = FollowCamera->GetComponentLocation();
	const FVector End = Start + FollowCamera->GetForwardVector() * InteractReach;

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (GetWorld() && GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor && HitActor->Implements<UInteractable>())
		{
			IInteractable::Execute_OnInteract(HitActor, this);
		}
	}
}

void ALongNoonCharacter::Prune(const FInputActionValue& /*Value*/)
{
	UE_LOG(LogLongNoon, Log, TEXT("[Input] Prune pressed"));
	if (Reclamation)
	{
		const bool bReclaimed = Reclamation->TryReclaimAhead();
		UE_LOG(LogLongNoon, Log, TEXT("[Input] Prune reclaim result = %s"), bReclaimed ? TEXT("true") : TEXT("false"));
	}
}
