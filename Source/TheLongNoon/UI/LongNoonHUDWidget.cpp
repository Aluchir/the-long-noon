#include "UI/LongNoonHUDWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

TSharedRef<SWidget> ULongNoonHUDWidget::RebuildWidget()
{
	if (!StaminaBar)
	{
		BuildTree();
	}
	return Super::RebuildWidget();
}

void ULongNoonHUDWidget::BuildTree()
{
	if (!WidgetTree)
	{
		return;
	}

	UCanvasPanel* Root = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("Root"));
	WidgetTree->RootWidget = Root;

	auto AddBar = [&](const TCHAR* Name, float Y, FLinearColor Color) -> UProgressBar*
	{
		UProgressBar* Bar = WidgetTree->ConstructWidget<UProgressBar>(UProgressBar::StaticClass(), Name);
		Bar->SetFillColorAndOpacity(Color);
		Bar->SetPercent(1.0f);
		if (UCanvasPanelSlot* CS = Root->AddChildToCanvas(Bar))
		{
			CS->SetPosition(FVector2D(40.0f, Y));
			CS->SetSize(FVector2D(260.0f, 16.0f));
		}
		return Bar;
	};
	StaminaBar = AddBar(TEXT("StaminaBar"), 40.0f, FLinearColor(0.45f, 0.80f, 0.45f));   // green
	FocusBar   = AddBar(TEXT("FocusBar"),   62.0f, FLinearColor(0.55f, 0.70f, 0.95f));   // blue

	// Interact prompt, lower-centre.
	PromptText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("PromptText"));
	PromptText->SetText(FText::GetEmpty());
	PromptText->SetVisibility(ESlateVisibility::Collapsed);
	if (UCanvasPanelSlot* CS = Root->AddChildToCanvas(PromptText))
	{
		CS->SetAnchors(FAnchors(0.5f, 0.85f));
		CS->SetAlignment(FVector2D(0.5f, 0.5f));
		CS->SetAutoSize(true);
	}

	// Lore toast, upper-centre.
	ToastText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("ToastText"));
	ToastText->SetText(FText::GetEmpty());
	ToastText->SetVisibility(ESlateVisibility::Collapsed);
	if (UCanvasPanelSlot* CS = Root->AddChildToCanvas(ToastText))
	{
		CS->SetAnchors(FAnchors(0.5f, 0.08f));
		CS->SetAlignment(FVector2D(0.5f, 0.5f));
		CS->SetAutoSize(true);
	}

	// Persistent objective tracker, upper-left under the meters.
	ObjectiveText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("ObjectiveText"));
	ObjectiveText->SetText(FText::GetEmpty());
	ObjectiveText->SetVisibility(ESlateVisibility::Collapsed);
	if (UCanvasPanelSlot* CS = Root->AddChildToCanvas(ObjectiveText))
	{
		CS->SetPosition(FVector2D(40.0f, 92.0f));
		CS->SetAutoSize(true);
	}

	// NPC dialogue line, lower-centre (above the interact prompt).
	DialogueText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("DialogueText"));
	DialogueText->SetText(FText::GetEmpty());
	DialogueText->SetVisibility(ESlateVisibility::Collapsed);
	if (UCanvasPanelSlot* CS = Root->AddChildToCanvas(DialogueText))
	{
		CS->SetAnchors(FAnchors(0.5f, 0.78f));
		CS->SetAlignment(FVector2D(0.5f, 0.5f));
		CS->SetAutoSize(true);
	}
}

void ULongNoonHUDWidget::UpdateTend(float Stamina, float Focus, float Comfort)
{
	const float Inv = MaxMeter > 0.0f ? 1.0f / MaxMeter : 0.01f;
	if (StaminaBar) { StaminaBar->SetPercent(FMath::Clamp(Stamina * Inv, 0.0f, 1.0f)); }
	if (FocusBar)   { FocusBar->SetPercent(FMath::Clamp(Focus * Inv, 0.0f, 1.0f)); }
}

void ULongNoonHUDWidget::SetInteractPrompt(const FText& Prompt)
{
	if (!PromptText)
	{
		return;
	}
	PromptText->SetText(Prompt);
	PromptText->SetVisibility(Prompt.IsEmpty() ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
}

void ULongNoonHUDWidget::OnLoreFound(FName FragmentId)
{
	if (!ToastText)
	{
		return;
	}
	ToastText->SetText(FText::Format(NSLOCTEXT("LongNoon", "LoreToast", "A fragment surfaces: {0}"), FText::FromName(FragmentId)));
	ToastText->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void ULongNoonHUDWidget::ShowToast(const FText& Message)
{
	if (!ToastText)
	{
		return;
	}
	ToastText->SetText(Message);
	ToastText->SetVisibility(Message.IsEmpty() ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
}

void ULongNoonHUDWidget::SetObjective(const FText& Objective)
{
	if (!ObjectiveText)
	{
		return;
	}
	ObjectiveText->SetText(Objective);
	ObjectiveText->SetVisibility(Objective.IsEmpty() ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
}

void ULongNoonHUDWidget::SetDialogue(const FText& Line)
{
	if (!DialogueText)
	{
		return;
	}
	DialogueText->SetText(Line);
	DialogueText->SetVisibility(Line.IsEmpty() ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
}
