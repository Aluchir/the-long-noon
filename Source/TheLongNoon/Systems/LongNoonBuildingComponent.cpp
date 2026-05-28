#include "Systems/LongNoonBuildingComponent.h"
#include "Systems/LongNoonRegionSubsystem.h"
#include "Core/LongNoonLog.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"

bool ULongNoonBuildingComponent::PlacePiece(FName PieceId, const FTransform& Where)
{
	if (PieceId.IsNone())
	{
		return false;
	}

	// TODO: consume crafted components from inventory, snap to grid/socket, spawn the
	// placed actor, register stations/storage. See docs/design/building.md.
	UE_LOG(LogLongNoon, Verbose, TEXT("[Building] Place %s at %s."),
		*PieceId.ToString(), *Where.GetLocation().ToString());
	return true;
}

void ULongNoonBuildingComponent::CompleteGateBuild(FName GateBuildId)
{
	if (GateBuildId.IsNone())
	{
		return;
	}

	if (const UWorld* World = GetWorld())
	{
		if (UGameInstance* GI = World->GetGameInstance())
		{
			if (ULongNoonRegionSubsystem* Regions = GI->GetSubsystem<ULongNoonRegionSubsystem>())
			{
				Regions->MarkGateBuildComplete(GateBuildId);
			}
		}
	}

	UE_LOG(LogLongNoon, Log, TEXT("[Building] Gate-build complete: %s."), *GateBuildId.ToString());
}
