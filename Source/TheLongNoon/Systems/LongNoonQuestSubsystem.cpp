#include "Systems/LongNoonQuestSubsystem.h"
#include "Core/LongNoonLog.h"

void ULongNoonQuestSubsystem::CompleteObjective(FName ObjectiveId)
{
	if (ObjectiveId.IsNone() || CompletedObjectives.Contains(ObjectiveId))
	{
		return;
	}

	CompletedObjectives.Add(ObjectiveId);
	OnObjectiveCompleted.Broadcast(ObjectiveId);
	UE_LOG(LogLongNoon, Log, TEXT("[Quest] Objective complete: %s."), *ObjectiveId.ToString());
}
