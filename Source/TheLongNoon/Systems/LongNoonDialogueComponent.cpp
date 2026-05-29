#include "Systems/LongNoonDialogueComponent.h"
#include "Systems/LongNoonCodexSubsystem.h"
#include "Systems/LongNoonDataRegistry.h"
#include "Core/LongNoonLog.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"

bool ULongNoonDialogueComponent::LoadFromData()
{
	if (NpcId.IsNone())
	{
		return false;
	}

	const UWorld* World = GetWorld();
	UGameInstance* GI = World ? World->GetGameInstance() : nullptr;
	const ULongNoonDataRegistry* Registry = GI ? GI->GetSubsystem<ULongNoonDataRegistry>() : nullptr;
	if (!Registry)
	{
		return false;
	}

	const FNpcRow* Npc = Registry->GetNpc(NpcId);
	if (!Npc)
	{
		UE_LOG(LogLongNoon, Warning, TEXT("[Dialogue] No NPC row '%s'."), *NpcId.ToString());
		return false;
	}

	Lines.Reset();
	Lines.Append(Registry->GetDialogueLines(Npc->FirstTalkLines));
	Lines.Append(Registry->GetDialogueLines(Npc->IdleLines));
	Lines.Append(Registry->GetDialogueLines(Npc->StoryBeatLines));
	if (!Npc->DaggerLine.IsNone())
	{
		Lines.Append(Registry->GetDialogueLines(Npc->DaggerLine.ToString()));
	}

	GrantsFragmentOnFirstTalk = Npc->GrantsFragmentOnFirstTalk;
	return Lines.Num() > 0;
}

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
