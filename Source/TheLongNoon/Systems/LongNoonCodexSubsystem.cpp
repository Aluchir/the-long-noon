#include "Systems/LongNoonCodexSubsystem.h"
#include "Data/LoreFragmentDef.h"
#include "Core/LongNoonGameInstance.h"
#include "Core/LongNoonEventSubsystem.h"
#include "Core/LongNoonLog.h"

void ULongNoonCodexSubsystem::RegisterFound(FName FragmentId)
{
	if (FragmentId.IsNone() || FoundFragments.Contains(FragmentId))
	{
		return;
	}

	FoundFragments.Add(FragmentId);

	if (ULongNoonEventSubsystem* Events = GetGameInstance()->GetSubsystem<ULongNoonEventSubsystem>())
	{
		Events->BroadcastLoreFragmentFound(FragmentId);
	}

	// Finding fragments is the parallel progression: enough of them raises literacy,
	// which retroactively unlocks readable translations.
	if (ULongNoonGameInstance* GI = Cast<ULongNoonGameInstance>(GetGameInstance()))
	{
		const int32 NewTier = LiteracyTierForFragmentCount(FoundFragments.Num());
		if (NewTier > GI->ScriptLiteracyTier)
		{
			GI->SetScriptLiteracyTier(NewTier);
			UE_LOG(LogLongNoon, Log, TEXT("[Codex] Literacy raised to tier %d (%d fragments found)."),
				NewTier, FoundFragments.Num());
		}
	}
}

int32 ULongNoonCodexSubsystem::LiteracyTierForFragmentCount(int32 FragmentsFound)
{
	static const int32 Thresholds[] = { 2, 5, 10, 15 };
	int32 Tier = 0;
	for (int32 T : Thresholds)
	{
		if (FragmentsFound >= T)
		{
			++Tier;
		}
	}
	return Tier;
}

bool ULongNoonCodexSubsystem::CanRead(const ULoreFragmentDef* Fragment) const
{
	if (!Fragment)
	{
		return false;
	}

	const ULongNoonGameInstance* GI = Cast<ULongNoonGameInstance>(GetGameInstance());
	const int32 Literacy = GI ? GI->ScriptLiteracyTier : 0;
	return IsLiteracyEnough(Fragment->RequiredLiteracyTier, Literacy);
}

FText ULongNoonCodexSubsystem::GetReadableText(const ULoreFragmentDef* Fragment) const
{
	if (!Fragment)
	{
		return FText::GetEmpty();
	}

	return CanRead(Fragment) ? Fragment->TranslatedText : Fragment->GlyphText;
}
