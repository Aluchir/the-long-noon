#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LongNoonMainMenuWidget.generated.h"

class UTextBlock;

/**
 * Title screen, built in C++ (no WBP, no UButton). Shows the game title and a
 * keyboard-driven prompt; NativeOnKeyDown handles Enter/Space (begin -> open the
 * first region) and Q/Esc (quit). The menu GameMode creates it and hands it focus.
 */
UCLASS()
class THELONGNOON_API ULongNoonMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Map opened when the player begins. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Menu")
	FName StartLevel = TEXT("L_Sunhollow_Greybox");

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	void BuildTree();

	UPROPERTY(Transient) TObjectPtr<UTextBlock> TitleText;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> SubtitleText;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> PromptText;
};
