#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LongNoonHUDWidget.generated.h"

class UProgressBar;
class UTextBlock;

/**
 * In-game HUD. Builds its own UMG tree in C++ (no hand-authored WBP required):
 * Stamina/Focus meters, a transient interact prompt, and a lore-found toast.
 * The character drives these via the forwarding methods below on BeginPlay.
 * A designer can still subclass this in a WBP for a richer cozy look later
 * (see docs/art/style-guide.md), but the default is fully functional.
 */
UCLASS()
class THELONGNOON_API ULongNoonHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Update the Tend meters (Stamina/Focus 0..100, Comfort 0..1). */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateTend(float Stamina, float Focus, float Comfort);

	/** Show a transient interact prompt ("Talk", "Gather", "Prune"); empty hides it. */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetInteractPrompt(const FText& Prompt);

	/** Show a gentle lore-found toast. */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void OnLoreFound(FName FragmentId);

	/** Show arbitrary toast text (ending beats, hints); empty hides it. */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowToast(const FText& Message);

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

	void BuildTree();

	UPROPERTY(Transient) TObjectPtr<UProgressBar> StaminaBar;
	UPROPERTY(Transient) TObjectPtr<UProgressBar> FocusBar;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> PromptText;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> ToastText;

	/** Max meter value the bars normalise against (matches ULongNoonTendComponent). */
	float MaxMeter = 100.0f;
};
