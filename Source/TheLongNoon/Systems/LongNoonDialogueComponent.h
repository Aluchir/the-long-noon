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

	/**
	 * Which NPC this component speaks for. If set, LoadFromData() pulls this NPC's
	 * roster row + dialogue lines from the registry (NPCs.csv / DialogueLines.csv).
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
	FName NpcId;

	/**
	 * Populate Lines (and the first-talk fragment) from the data registry using NpcId.
	 * Loads first-talk, idle, and story-beat lines in order, then the dagger line.
	 * Returns false if the registry or the NPC row is unavailable.
	 */
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	bool LoadFromData();

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
