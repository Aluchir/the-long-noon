#include "UI/LongNoonMainMenuWidget.h"
#include "UI/LongNoonUIStyle.h"
#include "Systems/LongNoonSaveService.h"
#include "Engine/GameInstance.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void ULongNoonMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	// Without this the widget can't hold keyboard focus, so NativeOnKeyDown never fires
	// and Enter/C/Q do nothing. Make it focusable and grab focus immediately.
	SetIsFocusable(true);
	SetKeyboardFocus();
}

TSharedRef<SWidget> ULongNoonMainMenuWidget::RebuildWidget()
{
	if (!TitleText)
	{
		BuildTree();
	}
	return Super::RebuildWidget();
}

void ULongNoonMainMenuWidget::BuildTree()
{
	if (!WidgetTree)
	{
		return;
	}

	UCanvasPanel* Root = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("MenuRoot"));
	WidgetTree->RootWidget = Root;

	UBorder* Bg = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("MenuBg"));
	Bg->SetBrushColor(LongNoonUI::Ink());
	if (UCanvasPanelSlot* CS = Root->AddChildToCanvas(Bg))
	{
		CS->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
		CS->SetOffsets(FMargin(0.0f));
	}

	TitleText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("Title"));
	TitleText->SetText(NSLOCTEXT("LongNoon", "GameTitle", "THE LONG NOON"));
	LongNoonUI::StyleText(TitleText, LongNoonUI::TitleFont(64.0f), LongNoonUI::Gold());
	if (UCanvasPanelSlot* CS = Root->AddChildToCanvas(TitleText))
	{
		CS->SetAnchors(FAnchors(0.5f, 0.38f));
		CS->SetAlignment(FVector2D(0.5f, 0.5f));
		CS->SetAutoSize(true);
	}

	SubtitleText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("Subtitle"));
	SubtitleText->SetText(NSLOCTEXT("LongNoon", "GameSubtitle", "a cozy garden at the end of dying"));
	LongNoonUI::StyleText(SubtitleText, LongNoonUI::HeadingFont(24.0f), LongNoonUI::Cream());
	if (UCanvasPanelSlot* CS = Root->AddChildToCanvas(SubtitleText))
	{
		CS->SetAnchors(FAnchors(0.5f, 0.46f));
		CS->SetAlignment(FVector2D(0.5f, 0.5f));
		CS->SetAutoSize(true);
	}

	PromptText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("Prompt"));
	PromptText->SetText(NSLOCTEXT("LongNoon", "MenuPrompt", "[Enter] begin     [C] continue     [Q] quit"));
	LongNoonUI::StyleText(PromptText, LongNoonUI::BodyFont(22.0f), LongNoonUI::Cream());
	if (UCanvasPanelSlot* CS = Root->AddChildToCanvas(PromptText))
	{
		CS->SetAnchors(FAnchors(0.5f, 0.6f));
		CS->SetAlignment(FVector2D(0.5f, 0.5f));
		CS->SetAutoSize(true);
	}
}

FReply ULongNoonMainMenuWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const FKey Key = InKeyEvent.GetKey();
	UE_LOG(LogTemp, Log, TEXT("[Menu] key received: %s"), *Key.ToString());
	if (Key == EKeys::Enter || Key == EKeys::SpaceBar || Key == EKeys::Gamepad_FaceButton_Bottom)
	{
		UGameplayStatics::OpenLevel(this, StartLevel);
		return FReply::Handled();
	}
	if (Key == EKeys::C)
	{
		// Continue: restore slot 1 into the GameInstance (progression persists across the
		// OpenLevel), then enter the world.
		if (UGameInstance* GI = GetGameInstance())
		{
			if (ULongNoonSaveService* Save = GI->GetSubsystem<ULongNoonSaveService>())
			{
				if (Save->DoesSlotExist(TEXT("Slot1")))
				{
					Save->LoadFromSlot(TEXT("Slot1"));
				}
			}
		}
		UGameplayStatics::OpenLevel(this, StartLevel);
		return FReply::Handled();
	}
	if (Key == EKeys::Q || Key == EKeys::Escape)
	{
		UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
		return FReply::Handled();
	}
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}
