#include "Systems/LongNoonDialogueComponent.h"
#include "Systems/LongNoonCodexSubsystem.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"

void ULongNoonDialogueComponent::StartDialogue()
{
	if (Lines.Num() == 0)
	{
		return;
	}

	if (!bHasTalked && !GrantsFragmentOnFirstTalk.IsNone())
	{
		if (const UWorld* World = GetWorld())
		{
			if (UGameInstance* GI = World->GetGameInstance())
			{
				if (ULongNoonCodexSubsystem* Codex = GI->GetSubsystem<ULongNoonCodexSubsystem>())
				{
					Codex->RegisterFound(GrantsFragmentOnFirstTalk);
				}
			}
		}
	}
	bHasTalked = true;

	CurrentLine = -1;
	Advance();
}

void ULongNoonDialogueComponent::Advance()
{
	CurrentLine++;
	if (Lines.IsValidIndex(CurrentLine))
	{
		OnDialogueLine.Broadcast(Lines[CurrentLine]);
	}
	else
	{
		CurrentLine = -1;
		OnDialogueEnded.Broadcast();
	}
}
