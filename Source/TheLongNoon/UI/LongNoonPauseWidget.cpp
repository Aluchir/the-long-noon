#include "UI/LongNoonPauseWidget.h"
#include "UI/LongNoonUIStyle.h"
#include "UI/LongNoonSettingsWidget.h"
#include "UI/LongNoonSaveMenuWidget.h"
#include "UI/LongNoonCodexWidget.h"
#include "UI/LongNoonInventoryWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/PlayerController.h"

namespace
{
	template <typename T>
	void OpenSubMenu(APlayerController* PC, int32 ZOrder)
	{
		if (!PC) { return; }
		if (T* W = CreateWidget<T>(PC, T::StaticClass()))
		{
			W->AddToViewport(ZOrder);
			FInputModeUIOnly Mode;
			Mode.SetWidgetToFocus(W->TakeWidget());
			Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PC->SetInputMode(Mode);
		}
	}
}

void ULongNoonPauseWidget::NativeConstruct()
{
	Super::NativeConstruct();
	// Must be focusable + hold keyboard focus or NativeOnKeyDown never fires (Esc/S/L/Q).
	SetIsFocusable(true);
	SetKeyboardFocus();
}

TSharedRef<SWidget> ULongNoonPauseWidget::RebuildWidget()
{
	if (!TitleText)
	{
		BuildTree();
	}
	return Super::RebuildWidget();
}

void ULongNoonPauseWidget::BuildTree()
{
	if (!WidgetTree)
	{
		return;
	}

	UCanvasPanel* Root = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("PauseRoot"));
	WidgetTree->RootWidget = Root;

	// Full-screen dim.
	UBorder* Dim = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("Dim"));
	Dim->SetBrushColor(LongNoonUI::PanelTint());
	if (UCanvasPanelSlot* CS = Root->AddChildToCanvas(Dim))
	{
		CS->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
		CS->SetOffsets(FMargin(0.0f));
	}

	TitleText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("PauseTitle"));
	TitleText->SetText(NSLOCTEXT("LongNoon", "Paused", "The Long Noon holds its breath."));
	LongNoonUI::StyleText(TitleText, LongNoonUI::HeadingFont(34.0f), LongNoonUI::Gold());
	if (UCanvasPanelSlot* CS = Root->AddChildToCanvas(TitleText))
	{
		CS->SetAnchors(FAnchors(0.5f, 0.42f));
		CS->SetAlignment(FVector2D(0.5f, 0.5f));
		CS->SetAutoSize(true);
	}

	HintText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("PauseHint"));
	HintText->SetText(NSLOCTEXT("LongNoon", "PauseHint", "[Esc] resume     [I] items     [J] codex     [S] settings     [L] save / load     [Q] quit"));
	LongNoonUI::StyleText(HintText, LongNoonUI::BodyFont(20.0f), LongNoonUI::Cream());
	if (UCanvasPanelSlot* CS = Root->AddChildToCanvas(HintText))
	{
		CS->SetAnchors(FAnchors(0.5f, 0.52f));
		CS->SetAlignment(FVector2D(0.5f, 0.5f));
		CS->SetAutoSize(true);
	}
}

FReply ULongNoonPauseWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const FKey Key = InKeyEvent.GetKey();
	if (Key == EKeys::Escape || Key == EKeys::Enter || Key == EKeys::Gamepad_Special_Right)
	{
		Resume();
		return FReply::Handled();
	}
	if (Key == EKeys::Q)
	{
		UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
		return FReply::Handled();
	}
	if (Key == EKeys::S)
	{
		OpenSubMenu<ULongNoonSettingsWidget>(GetOwningPlayer(), 110);
		return FReply::Handled();
	}
	if (Key == EKeys::L)
	{
		OpenSubMenu<ULongNoonSaveMenuWidget>(GetOwningPlayer(), 110);
		return FReply::Handled();
	}
	if (Key == EKeys::J)
	{
		OpenSubMenu<ULongNoonCodexWidget>(GetOwningPlayer(), 110);
		return FReply::Handled();
	}
	if (Key == EKeys::I)
	{
		OpenSubMenu<ULongNoonInventoryWidget>(GetOwningPlayer(), 110);
		return FReply::Handled();
	}
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void ULongNoonPauseWidget::Resume()
{
	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		UGameplayStatics::SetGamePaused(this, false);
		PC->SetInputMode(FInputModeGameOnly());
		PC->bShowMouseCursor = false;
	}
	RemoveFromParent();
}
