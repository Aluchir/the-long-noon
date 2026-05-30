#include "Systems/LongNoonNpc.h"
#include "Systems/LongNoonDialogueComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Animation/AnimSequence.h"
#include "Core/LongNoonLog.h"
#include "UI/LongNoonHUD.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"

ALongNoonNpc::ALongNoonNpc()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("NpcMesh"));
	SetRootComponent(Mesh);

	// Default body + idle clip: Kenney Animated Characters 3 (CC0). Editable per instance.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Body(TEXT("/Game/ThirdParty/CharAnim/characterMedium.characterMedium"));
	if (Body.Succeeded())
	{
		Mesh->SetSkeletalMeshAsset(Body.Object);
	}
	static ConstructorHelpers::FObjectFinder<UAnimSequence> Idle(TEXT("/Game/ThirdParty/CharAnim/idleRoot_Idle.idleRoot_Idle"));
	if (Idle.Succeeded())
	{
		IdleAnim = Idle.Object;
	}
	// Locomotion clip is a run; we slow its play rate to read as a gentle walk.
	static ConstructorHelpers::FObjectFinder<UAnimSequence> Walk(TEXT("/Game/ThirdParty/CharAnim/runRoot_Run.runRoot_Run"));
	if (Walk.Succeeded())
	{
		WalkAnim = Walk.Object;
	}
	Mesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);

	Dialogue = CreateDefaultSubobject<ULongNoonDialogueComponent>(TEXT("Dialogue"));
}

void ALongNoonNpc::BeginPlay()
{
	Super::BeginPlay();

	// Play the looping idle and auto-scale to a sane on-screen height (the placed
	// instances carry stale scales from the old static-mesh swap).
	if (Mesh)
	{
		if (IdleAnim)
		{
			Mesh->PlayAnimation(IdleAnim, true);
			Mesh->SetPlayRate(1.0f);
			bIdlePlaying = true;
		}
		if (USkeletalMesh* Skm = Mesh->GetSkeletalMeshAsset())
		{
			const float H = FMath::Max(1.0f, Skm->GetBounds().BoxExtent.Z * 2.0f);
			const float S = TargetHeight / H;
			SetActorScale3D(FVector(S));
		}
	}

	// Anchor wandering to wherever the instance was placed/spawned.
	HomeLocation = GetActorLocation();
	WanderState = EWanderState::Pausing;
	PauseTimer = PauseSeconds;

	if (Dialogue && !NpcId.IsNone())
	{
		Dialogue->NpcId = NpcId;
		const bool bOk = Dialogue->LoadFromData();
		Dialogue->OnDialogueLine.AddUniqueDynamic(this, &ALongNoonNpc::HandleDialogueLine);
		Dialogue->OnDialogueEnded.AddUniqueDynamic(this, &ALongNoonNpc::HandleDialogueEnded);
		UE_LOG(LogLongNoon, Log, TEXT("[NPC] %s LoadFromData=%s lines=%d"),
			*NpcId.ToString(), bOk ? TEXT("ok") : TEXT("FAIL"), Dialogue->Lines.Num());
	}
}

void ALongNoonNpc::OnInteract_Implementation(AActor* /*Interactor*/)
{
	if (!Dialogue)
	{
		return;
	}
	// First interact starts the talk; each subsequent interact advances a line.
	if (Dialogue->IsConversing())
	{
		Dialogue->Advance();
	}
	else
	{
		Dialogue->StartDialogue();
		UE_LOG(LogLongNoon, Log, TEXT("[NPC] %s talk started (%d lines)"),
			*NpcId.ToString(), Dialogue->Lines.Num());
	}
}

void ALongNoonNpc::HandleDialogueLine(const FText& Line)
{
	if (const UWorld* World = GetWorld())
	{
		if (const APlayerController* PC = World->GetFirstPlayerController())
		{
			if (ALongNoonHUD* HUD = Cast<ALongNoonHUD>(PC->GetHUD()))
			{
				HUD->SetDialogue(Line);
			}
		}
	}
}

void ALongNoonNpc::HandleDialogueEnded()
{
	if (const UWorld* World = GetWorld())
	{
		if (const APlayerController* PC = World->GetFirstPlayerController())
		{
			if (ALongNoonHUD* HUD = Cast<ALongNoonHUD>(PC->GetHUD()))
			{
				HUD->SetDialogue(FText::GetEmpty());
			}
		}
	}
}

FText ALongNoonNpc::GetInteractPrompt_Implementation() const
{
	return NSLOCTEXT("LongNoon", "TalkPrompt", "Talk");
}

void ALongNoonNpc::PlayIdle()
{
	if (Mesh && IdleAnim && !bIdlePlaying)
	{
		Mesh->PlayAnimation(IdleAnim, true);
		Mesh->SetPlayRate(1.0f);
		bIdlePlaying = true;
	}
}

void ALongNoonNpc::BeginWalk()
{
	// Deterministic, per-instance target: hash the unique id with the running clock
	// through sine so different NPCs spread out and each one drifts over time.
	// (No FMath::Rand so behaviour stays reproducible.)
	const float Seed = static_cast<float>(GetUniqueID());
	const float A = FMath::Sin(WanderClock * 0.37f + Seed * 0.013f);
	const float B = FMath::Cos(WanderClock * 0.29f + Seed * 0.021f);

	// Angle around home and a radius in [0.25, 1] * WanderRadius.
	const float Angle = (A * 0.5f + 0.5f) * 2.0f * PI;
	const float Dist = WanderRadius * (0.25f + 0.75f * (B * 0.5f + 0.5f));

	WanderTarget = HomeLocation + FVector(FMath::Cos(Angle) * Dist, FMath::Sin(Angle) * Dist, 0.0f);
	WanderState = EWanderState::Walking;

	if (Mesh && WalkAnim)
	{
		Mesh->PlayAnimation(WalkAnim, true);
		Mesh->SetPlayRate(0.6f); // run clip slowed to read as a walk
		bIdlePlaying = false;
	}
}

void ALongNoonNpc::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	WanderClock += DeltaSeconds;

	// Stand still and stay idle while the player is talking to us, or if wander is off.
	const bool bConversing = Dialogue && Dialogue->IsConversing();
	if (!bWander || bConversing)
	{
		PlayIdle();
		// Re-arm a pause so we don't immediately dart off when the talk ends.
		WanderState = EWanderState::Pausing;
		PauseTimer = PauseSeconds;
		return;
	}

	switch (WanderState)
	{
	case EWanderState::Pausing:
	{
		PlayIdle();
		PauseTimer -= DeltaSeconds;
		if (PauseTimer <= 0.0f)
		{
			BeginWalk();
		}
		break;
	}
	case EWanderState::Walking:
	{
		const FVector Current = GetActorLocation();
		FVector ToTarget = WanderTarget - Current;
		ToTarget.Z = 0.0f;
		const float Remaining = ToTarget.Size();

		const float Step = WalkSpeed * DeltaSeconds;
		if (Remaining <= Step || Remaining <= KINDA_SMALL_NUMBER)
		{
			// Arrived: snap to target and return to a pause.
			SetActorLocation(FVector(WanderTarget.X, WanderTarget.Y, Current.Z));
			WanderState = EWanderState::Pausing;
			PauseTimer = PauseSeconds;
			PlayIdle();
			break;
		}

		const FVector Dir = ToTarget / Remaining;
		SetActorLocation(Current + Dir * Step);

		// Face the direction of travel (yaw only).
		FRotator Rot = GetActorRotation();
		Rot.Yaw = Dir.Rotation().Yaw;
		SetActorRotation(Rot);
		break;
	}
	}
}
