#include "UI/LongNoonCodexWidget.h"
#include "UI/LongNoonUIStyle.h"
#include "Systems/LongNoonCodexSubsystem.h"
#include "Systems/LongNoonDataRegistry.h"
#include "Core/LongNoonGameInstance.h"
#include "Data/LongNoonRowTypes.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/VerticalBox.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerController.h"
#include "Engine/GameInstance.h"

void ULongNoonCodexWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetIsFocusable(true);
	SetKeyboardFocus();

	if (UGameInstance* GI = GetGameInstance())
	{
		Codex = GI->GetSubsystem<ULongNoonCodexSubsystem>();
		Registry = GI->GetSubsystem<ULongNoonDataRegistry>();
		if (const ULongNoonGameInstance* LNGI = Cast<ULongNoonGameInstance>(GI))
		{
			Literacy = LNGI->ScriptLiteracyTier;
		}
	}
	Gather();
	Refresh();
}

TSharedRef<SWidget> ULongNoonCodexWidget::RebuildWidget()
{
	if (!ListBox)
	{
		BuildTree();
	}
	return Super::RebuildWidget();
}

void ULongNoonCodexWidget::BuildTree()
{
	if (!WidgetTree)
	{
		return;
	}

	UCanvasPanel* Root = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("CodexRoot"));
	WidgetTree->RootWidget = Root;

	UBorder* Bg = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("Bg"));
	Bg->SetBrushColor(LongNoonUI::Ink());
	if (UCanvasPanelSlot* CS = Root->AddChildToCanvas(Bg))
	{
		CS->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
		CS->SetOffsets(FMargin(0.0f));
	}

	TitleText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("CodexTitle"));
	LongNoonUI::StyleText(TitleText, LongNoonUI::HeadingFont(34.0f), LongNoonUI::Gold());
	if (UCanvasPanelSlot* CS = Root->AddChildToCanvas(TitleText))
	{
		CS->SetAnchors(FAnchors(0.5f, 0.08f));
		CS->SetAlignment(FVector2D(0.5f, 0.5f));
		CS->SetAutoSize(true);
	}

	// Found-fragment list, left column.
	ListBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("CodexList"));
	if (UCanvasPanelSlot* CS = Root->AddChildToCanvas(ListBox))
	{
		CS->SetAnchors(FAnchors(0.08f, 0.2f));
		CS->SetAlignment(FVector2D(0.0f, 0.0f));
		CS->SetAutoSize(true);
	}

	// Selected fragment's text, right side, wrapped.
	BodyText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("CodexBody"));
	BodyText->SetAutoWrapText(true);
	LongNoonUI::StyleText(BodyText, LongNoonUI::BodyFont(22.0f), LongNoonUI::Cream());
	if (UCanvasPanelSlot* CS = Root->AddChildToCanvas(BodyText))
	{
		CS->SetAnchors(FAnchors(0.45f, 0.2f, 0.92f, 0.85f));
		CS->SetOffsets(FMargin(0.0f));
	}

	HintText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("CodexHint"));
	HintText->SetText(NSLOCTEXT("LongNoon", "CodexHint", "[Up/Down] select     [Esc] close"));
	LongNoonUI::StyleText(HintText, LongNoonUI::BodyFont(18.0f), LongNoonUI::Cream());
	if (UCanvasPanelSlot* CS = Root->AddChildToCanvas(HintText))
	{
		CS->SetAnchors(FAnchors(0.5f, 0.92f));
		CS->SetAlignment(FVector2D(0.5f, 0.5f));
		CS->SetAutoSize(true);
	}
}

void ULongNoonCodexWidget::Gather()
{
	Found.Reset();
	if (Codex.IsValid())
	{
		for (const FName& Id : Codex->GetFoundFragments())
		{
			Found.Add(Id);
		}
	}
	Found.Sort([](const FName& A, const FName& B) { return A.LexicalLess(B); });
	Selected = 0;
}

void ULongNoonCodexWidget::Refresh()
{
	if (TitleText)
	{
		TitleText->SetText(FText::FromString(FString::Printf(
			TEXT("The Codex    (literacy %d / 4,  %d found)"), Literacy, Found.Num())));
	}
	if (!ListBox)
	{
		return;
	}
	ListBox->ClearChildren();

	if (Found.Num() == 0)
	{
		UTextBlock* Empty = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
		Empty->SetText(NSLOCTEXT("LongNoon", "CodexEmpty", "No fragments found yet. Read the tablets you meet."));
		LongNoonUI::StyleText(Empty, LongNoonUI::BodyFont(20.0f), LongNoonUI::Cream());
		ListBox->AddChildToVerticalBox(Empty);
		if (BodyText) { BodyText->SetText(FText::GetEmpty()); }
		return;
	}

	for (int32 i = 0; i < Found.Num(); ++i)
	{
		const FString Caret = (i == Selected) ? TEXT("> ") : TEXT("   ");
		UTextBlock* RowText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
		RowText->SetText(FText::FromString(Caret + Found[i].ToString()));
		LongNoonUI::StyleText(RowText, LongNoonUI::BodyFont(20.0f),
			(i == Selected) ? LongNoonUI::Gold() : LongNoonUI::Cream());
		ListBox->AddChildToVerticalBox(RowText);
	}

	// Body: glyphs until literate enough, then the translation.
	if (BodyText && Found.IsValidIndex(Selected) && Registry.IsValid())
	{
		const FLoreFragmentRow* Row = Registry->GetFragment(Found[Selected]);
		if (Row)
		{
			const bool bReadable = ULongNoonCodexSubsystem::IsLiteracyEnough(Row->RequiredLiteracyTier, Literacy);
			const FText Header = bReadable
				? NSLOCTEXT("LongNoon", "CodexReadable", "(translated)\n\n")
				: NSLOCTEXT("LongNoon", "CodexGlyph", "(the glyphs resist you - find more, learn more)\n\n");
			const FText Lore = bReadable ? Row->TranslatedText : Row->GlyphText;
			BodyText->SetText(FText::Format(NSLOCTEXT("LongNoon", "CodexBodyFmt", "{0}{1}"), Header, Lore));
		}
		else
		{
			BodyText->SetText(FText::GetEmpty());
		}
	}
}

FReply ULongNoonCodexWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const FKey Key = InKeyEvent.GetKey();
	if (Key == EKeys::Up || Key == EKeys::Gamepad_DPad_Up)
	{
		if (Found.Num() > 0) { Selected = (Selected - 1 + Found.Num()) % Found.Num(); Refresh(); }
		return FReply::Handled();
	}
	if (Key == EKeys::Down || Key == EKeys::Gamepad_DPad_Down)
	{
		if (Found.Num() > 0) { Selected = (Selected + 1) % Found.Num(); Refresh(); }
		return FReply::Handled();
	}
	if (Key == EKeys::Escape || Key == EKeys::Gamepad_Special_Right)
	{
		Close();
		return FReply::Handled();
	}
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void ULongNoonCodexWidget::Close()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		PC->FlushPressedKeys();
	}
	RemoveFromParent();
}
