#include "Core/LongNoonGameInstance.h"
#include "Core/LongNoonEventSubsystem.h"

void ULongNoonGameInstance::SetScriptLiteracyTier(int32 NewTier)
{
	NewTier = FMath::Clamp(NewTier, 0, 4);
	if (NewTier == ScriptLiteracyTier)
	{
		return;
	}

	ScriptLiteracyTier = NewTier;

	if (ULongNoonEventSubsystem* Events = GetSubsystem<ULongNoonEventSubsystem>())
	{
		Events->BroadcastScriptLiteracyChanged(ScriptLiteracyTier);
	}
}
