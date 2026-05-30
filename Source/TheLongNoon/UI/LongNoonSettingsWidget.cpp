#include "UI/LongNoonSettingsWidget.h"
#include "UI/LongNoonUIStyle.h"
#include "Core/LongNoonUserSettings.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerInput.h"

void ULongNoonSettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Make sure the overlay can receive keyboard focus (so the arrow keys land here,
	// not in the game). The owning code is expected to also SetWidgetToFocus on it.
	bIsFocusable = true;

	// Seed from the persisted settings (volume stored normalized 0..1; UI uses 0..100).
	if (ULongNoonUserSettings* S = ULongNoonUserSettings::Load())
	{
		MasterVolume = S->MasterVolume * 100.0f;
		MouseSensitivity = S->MouseSensitivity;
	}
	ApplyMasterVolume();
	ApplyMouseSensitivity();
	RefreshRows();
}

TSharedRef<SWidget> ULongNoonSettingsWidget::RebuildWidget()
{
	if (!TitleText)
	{
		BuildTree();
	}
	return Super::RebuildWidget();
}

void ULongNoonSettingsWidget::BuildTree()
{
	if (!WidgetTree)
	{
		return;
	}

	UCanvasPanel* Root = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("SettingsRoot"));
	WidgetTree->RootWidget = Root;

	// Full-screen dim, matching the pause menu.
	UBorder* Dim = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("Dim"));
	Dim->SetBrushColor(LongNoonUI::PanelTint());
	if (UCanvasPanelSlot* CS = Root->AddChildToCanvas(Dim))
	{
		CS->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
		CS->SetOffsets(FMargin(0.0f));
	}

	TitleText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("SettingsTitle"));
	TitleText->SetText(NSLOCTEXT("LongNoon", "SettingsTitle", "Settings"));
	LongNoonUI::StyleText(TitleText, LongNoonUI::HeadingFont(36.0f), LongNoonUI::Gold());
	if (UCanvasPanelSlot* CS = Root->AddChildToCanvas(TitleText))
	{
		CS->SetAnchors(FAnchors(0.5f, 0.34f));
		CS->SetAlignment(FVector2D(0.5f, 0.5f));
		CS->SetAutoSize(true);
	}

	auto AddRow = [&](const TCHAR* Name, float Y) -> UTextBlock*
	{
		UTextBlock* Row = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), Name);
		Row->SetText(FText::GetEmpty());
		LongNoonUI::StyleText(Row, LongNoonUI::BodyFont(24.0f), LongNoonUI::Cream());
		if (UCanvasPanelSlot* CS = Root->AddChildToCanvas(Row))
		{
			CS->SetAnchors(FAnchors(0.5f, Y));
			CS->SetAlignment(FVector2D(0.5f, 0.5f));
			CS->SetAutoSize(true);
		}
		return Row;
	};
	VolumeRow      = AddRow(TEXT("VolumeRow"),      0.46f);
	SensitivityRow = AddRow(TEXT("SensitivityRow"), 0.52f);
	BackRow        = AddRow(TEXT("BackRow"),        0.58f);

	HintText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("SettingsHint"));
	HintText->SetText(NSLOCTEXT("LongNoon", "SettingsHint", "[Up/Down] select     [Left/Right] adjust     [Esc] back"));
	LongNoonUI::StyleText(HintText, LongNoonUI::BodyFont(18.0f), LongNoonUI::Cream());
	if (UCanvasPanelSlot* CS = Root->AddChildToCanvas(HintText))
	{
		CS->SetAnchors(FAnchors(0.5f, 0.68f));
		CS->SetAlignment(FVector2D(0.5f, 0.5f));
		CS->SetAutoSize(true);
	}

	RefreshRows();
}

