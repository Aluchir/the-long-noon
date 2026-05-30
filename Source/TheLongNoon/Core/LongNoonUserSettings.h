#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LongNoonUserSettings.generated.h"

/**
 * Persistent user preferences (audio + input) that survive an app relaunch.
 *
 * Backed by USaveGame and stored in a fixed slot ("LongNoonSettings", index 0) so
 * callers never have to manage slot bookkeeping -- use the static Load/Save/Apply
 * helpers below.
 *
 * Value conventions:
 *  - MasterVolume is NORMALIZED 0..1 (default 0.7). Note the settings widget
 *    (ULongNoonSettingsWidget) keeps its master volume in PERCENT 0..100; convert
 *    when wiring the two together (percent = MasterVolume * 100).
 *  - MouseSensitivity is a multiplier in the range 0.2 .. 3.0 (default 1.0), matching
 *    the widget's range exactly.
 *
 * See docs/tech/coding-standards-and-architecture.md.
 */
UCLASS()
class THELONGNOON_API ULongNoonUserSettings : public USaveGame
{
	GENERATED_BODY()

public:
	/** Master output volume, normalized 0..1. */
	UPROPERTY()
	float MasterVolume = 0.7f;

	/** Mouse sensitivity multiplier (0.2 .. 3.0). */
	UPROPERTY()
	float MouseSensitivity = 1.0f;

	/** Fixed save slot name; callers should not need to reference this directly. */
	static const TCHAR* SlotName() { return TEXT("LongNoonSettings"); }

	/** Fixed user index for the save slot. */
	static constexpr int32 UserIndex = 0;

	/**
	 * Load settings from the fixed slot. If the slot is missing or fails to load,
	 * returns a fresh instance carrying the class defaults. Never returns null.
	 */
	static ULongNoonUserSettings* Load();

	/** Save the given settings to the fixed slot. No-op if In is null. */
	static void Save(ULongNoonUserSettings* In);

	/**
	 * Apply settings to the running game. Pushes MasterVolume to the audio engine via
	 * the "au.MasterVolume X" console command on the first local PlayerController.
	 * Mouse sensitivity is applied by callers/UI (it has no single engine sink here).
	 * Guards a null WorldContext / world / PlayerController and is a no-op if In is null.
	 */
	static void Apply(const UObject* WorldContext, ULongNoonUserSettings* In);
};
