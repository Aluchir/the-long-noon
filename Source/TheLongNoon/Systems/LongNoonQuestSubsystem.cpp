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

	// If the seeded quest just finished, fire its one-shot completion event.
	if (!bActiveQuestBroadcast && ActiveQuest.Num() > 0 && AreAllComplete(ActiveQuest, CompletedObjectives))
	{
		bActiveQuestBroadcast = true;
		OnActiveQuestCompleted.Broadcast();
		UE_LOG(LogLongNoon, Log, TEXT("[Quest] Active quest complete (%d objectives)."), ActiveQuest.Num());
	}
}

void ULongNoonQuestSubsystem::StartQuest(const TArray<FName>& Ordered)
{
	ActiveQuest = Ordered;
	bActiveQuestBroadcast = false;
	UE_LOG(LogLongNoon, Log, TEXT("[Quest] Quest started with %d objectives; active=%s."),
		ActiveQuest.Num(), *GetActiveQuestObjective().ToString());
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
