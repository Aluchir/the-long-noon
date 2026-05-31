#include "UI/LongNoonInventoryWidget.h"
#include "UI/LongNoonUIStyle.h"
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
#include "GameFramework/Pawn.h"
#include "Engine/GameInstance.h"

void ULongNoonInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetIsFocusable(true);
	SetKeyboardFocus();
	Refresh();
}

TSharedRef<SWidget> ULongNoonInventoryWidget::RebuildWidget()
{
	if (!ListBox)
	{
		BuildTree();
	}
	return Super::RebuildWidget();
}

void ULongNoonInventoryWidget::BuildTree()
{
	if (!WidgetTree)
	{
		return;
	}

	UCanvasPanel* Root = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("InvRoot"));
	WidgetTree->RootWidget = Root;

	UBorder* Dim = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("Dim"));
	Dim->SetBrushColor(LongNoonUI::PanelTint());
	if (UCanvasPanelSlot* CS = Root->AddChildToCanvas(Dim))
	{
		CS->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
		CS->SetOffsets(FMargin(0.0f));
	}

	TitleText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("InvTitle"));
	TitleText->SetText(NSLOCTEXT("LongNoon", "InvTitle", "What you carry"));
	LongNoonUI::StyleText(TitleText, LongNoonUI::HeadingFont(34.0f), LongNoonUI::Gold());
	if (UCanvasPanelSlot* CS = Root->AddChildToCanvas(TitleText))
	{
		CS->SetAnchors(FAnchors(0.5f, 0.2f));
		CS->SetAlignment(FVector2D(0.5f, 0.5f));
		CS->SetAutoSize(true);
	}

	ListBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("InvList"));
	if (UCanvasPanelSlot* CS = Root->AddChildToCanvas(ListBox))
	{
		CS->SetAnchors(FAnchors(0.5f, 0.3f));
		CS->SetAlignment(FVector2D(0.5f, 0.0f));
		CS->SetAutoSize(true);
	}

	HintText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("InvHint"));
	HintText->SetText(NSLOCTEXT("LongNoon", "InvHint", "[Esc] close"));
	LongNoonUI::StyleText(HintText, LongNoonUI::BodyFont(18.0f), LongNoonUI::Cream());
	if (UCanvasPanelSlot* CS = Root->AddChildToCanvas(HintText))
	{
		CS->SetAnchors(FAnchors(0.5f, 0.86f));
		CS->SetAlignment(FVector2D(0.5f, 0.5f));
		CS->SetAutoSize(true);
	}
}

void ULongNoonInventoryWidget::Refresh()
{
	if (!ListBox)
	{
		return;
	}
	ListBox->ClearChildren();

	ULongNoonInventoryComponent* Inv = nullptr;
	if (const APlayerController* PC = GetOwningPlayer())
	{
		if (const APawn* Pawn = PC->GetPawn())
		{
			Inv = Pawn->FindComponentByClass<ULongNoonInventoryComponent>();
		}
	}

	ULongNoonDataRegistry* Reg = nullptr;
	if (UGameInstance* GI = GetGameInstance())
	{
		Reg = GI->GetSubsystem<ULongNoonDataRegistry>();
	}

	if (!Inv || Inv->GetStacks().Num() == 0)
	{
		UTextBlock* Empty = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
		Empty->SetText(NSLOCTEXT("LongNoon", "InvEmpty", "Your hands are empty. Gather something."));
		LongNoonUI::StyleText(Empty, LongNoonUI::BodyFont(22.0f), LongNoonUI::Cream());
		ListBox->AddChildToVerticalBox(Empty);
		return;
	}

	for (const TPair<FName, int32>& Stack : Inv->GetStacks())
	{
		FString Name = Stack.Key.ToString();
		if (Reg)
		{
			if (const FItemRow* Row = Reg->GetItem(Stack.Key))
			{
				if (!Row->DisplayName.IsEmpty())
				{
					Name = Row->DisplayName.ToString();
				}
			}
		}
		UTextBlock* Line = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
		Line->SetText(FText::FromString(FString::Printf(TEXT("%s    x%d"), *Name, Stack.Value)));
		LongNoonUI::StyleText(Line, LongNoonUI::BodyFont(22.0f), LongNoonUI::Cream());
		ListBox->AddChildToVerticalBox(Line);
	}
}

FReply ULongNoonInventoryWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const FKey Key = InKeyEvent.GetKey();
	if (Key == EKeys::Escape || Key == EKeys::I || Key == EKeys::Gamepad_Special_Right)
	{
		Close();
		return FReply::Handled();
	}
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void ULongNoonInventoryWidget::Close()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		PC->FlushPressedKeys();
	}
	RemoveFromParent();
}
