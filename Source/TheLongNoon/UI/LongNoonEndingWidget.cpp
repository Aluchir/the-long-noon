#include "UI/LongNoonEndingWidget.h"
#include "UI/LongNoonUIStyle.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

void ULongNoonEndingWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetIsFocusable(true);
	SetKeyboardFocus();
}

TSharedRef<SWidget> ULongNoonEndingWidget::RebuildWidget()
{
	if (!TitleText)
	{
		BuildTree();
	}
	return Super::RebuildWidget();
}

void ULongNoonEndingWidget::BuildTree()
{
	if (!WidgetTree)
	{
		return;
	}

	UCanvasPanel* Root = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("EndRoot"));
	WidgetTree->RootWidget = Root;

	UBorder* Bg = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("EndBg"));
	Bg->SetBrushColor(LongNoonUI::Ink());
	if (UCanvasPanelSlot* CS = Root->AddChildToCanvas(Bg))
	{
		CS->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
		CS->SetOffsets(FMargin(0.0f));
	}

	TitleText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("EndTitle"));
	LongNoonUI::StyleText(TitleText, LongNoonUI::TitleFont(52.0f), LongNoonUI::Gold());
	if (UCanvasPanelSlot* CS = Root->AddChildToCanvas(TitleText))
	{
		CS->SetAnchors(FAnchors(0.5f, 0.26f));
		CS->SetAlignment(FVector2D(0.5f, 0.5f));
		CS->SetAutoSize(true);
	}

	BodyText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("EndBody"));
	BodyText->SetAutoWrapText(true);
	BodyText->SetJustification(ETextJustify::Center);
	LongNoonUI::StyleText(BodyText, LongNoonUI::BodyFont(24.0f), LongNoonUI::Cream());
	if (UCanvasPanelSlot* CS = Root->AddChildToCanvas(BodyText))
	{
		CS->SetAnchors(FAnchors(0.22f, 0.4f, 0.78f, 0.8f));
		CS->SetOffsets(FMargin(0.0f));
	}

	PromptText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("EndPrompt"));
	PromptText->SetText(NSLOCTEXT("LongNoon", "EndPrompt", "[Enter] return to the title"));
	LongNoonUI::StyleText(PromptText, LongNoonUI::BodyFont(20.0f), LongNoonUI::Cream());
	if (UCanvasPanelSlot* CS = Root->AddChildToCanvas(PromptText))
	{
		CS->SetAnchors(FAnchors(0.5f, 0.9f));
		CS->SetAlignment(FVector2D(0.5f, 0.5f));
		CS->SetAutoSize(true);
	}
}

void ULongNoonEndingWidget::SetEnding(const FText& InTitle, const FText& InBody)
{
	if (!TitleText)
	{
		BuildTree();
	}
	if (TitleText) { TitleText->SetText(InTitle); }
	if (BodyText)  { BodyText->SetText(InBody); }
}

FReply ULongNoonEndingWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const FKey Key = InKeyEvent.GetKey();
	if (Key == EKeys::Enter || Key == EKeys::SpaceBar || Key == EKeys::Escape || Key == EKeys::Gamepad_FaceButton_Bottom)
	{
		if (APlayerController* PC = GetOwningPlayer())
		{
			PC->SetInputMode(FInputModeGameOnly());
			PC->bShowMouseCursor = false;
		}
		UGameplayStatics::OpenLevel(this, ReturnLevel);
		return FReply::Handled();
	}
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}
