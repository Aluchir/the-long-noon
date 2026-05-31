#include "UI/LongNoonCraftingMenuWidget.h"
#include "UI/LongNoonUIStyle.h"
#include "Systems/LongNoonCraftingComponent.h"
#include "Systems/LongNoonInventoryComponent.h"
#include "Systems/LongNoonDataRegistry.h"
#include "Data/LongNoonRowTypes.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/VerticalBox.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"

void ULongNoonCraftingMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetIsFocusable(true);
	SetKeyboardFocus();
}

TSharedRef<SWidget> ULongNoonCraftingMenuWidget::RebuildWidget()
{
	if (!ListBox)
	{
		BuildTree();
	}
	return Super::RebuildWidget();
}

void ULongNoonCraftingMenuWidget::BuildTree()
{
	if (!WidgetTree)
	{
		return;
	}

	UCanvasPanel* Root = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("CraftRoot"));
	WidgetTree->RootWidget = Root;

	UBorder* Dim = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("Dim"));
	Dim->SetBrushColor(LongNoonUI::PanelTint());
	if (UCanvasPanelSlot* CS = Root->AddChildToCanvas(Dim))
	{
		CS->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
		CS->SetOffsets(FMargin(0.0f));
	}

	TitleText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("CraftTitle"));
	TitleText->SetText(NSLOCTEXT("LongNoon", "CraftTitle", "Crafting"));
	LongNoonUI::StyleText(TitleText, LongNoonUI::HeadingFont(36.0f), LongNoonUI::Gold());
	if (UCanvasPanelSlot* CS = Root->AddChildToCanvas(TitleText))
	{
		CS->SetAnchors(FAnchors(0.5f, 0.18f));
		CS->SetAlignment(FVector2D(0.5f, 0.5f));
		CS->SetAutoSize(true);
	}

	ListBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("CraftList"));
	if (UCanvasPanelSlot* CS = Root->AddChildToCanvas(ListBox))
	{
		CS->SetAnchors(FAnchors(0.5f, 0.28f));
		CS->SetAlignment(FVector2D(0.5f, 0.0f));
		CS->SetAutoSize(true);
	}

	HintText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("CraftHint"));
	HintText->SetText(NSLOCTEXT("LongNoon", "CraftHint", "[Up/Down] select     [Enter] craft     [Esc] leave"));
	LongNoonUI::StyleText(HintText, LongNoonUI::BodyFont(18.0f), LongNoonUI::Cream());
	if (UCanvasPanelSlot* CS = Root->AddChildToCanvas(HintText))
	{
		CS->SetAnchors(FAnchors(0.5f, 0.86f));
		CS->SetAlignment(FVector2D(0.5f, 0.5f));
		CS->SetAutoSize(true);
	}
}

void ULongNoonCraftingMenuWidget::Init(ULongNoonCraftingComponent* InCrafting, FName InStationId)
{
	Crafting = InCrafting;
	StationId = InStationId;

	if (InCrafting)
	{
		if (AActor* Owner = InCrafting->GetOwner())
		{
			Inventory = Owner->FindComponentByClass<ULongNoonInventoryComponent>();
		}
		if (const UWorld* World = InCrafting->GetWorld())
		{
			if (UGameInstance* GI = World->GetGameInstance())
			{
				Registry = GI->GetSubsystem<ULongNoonDataRegistry>();
			}
		}
		// Stand the player at this station so CraftById's station gate passes.
		InCrafting->CurrentStation = InStationId;
	}

	// Recipes craftable here: this station's, plus station-agnostic ones.
	RecipeIds.Reset();
	if (Registry.IsValid())
	{
		for (const FName& Id : Registry->GetRecipeIds())
		{
			const FRecipeRow* Row = Registry->GetRecipe(Id);
			if (Row && (Row->RequiredStation.IsNone() || Row->RequiredStation == StationId))
			{
				RecipeIds.Add(Id);
			}
		}
	}
	Selected = 0;
	RefreshRows();
}

void ULongNoonCraftingMenuWidget::RefreshRows()
{
	if (!ListBox)
	{
		return;
	}
	ListBox->ClearChildren();

	if (RecipeIds.Num() == 0)
	{
		UTextBlock* Empty = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
		Empty->SetText(NSLOCTEXT("LongNoon", "CraftNone", "Nothing can be made at this bench yet."));
		LongNoonUI::StyleText(Empty, LongNoonUI::BodyFont(22.0f), LongNoonUI::Cream());
		ListBox->AddChildToVerticalBox(Empty);
		return;
	}

	for (int32 i = 0; i < RecipeIds.Num(); ++i)
	{
		const FRecipeRow* Row = Registry.IsValid() ? Registry->GetRecipe(RecipeIds[i]) : nullptr;

		// Affordable if the inventory holds every parsed input.
		bool bAfford = true;
		if (Row && Inventory.IsValid())
		{
			for (const FParsedRecipeInput& In : ULongNoonDataRegistry::ParseRecipeInputs(Row->Inputs))
			{
				if (!Inventory->HasItem(In.ItemId, In.Quantity))
				{
					bAfford = false;
					break;
				}
			}
		}
		else
		{
			bAfford = false;
		}

		const FString Caret = (i == Selected) ? TEXT("> ") : TEXT("   ");
		const FString OutName = Row ? Row->Output.ToString() : RecipeIds[i].ToString();
		const FString Status = bAfford ? TEXT("") : TEXT("   (need materials)");
		const FString Label = FString::Printf(TEXT("%s%s%s"), *Caret, *OutName, *Status);

		UTextBlock* RowText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
		RowText->SetText(FText::FromString(Label));
		LongNoonUI::StyleText(RowText, LongNoonUI::BodyFont(22.0f),
			bAfford ? LongNoonUI::Cream() : FLinearColor(0.55f, 0.5f, 0.45f, 1.0f));
		ListBox->AddChildToVerticalBox(RowText);
	}
}

void ULongNoonCraftingMenuWidget::CraftSelected()
{
	if (!Crafting.IsValid() || !RecipeIds.IsValidIndex(Selected))
	{
		return;
	}
	const bool bOk = Crafting->CraftById(RecipeIds[Selected]);
	if (bOk && TitleText)
	{
		TitleText->SetText(NSLOCTEXT("LongNoon", "CraftMade", "Made it."));
	}
	RefreshRows();
}

FReply ULongNoonCraftingMenuWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const FKey Key = InKeyEvent.GetKey();
	if (Key == EKeys::Up || Key == EKeys::Gamepad_DPad_Up)
	{
		if (RecipeIds.Num() > 0) { Selected = (Selected - 1 + RecipeIds.Num()) % RecipeIds.Num(); RefreshRows(); }
		return FReply::Handled();
	}
	if (Key == EKeys::Down || Key == EKeys::Gamepad_DPad_Down)
	{
		if (RecipeIds.Num() > 0) { Selected = (Selected + 1) % RecipeIds.Num(); RefreshRows(); }
		return FReply::Handled();
	}
	if (Key == EKeys::Enter || Key == EKeys::SpaceBar)
	{
		CraftSelected();
		return FReply::Handled();
	}
	if (Key == EKeys::Escape || Key == EKeys::Gamepad_Special_Right)
	{
		Close();
		return FReply::Handled();
	}
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void ULongNoonCraftingMenuWidget::Close()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		PC->SetInputMode(FInputModeGameOnly());
		PC->bShowMouseCursor = false;
		PC->FlushPressedKeys();
	}
	RemoveFromParent();
}
