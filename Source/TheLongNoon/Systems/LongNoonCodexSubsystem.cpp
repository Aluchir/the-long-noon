#include "Systems/LongNoonCodexSubsystem.h"
#include "Data/LoreFragmentDef.h"
#include "Core/LongNoonGameInstance.h"
#include "Core/LongNoonEventSubsystem.h"

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
