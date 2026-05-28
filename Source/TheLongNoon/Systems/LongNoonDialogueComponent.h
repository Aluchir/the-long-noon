#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LongNoonDialogueComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueLine, const FText&, Line);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueEnded);

/**
 * Simple line-advancing dialogue for an NPC. Lines are warm on the surface;
 * the dagger lives in subtext, not a separate branch (see
 * docs/lore/tone-and-writing-style-guide.md). Optionally registers a lore
 * fragment with the codex the first time the player talks.
 */
UCLASS(ClassGroup = (LongNoon), meta = (BlueprintSpawnableComponent))
class THELONGNOON_API ULongNoonDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Dialogue")
	FOnDialogueLine OnDialogueLine;

	UPROPERTY(BlueprintAssignable, Category = "Dialogue")
	FOnDialogueEnded OnDialogueEnded;

	/** The NPC's lines for this conversation. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue", meta = (MultiLine = true))
	TArray<FText> Lines;

	/** Lore fragment id granted on first talk (None = none). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
	FName GrantsFragmentOnFirstTalk;

	/** Begin (or restart) the conversation from the first line. */
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void StartDialogue();

	/** Advance to the next line; broadcasts the line or ends the conversation. */
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void Advance();

private:
	int32 CurrentLine = -1;
	bool bHasTalked = false;
};
