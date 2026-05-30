#include "UI/LongNoonSaveMenuWidget.h"
#include "Systems/LongNoonSaveService.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Engine/GameInstance.h"

namespace
{
	const FLinearColor RowSelectedColor(1.0f, 0.92f, 0.70f, 1.0f);
	const FLinearColor RowNormalColor(0.70f, 0.70f, 0.74f, 1.0f);
}

FString ULongNoonSaveMenuWidget::SlotNameForIndex(int32 Index)
{
	return FString::Printf(TEXT("Slot%d"), Index + 1);
}

TSharedRef<SWidget> ULongNoonSaveMenuWidget::RebuildWidget()
{
	if (!TitleText)
	{
		BuildTree();
	}
	return Super::RebuildWidget();
}

void ULongNoonSaveMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	// Take keyboard focus so NativeOnKeyDown receives input.
	SetIsFocusable(true);
	RefreshRows();
	SetKeyboardFocus();
}

void ULongNoonSaveMenuWidget::BuildTree()
{
	if (!WidgetTree)
	{
		return;
	}

	UCanvasPanel* Root = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("SaveMenuRoot"));
	WidgetTree->RootWidget = Root;

	// Full-screen dim, matching the pause widget.
	UBorder* Dim = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("Dim"));
	Dim->SetBrushColor(FLinearColor(0.02f, 0.02f, 0.03f, 0.6f));
	if (UCanvasPanelSlot* CS = Root->AddChildToCanvas(Dim))
	{
		CS->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
		CS->SetOffsets(FMargin(0.0f));
	}

	// Centered panel holding the rows.
	UBorder* Panel = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("SaveMenuPanel"));
	Panel->SetBrushColor(FLinearColor(0.05f, 0.05f, 0.07f, 0.92f));
	Panel->SetPadding(FMargin(32.0f, 24.0f));
	if (UCanvasPanelSlot* CS = Root->AddChildToCanvas(Panel))
	{
		CS->SetAnchors(FAnchors(0.5f, 0.5f));
		CS->SetAlignment(FVector2D(0.5f, 0.5f));
		CS->SetAutoSize(true);
	}

	UVerticalBox* Column = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("SaveMenuColumn"));
	Panel->SetContent(Column);

	TitleText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("SaveMenuTitle"));
	TitleText->SetText(NSLOCTEXT("LongNoon", "SaveMenuTitle", "What is remembered, persists."));
	if (UVerticalBoxSlot* VS = Column->AddChildToVerticalBox(TitleText))
	{
		VS->SetHorizontalAlignment(HAlign_Center);
		VS->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 16.0f));
	}

	SlotRows.Reset();
	for (int32 i = 0; i < NumSlots; ++i)
	{
		UTextBlock* Row = WidgetTree->ConstructWidget<UTextBlock>(
			UTextBlock::StaticClass(), *FString::Printf(TEXT("SaveSlotRow%d"), i));
		Row->SetColorAndOpacity(RowNormalColor);
		if (UVerticalBoxSlot* VS = Column->AddChildToVerticalBox(Row))
		{
			VS->SetHorizontalAlignment(HAlign_Center);
			VS->SetPadding(FMargin(0.0f, 4.0f));
		}
		SlotRows.Add(Row);
	}

	BackRow = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("SaveMenuBack"));
	BackRow->SetText(NSLOCTEXT("LongNoon", "SaveMenuBack", "Back"));
	BackRow->SetColorAndOpacity(RowNormalColor);
	if (UVerticalBoxSlot* VS = Column->AddChildToVerticalBox(BackRow))
	{
		VS->SetHorizontalAlignment(HAlign_Center);
		VS->SetPadding(FMargin(0.0f, 12.0f, 0.0f, 0.0f));
	}

	HintText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("SaveMenuHint"));
	HintText->SetText(NSLOCTEXT("LongNoon", "SaveMenuHint",
		"[Up/Down] select   [S] save   [L] load   [Esc] back"));
	HintText->SetColorAndOpacity(FLinearColor(0.45f, 0.45f, 0.50f, 1.0f));
	if (UVerticalBoxSlot* VS = Column->AddChildToVerticalBox(HintText))
	{
		VS->SetHorizontalAlignment(HAlign_Center);
		VS->SetPadding(FMargin(0.0f, 20.0f, 0.0f, 0.0f));
	}
}

void ULongNoonSaveMenuWidget::RefreshRows()
{
	const ULongNoonSaveService* Save = nullptr;
	if (const UGameInstance* GI = GetGameInstance())
	{
		Save = GI->GetSubsystem<ULongNoonSaveService>();
	}

	for (int32 i = 0; i < SlotRows.Num(); ++i)
	{
		UTextBlock* Row = SlotRows[i];
		if (!Row)
		{
			continue;
		}

		const bool bExists = Save ? Save->DoesSlotExist(SlotNameForIndex(i)) : false;
		const FText Status = bExists
			? NSLOCTEXT("LongNoon", "SlotSaved", "saved")
			: NSLOCTEXT("LongNoon", "SlotEmpty", "<empty>");
		Row->SetText(FText::Format(
			NSLOCTEXT("LongNoon", "SlotRowFmt", "Slot {0}: {1}"),
			FText::AsNumber(i + 1), Status));

		Row->SetColorAndOpacity(SelectedRow == i ? RowSelectedColor : RowNormalColor);
	}

	if (BackRow)
	{
		BackRow->SetColorAndOpacity(SelectedRow == NumSlots ? RowSelectedColor : RowNormalColor);
	}
}

FReply ULongNoonSaveMenuWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const FKey Key = InKeyEvent.GetKey();

	if (Key == EKeys::Up || Key == EKeys::Gamepad_DPad_Up || Key == EKeys::W)
	{
		SelectedRow = (SelectedRow + NumSlots) % (NumSlots + 1);
		RefreshRows();
		return FReply::Handled();
	}
	if (Key == EKeys::Down || Key == EKeys::Gamepad_DPad_Down)
	{
		SelectedRow = (SelectedRow + 1) % (NumSlots + 1);
		RefreshRows();
		return FReply::Handled();
	}

	// Esc, or Enter while Back is highlighted, closes the overlay.
	if (Key == EKeys::Escape || Key == EKeys::Gamepad_Special_Right ||
		(Key == EKeys::Enter && SelectedRow == NumSlots))
	{
		Close();
		return FReply::Handled();
	}

	// S = save selected slot; L = load selected slot. Only meaningful on a slot row.
	if ((Key == EKeys::S || Key == EKeys::L) && SelectedRow >= 0 && SelectedRow < NumSlots)
	{
		if (const UGameInstance* GI = GetGameInstance())
		{
			if (ULongNoonSaveService* Save = GI->GetSubsystem<ULongNoonSaveService>())
			{
				const FString SlotName = SlotNameForIndex(SelectedRow);
				if (Key == EKeys::S)
				{
					Save->SaveToSlot(SlotName);
				}
				else
				{
					Save->LoadFromSlot(SlotName);
				}
			}
		}
		RefreshRows();
		return FReply::Handled();
	}

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void ULongNoonSaveMenuWidget::Close()
{
	RemoveFromParent();
}
