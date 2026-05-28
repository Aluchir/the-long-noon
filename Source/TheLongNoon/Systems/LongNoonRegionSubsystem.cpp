#include "Systems/LongNoonRegionSubsystem.h"
#include "Data/RegionDef.h"
#include "Core/LongNoonGameInstance.h"
#include "Core/LongNoonEventSubsystem.h"

bool ULongNoonRegionSubsystem::CanEnterRegion(const URegionDef* Region) const
{
	if (!Region)
	{
		return false;
	}

	const ULongNoonGameInstance* GI = Cast<ULongNoonGameInstance>(GetGameInstance());
	const int32 ToolTier = GI ? GI->MaxToolTier : 1;

	if (ToolTier < Region->RequiredToolTier)
	{
		return false;
	}
	if (!Region->RequiredTraversal.IsNone() && !AcquiredTraversals.Contains(Region->RequiredTraversal))
	{
		return false;
	}
	if (!Region->RequiredGateBuild.IsNone() && !CompletedGateBuilds.Contains(Region->RequiredGateBuild))
	{
		return false;
	}

	return true;
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
