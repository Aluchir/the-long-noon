#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LongNoonEndingWidget.generated.h"

class UTextBlock;

/**
 * Full-screen ending sequence (C++ UMG, no UButton). Shown when the player makes the
 * final choice at the First Garden: the ending's title and epilogue over a quiet dark
 * field, then a prompt back to the title screen. Keyboard-driven. See docs/lore/endings.md.
 */
UCLASS()
class THELONGNOON_API ULongNoonEndingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Set the ending title + epilogue to display. */
	void SetEnding(const FText& InTitle, const FText& InBody);

protected:
	virtual void NativeConstruct() override;
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	void BuildTree();

	UPROPERTY(Transient) TObjectPtr<UTextBlock> TitleText;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> BodyText;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> PromptText;

	/** Map returned to when the player dismisses the ending. */
	FName ReturnLevel = TEXT("L_MainMenu");
};
