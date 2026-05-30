#include "Systems/LongNoonGateBuild.h"
#include "Systems/LongNoonBuildingComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Core/LongNoonLog.h"
#include "Kismet/GameplayStatics.h"

ALongNoonGateBuild::ALongNoonGateBuild()
{
	PrimaryActorTick.bCanEverTick = false;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GateMesh"));
	SetRootComponent(Mesh);
}

void ALongNoonGateBuild::OnInteract_Implementation(AActor* Interactor)
{
	if (bBuilt || GateBuildId.IsNone() || !Interactor)
	{
		return;
	}

	// Route through the interactor's building component (the player carries one).
	if (ULongNoonBuildingComponent* Building = Interactor->FindComponentByClass<ULongNoonBuildingComponent>())
	{
		Building->CompleteGateBuild(GateBuildId, CompletesObjective);
		bBuilt = true;
		UE_LOG(LogLongNoon, Log, TEXT("[GateBuild] Built %s."), *GateBuildId.ToString());
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);

		// Walk through the gate into the region beyond, if one is set.
		if (!TargetLevel.IsNone())
		{
			UE_LOG(LogLongNoon, Log, TEXT("[GateBuild] Opening level %s."), *TargetLevel.ToString());
			UGameplayStatics::OpenLevel(this, TargetLevel);
		}
	}
}

FText ALongNoonGateBuild::GetInteractPrompt_Implementation() const
{
	return NSLOCTEXT("LongNoon", "BuildPrompt", "Raise the way");
}