void ULongNoonSettingsWidget::RefreshRows()
{
	// A leading caret marks the selected row; trailing spaces keep things aligned.
	auto Marker = [this](ESettingsRow Row) -> FString
	{
		return (SelectedRow == static_cast<int32>(Row)) ? TEXT("> ") : TEXT("   ");
	};

	if (VolumeRow)
	{
		VolumeRow->SetText(FText::FromString(FString::Printf(
			TEXT("%sMaster Volume:  %d%%"), *Marker(ESettingsRow::MasterVolume), FMath::RoundToInt(MasterVolume))));
	}
	if (SensitivityRow)
	{
		SensitivityRow->SetText(FText::FromString(FString::Printf(
			TEXT("%sMouse Sensitivity:  %.2f"), *Marker(ESettingsRow::MouseSensitivity), MouseSensitivity)));
	}
	if (BackRow)
	{
		BackRow->SetText(FText::FromString(FString::Printf(
			TEXT("%sBack"), *Marker(ESettingsRow::Back))));
	}
}

void ULongNoonSettingsWidget::MoveSelection(int32 Delta)
{
	const int32 Num = static_cast<int32>(ESettingsRow::Count);
	SelectedRow = ((SelectedRow + Delta) % Num + Num) % Num; // wrap, handling negatives
	RefreshRows();
}

void ULongNoonSettingsWidget::AdjustSelected(int32 Direction)
{
	switch (static_cast<ESettingsRow>(SelectedRow))
	{
	case ESettingsRow::MasterVolume:
		MasterVolume = FMath::Clamp(MasterVolume + Direction * 5.0f, 0.0f, 100.0f);
		ApplyMasterVolume();
		break;

	case ESettingsRow::MouseSensitivity:
		MouseSensitivity = FMath::Clamp(MouseSensitivity + Direction * 0.1f, 0.2f, 3.0f);
		ApplyMouseSensitivity();
		break;

	case ESettingsRow::Back:
	default:
		// Nothing to adjust on the Back row.
		break;
	}

	RefreshRows();
}

void ULongNoonSettingsWidget::ApplyMasterVolume()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		PC->ConsoleCommand(FString::Printf(TEXT("au.MasterVolume %.2f"), MasterVolume / 100.0f));
	}
}

void ULongNoonSettingsWidget::ApplyMouseSensitivity()
{
	// Best-effort: scale the player input's look sensitivity. This is intentionally
	// defensive (null-checked) so the overlay still works if the input setup differs.
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (UPlayerInput* Input = PC->PlayerInput)
		{
			Input->SetMouseSensitivity(MouseSensitivity);
		}
	}
}

FReply ULongNoonSettingsWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const FKey Key = InKeyEvent.GetKey();

	if (Key == EKeys::Up || Key == EKeys::Gamepad_DPad_Up)
	{
		MoveSelection(-1);
		return FReply::Handled();
	}
	if (Key == EKeys::Down || Key == EKeys::Gamepad_DPad_Down)
	{
		MoveSelection(1);
		return FReply::Handled();
	}
	if (Key == EKeys::Left || Key == EKeys::Gamepad_DPad_Left)
	{
		AdjustSelected(-1);
		return FReply::Handled();
	}
	if (Key == EKeys::Right || Key == EKeys::Gamepad_DPad_Right)
	{
		AdjustSelected(1);
		return FReply::Handled();
	}

	// Esc always closes. Enter closes only when the Back row is selected.
	if (Key == EKeys::Escape || Key == EKeys::Gamepad_Special_Right)
	{
		Close();
		return FReply::Handled();
	}
	if (Key == EKeys::Enter && static_cast<ESettingsRow>(SelectedRow) == ESettingsRow::Back)
	{
		Close();
		return FReply::Handled();
	}

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void ULongNoonSettingsWidget::Close()
{
	// Persist so the choices survive a relaunch (convert UI percent back to 0..1).
	if (ULongNoonUserSettings* S = ULongNoonUserSettings::Load())
	{
		S->MasterVolume = MasterVolume / 100.0f;
		S->MouseSensitivity = MouseSensitivity;
		ULongNoonUserSettings::Save(S);
	}

	RemoveFromParent();

	// Hand keyboard focus back to whatever invoked us (e.g. the pause menu). The opener
	// is responsible for any input-mode/cursor restoration once the game resumes.
	if (APlayerController* PC = GetOwningPlayer())
	{
		PC->FlushPressedKeys();
	}
}
