#include "Core/LongNoonGameInstance.h"
#include "Core/LongNoonEventSubsystem.h"
#include "Core/LongNoonLog.h"

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

void ULongNoonGameInstance::SetRemembererFound()
{
	if (bRemembererFound)
	{
		return;
	}
	bRemembererFound = true;
	UE_LOG(LogLongNoon, Log, TEXT("[Progression] The Rememberer is found. The Third Way opens."));
}
