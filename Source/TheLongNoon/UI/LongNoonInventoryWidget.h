#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LongNoonInventoryWidget.generated.h"

class UTextBlock;
class UVerticalBox;

/**
 * Inventory screen (C++ UMG, keyboard-driven, no UButton). Lists what the player is
 * carrying (item id + count) from the pawn's inventory component, using the registry's
 * display names where available. Self-sufficient: finds the inventory from the owning
 * player's pawn on construct. Esc closes.
 */
UCLASS()
class THELONGNOON_API ULongNoonInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	void BuildTree();
	void Refresh();
	void Close();

	UPROPERTY(Transient) TObjectPtr<UTextBlock> TitleText;
	UPROPERTY(Transient) TObjectPtr<UVerticalBox> ListBox;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> HintText;
};
