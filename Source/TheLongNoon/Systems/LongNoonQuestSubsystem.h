#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LongNoonQuestSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnObjectiveCompleted, FName, ObjectiveId);

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

	UFUNCTION(BlueprintCallable, Category = "Quest")
	void CompleteObjective(FName ObjectiveId);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Quest")
	bool IsObjectiveComplete(FName ObjectiveId) const { return CompletedObjectives.Contains(ObjectiveId); }

	/** The first objective in the ordered list not yet completed (drives the journal/HUD). */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Quest")
	FName GetActiveObjective(const TArray<FName>& Ordered) const { return ActiveObjective(Ordered, CompletedObjectives); }

	/** Pure quest-sequence helpers (no GameInstance needed); unit-testable. */
	static FName ActiveObjective(const TArray<FName>& Ordered, const TSet<FName>& Completed);
	static bool AreAllComplete(const TArray<FName>& Ordered, const TSet<FName>& Completed);

private:
	UPROPERTY()
	TSet<FName> CompletedObjectives;
};
