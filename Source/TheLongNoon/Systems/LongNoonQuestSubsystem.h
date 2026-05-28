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

private:
	UPROPERTY()
	TSet<FName> CompletedObjectives;
};
