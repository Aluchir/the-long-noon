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
	PrimaryActorTick.bCanEverTick = false;

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
		}
		if (USkeletalMesh* Skm = Mesh->GetSkeletalMeshAsset())
		{
			const float H = FMath::Max(1.0f, Skm->GetBounds().BoxExtent.Z * 2.0f);
			const float S = TargetHeight / H;
			SetActorScale3D(FVector(S));
		}
	}

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
