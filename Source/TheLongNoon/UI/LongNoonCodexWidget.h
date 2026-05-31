#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LongNoonCodexWidget.generated.h"

class UTextBlock;
class UVerticalBox;
class ULongNoonCodexSubsystem;
class ULongNoonDataRegistry;

/**
 * The codex / journal screen (C++ UMG, keyboard-driven, no UButton). Lists the lore
 * fragments the player has found and shows the selected one's text - the forgotten
 * glyphs until script literacy is high enough, then the translation. Self-sufficient:
 * pulls the codex subsystem, data registry, and literacy tier from the GameInstance on
 * construct. Up/Down select, Esc close. See docs/lore/forgotten-script.md.
 */
UCLASS()
class THELONGNOON_API ULongNoonCodexWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	void BuildTree();
	void Gather();
	void Refresh();
	void Close();

	UPROPERTY(Transient) TObjectPtr<UTextBlock> TitleText;
	UPROPERTY(Transient) TObjectPtr<UVerticalBox> ListBox;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> BodyText;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> HintText;

	UPROPERTY(Transient) TWeakObjectPtr<ULongNoonCodexSubsystem> Codex;
	UPROPERTY(Transient) TWeakObjectPtr<ULongNoonDataRegistry> Registry;

	TArray<FName> Found;
	int32 Selected = 0;
	int32 Literacy = 0;
};
