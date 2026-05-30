#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LongNoonPauseWidget.generated.h"

class UTextBlock;

/**
 * Keyboard-driven pause menu, built in C++ (no WBP, no UButton -> avoids the
 * HittestGrid C4459 issue). Shows a dimmed panel with options; handles its own keys
 * via NativeOnKeyDown (which fires even while the game is paused, since UI input is
 * not gated by pause): Esc/Enter resume, Q quits. The character pauses the game and
 * hands focus to this widget. See docs/art/style-guide.md.
 */
UCLASS()
class THELONGNOON_API ULongNoonPauseWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Unpause, remove the menu, and restore game input. */
	UFUNCTION(BlueprintCallable, Category = "Pause")
	void Resume();

protected:
	virtual void NativeConstruct() override;
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	void BuildTree();

	UPROPERTY(Transient) TObjectPtr<UTextBlock> TitleText;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> HintText;
};
