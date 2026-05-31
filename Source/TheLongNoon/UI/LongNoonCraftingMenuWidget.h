#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LongNoonCraftingMenuWidget.generated.h"

class UTextBlock;
class UVerticalBox;
class ULongNoonCraftingComponent;
class ULongNoonInventoryComponent;
class ULongNoonDataRegistry;

/**
 * Keyboard-driven crafting menu (C++ UMG, no UButton). Opened by a crafting station;
 * lists the recipes available at that station, shows each recipe's cost and whether the
 * player can afford it, and crafts the selected one through the player's crafting
 * component. Up/Down select, Enter craft, Esc close. See docs/design/crafting.md.
 */
UCLASS()
class THELONGNOON_API ULongNoonCraftingMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Wire the menu to the player's crafting component + the station it represents. */
	void Init(ULongNoonCraftingComponent* InCrafting, FName InStationId);

protected:
	virtual void NativeConstruct() override;
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	void BuildTree();
	void RefreshRows();
	void CraftSelected();
	void Close();

	UPROPERTY(Transient) TObjectPtr<UTextBlock> TitleText;
	UPROPERTY(Transient) TObjectPtr<UVerticalBox> ListBox;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> HintText;

	UPROPERTY(Transient) TWeakObjectPtr<ULongNoonCraftingComponent> Crafting;
	UPROPERTY(Transient) TWeakObjectPtr<ULongNoonInventoryComponent> Inventory;
	UPROPERTY(Transient) TWeakObjectPtr<ULongNoonDataRegistry> Registry;

	FName StationId;
	TArray<FName> RecipeIds;
	int32 Selected = 0;
};
