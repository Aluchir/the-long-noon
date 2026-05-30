#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LongNoonEndings.generated.h"

/** The three resolutions of the final choice. See docs/lore/endings.md. */
UENUM(BlueprintType)
enum class ELongNoonEnding : uint8
{
	None              UMETA(DisplayName = "None"),
	GiftReturned      UMETA(DisplayName = "The Gift Returned"),
	LongNoonContinues UMETA(DisplayName = "The Long Noon Continues"),
	ThirdWay          UMETA(DisplayName = "The Third Way")
};

/** What the player tells the Gardener at the First Garden. */
UENUM(BlueprintType)
enum class ELongNoonChoice : uint8
{
	GiveItBack   UMETA(DisplayName = "Give the ending back"),
	KeepTheNoon  UMETA(DisplayName = "Keep the Long Noon"),
	TheThirdWay  UMETA(DisplayName = "Return their memory (Third Way)")
};

/**
 * Pure resolution of the ending from the player's choice + progression. The Third
 * Way is only reachable if the player found the Rememberer (the optional deep-lore
 * path); choosing it otherwise is not selectable. See docs/design/progression-and-gating.md.
 */
UCLASS()
class THELONGNOON_API ULongNoonEndingLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Endings")
	static bool IsThirdWayAvailable(bool bRemembererFound) { return bRemembererFound; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Endings")
	static ELongNoonEnding ResolveEnding(ELongNoonChoice Choice, bool bRemembererFound);

	/** Short title for an ending (or the "not yet" title when the Third Way is locked). */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Endings")
	static FText GetEndingTitle(ELongNoonEnding Ending);

	/**
	 * The scripted ending beat + epilogue, condensed from docs/lore/endings.md.
	 * For None (Third Way attempted without the Rememberer) returns the gentle
	 * "the way is not open to you yet" text rather than empty.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Endings")
	static FText GetEndingText(ELongNoonEnding Ending);
};
