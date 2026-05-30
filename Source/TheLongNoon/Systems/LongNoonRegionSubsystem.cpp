#include "Systems/LongNoonRegionSubsystem.h"
#include "Data/RegionDef.h"
#include "Core/LongNoonGameInstance.h"
#include "Core/LongNoonEventSubsystem.h"

bool ULongNoonRegionSubsystem::CanEnter(int32 RequiredToolTier, FName RequiredTraversal, FName RequiredGateBuild,
	int32 CurrentToolTier, const TSet<FName>& Traversals, const TSet<FName>& GateBuilds)
{
	if (CurrentToolTier < RequiredToolTier)
	{
		return false;
	}
	if (!RequiredTraversal.IsNone() && !Traversals.Contains(RequiredTraversal))
	{
		return false;
	}
	if (!RequiredGateBuild.IsNone() && !GateBuilds.Contains(RequiredGateBuild))
	{
		return false;
	}
	return true;
}

bool ULongNoonRegionSubsystem::CanEnterRegion(const URegionDef* Region) const
{
	if (!Region)
	{
		return false;
	}

	const ULongNoonGameInstance* GI = Cast<ULongNoonGameInstance>(GetGameInstance());
	const int32 ToolTier = GI ? GI->MaxToolTier : 1;

	return CanEnter(Region->RequiredToolTier, Region->RequiredTraversal, Region->RequiredGateBuild,
		ToolTier, AcquiredTraversals, CompletedGateBuilds);
}

void ULongNoonRegionSubsystem::UnlockRegion(const URegionDef* Region)
{
	if (!Region)
	{
		return;
	}

	if (ULongNoonGameInstance* GI = Cast<ULongNoonGameInstance>(GetGameInstance()))
	{
		bool bAlready = false;
		GI->UnlockedRegions.Add(Region->RegionId, &bAlready);
		if (bAlready)
		{
			return;
		}
	}

	if (ULongNoonEventSubsystem* Events = GetGameInstance()->GetSubsystem<ULongNoonEventSubsystem>())
	{
		Events->BroadcastRegionUnlocked(Region->RegionId);
	}
}
