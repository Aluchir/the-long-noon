#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LongNoonSaveGame.generated.h"

/**
 * Serialized game state. VERSIONED from day one (SaveVersion + migration path) so
 * QA's save-compatibility testing has something to migrate against. Store plain
 * data; rebuild actors on load rather than serializing live actors.
 * See docs/tech/coding-standards-and-architecture.md.
 */
UCLASS()
class THELONGNOON_API ULongNoonSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	/** Bump when the schema changes; migrate older saves forward. */
	UPROPERTY(VisibleAnywhere, Category = "Save")
	int32 SaveVersion = 1;

	UPROPERTY(VisibleAnywhere, Category = "Save")
	int32 ScriptLiteracyTier = 0;

	UPROPERTY(VisibleAnywhere, Category = "Save")
	bool bRemembererFound = false;

	UPROPERTY(VisibleAnywhere, Category = "Save")
	int32 MaxToolTier = 1;

	UPROPERTY(VisibleAnywhere, Category = "Save")
	TArray<FName> UnlockedRegions;

	UPROPERTY(VisibleAnywhere, Category = "Save")
	TMap<FName, int32> InventoryStacks;

	UPROPERTY(VisibleAnywhere, Category = "Save")
	TSet<FName> FoundFragments;

	UPROPERTY(VisibleAnywhere, Category = "Save")
	FName CurrentRegionId;
};
