#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LongNoonSaveService.generated.h"

class ULongNoonSaveGame;

/**
 * Slot-based save/load (GameInstance subsystem). Gathers plain data from the game
 * instance and systems into a versioned USaveGame, and applies it back on load.
 * Steam Cloud is layered later at the platform milestone.
 * See docs/tech/coding-standards-and-architecture.md.
 */
UCLASS()
class THELONGNOON_API ULongNoonSaveService : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Save")
	bool SaveToSlot(const FString& SlotName);

	UFUNCTION(BlueprintCallable, Category = "Save")
	bool LoadFromSlot(const FString& SlotName);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Save")
	bool DoesSlotExist(const FString& SlotName) const;

private:
	/** Apply a loaded save, running migrations if its SaveVersion is older. */
	void ApplyLoadedSave(ULongNoonSaveGame* Save);
};
