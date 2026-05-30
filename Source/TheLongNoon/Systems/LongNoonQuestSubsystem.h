#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LongNoonQuestSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnObjectiveCompleted, FName, ObjectiveId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActiveQuestCompleted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuestStarted);

/**
 * Lightweight objective tracking. Quests/beats are authored as objective ids per
 * region (see docs/design/quest-flow.md); systems complete them and the HUD/journal
 * react. Persisted via the SaveService.
 */
UCLASS()
class THELONGNOON_API ULongNoonQuestSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Quest")
	FOnObjectiveCompleted OnObjectiveCompleted;

	/** Fires once when every objective of the active quest has been completed. */
	UPROPERTY(BlueprintAssignable, Category = "Quest")
	FOnActiveQuestCompleted OnActiveQuestCompleted;

	/** Fires when a new quest is seeded (so UI can show the first objective). */
	UPROPERTY(BlueprintAssignable, Category = "Quest")
	FOnQuestStarted OnQuestStarted;

	UFUNCTION(BlueprintCallable, Category = "Quest")
	void CompleteObjective(FName ObjectiveId);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Quest")
	bool IsObjectiveComplete(FName ObjectiveId) const { return CompletedObjectives.Contains(ObjectiveId); }

	/** Seed the active quest with an ordered objective list (a level/region quest board). */
	UFUNCTION(BlueprintCallable, Category = "Quest")
	void StartQuest(const TArray<FName>& Ordered);

	/** The first objective in the ordered list not yet completed (drives the journal/HUD). */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Quest")
	FName GetActiveObjective(const TArray<FName>& Ordered) const { return ActiveObjective(Ordered, CompletedObjectives); }

	/** The active objective of the seeded quest (None if no quest or all complete). */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Quest")
	FName GetActiveQuestObjective() const { return ActiveObjective(ActiveQuest, CompletedObjectives); }

	/** True once the seeded quest is fully complete. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Quest")
	bool IsActiveQuestComplete() const { return ActiveQuest.Num() > 0 && AreAllComplete(ActiveQuest, CompletedObjectives); }

	/** Pure quest-sequence helpers (no GameInstance needed); unit-testable. */
	static FName ActiveObjective(const TArray<FName>& Ordered, const TSet<FName>& Completed);
	static bool AreAllComplete(const TArray<FName>& Ordered, const TSet<FName>& Completed);

private:
	UPROPERTY()
	TSet<FName> CompletedObjectives;

	/** Ordered objective ids of the currently seeded quest. */
	UPROPERTY()
	TArray<FName> ActiveQuest;

	/** Guards the one-shot OnActiveQuestCompleted broadcast. */
	bool bActiveQuestBroadcast = false;
};
