#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LongNoonQuestBoard.generated.h"

class UStaticMeshComponent;

/**
 * Seeds a region's quest. On BeginPlay it hands its ordered objective list to the
 * quest subsystem (StartQuest), so the journal/HUD can show the active step and the
 * subsystem can fire OnActiveQuestCompleted when the loop is done. One per region
 * greybox; the objective ids match the placed interactables (gather/craft/prune/build).
 * See docs/design/quest-flow.md.
 */
UCLASS()
class THELONGNOON_API ALongNoonQuestBoard : public AActor
{
	GENERATED_BODY()

public:
	ALongNoonQuestBoard();

	/** Ordered objective ids for this region's quest. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	TArray<FName> Objectives;

	/** Seed the quest automatically on BeginPlay. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	bool bAutoStart = true;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest")
	TObjectPtr<UStaticMeshComponent> Mesh;
};
