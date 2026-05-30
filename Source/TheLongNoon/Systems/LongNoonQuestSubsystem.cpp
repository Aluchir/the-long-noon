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

FName ULongNoonQuestSubsystem::ActiveObjective(const TArray<FName>& Ordered, const TSet<FName>& Completed)
{
	for (const FName& Obj : Ordered)
	{
		if (!Completed.Contains(Obj))
		{
			return Obj;
		}
	}
	return NAME_None;
}

bool ULongNoonQuestSubsystem::AreAllComplete(const TArray<FName>& Ordered, const TSet<FName>& Completed)
{
	for (const FName& Obj : Ordered)
	{
		if (!Completed.Contains(Obj))
		{
			return false;
		}
	}
	return true;
}
