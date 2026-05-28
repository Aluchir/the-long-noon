#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LongNoonHUDWidget.generated.h"

/**
 * Base class for the in-game HUD widget. Bind cozy, scalable, colorblind-safe UMG
 * on top of this in a WBP_HUD Blueprint (see docs/art/style-guide.md and the UI module).
 * Exposes BlueprintImplementableEvents the systems can call to refresh the display.
 */
UCLASS(Abstract)
class THELONGNOON_API ULongNoonHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Update the Tend meters (Stamina/Focus 0..max). Implement in the WBP. */
	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void UpdateTend(float Stamina, float Focus, float Comfort);

	/** Show a transient interact prompt ("Talk", "Gather", "Read"); empty hides it. */
	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void SetInteractPrompt(const FText& Prompt);

	/** Notify that a lore fragment was found (for a gentle toast). */
	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void OnLoreFound(FName FragmentId);
};
