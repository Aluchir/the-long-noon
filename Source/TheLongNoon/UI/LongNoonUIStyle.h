// LongNoonUIStyle.h
//
// Header-only art-direction helper for The Long Noon's C++ UMG widgets.
// No .cpp, no asset dependencies: everything is inline and leans on engine
// built-ins (Roboto via FCoreStyle) so it compiles against the modules we
// already link (Slate / SlateCore / UMG).
//
// Usage from a widget's BuildTree():
//   using namespace LongNoonUI;
//   StyleText(TitleText, TitleFont(), Cream());
//   StyleText(HintText,  BodyFont(),  Gold());
//
#pragma once

#include "CoreMinimal.h"
#include "Math/Color.h"
#include "Fonts/SlateFontInfo.h"
#include "Styling/CoreStyle.h"
#include "Components/TextBlock.h"

/**
 * Cohesive "cozy noon" palette + typography for the game's UMG layer.
 * All helpers are inline statics so the header can be included anywhere
 * without an accompanying translation unit.
 */
namespace LongNoonUI
{
	// ---- Palette ------------------------------------------------------------

	/** Soft off-white for body and heading text. */
	inline FLinearColor Cream()
	{
		return FLinearColor(0.96f, 0.93f, 0.85f, 1.0f);
	}

	/** Warm gold accent for titles, highlights and key prompts. */
	inline FLinearColor Gold()
	{
		return FLinearColor(0.95f, 0.78f, 0.35f, 1.0f);
	}

	/** Deep warm brown-black for opaque panels and backplates. */
	inline FLinearColor Ink()
	{
		return FLinearColor(0.06f, 0.05f, 0.04f, 1.0f);
	}

	/** Translucent dark wash for dim borders / scrims over the world. */
	inline FLinearColor PanelTint()
	{
		return FLinearColor(0.06f, 0.05f, 0.04f, 0.66f);
	}

	// ---- Typography ---------------------------------------------------------
	// Engine Roboto via FCoreStyle so there is no .uasset to cook or reference.

	/** Large bold display font for screen titles. */
	inline FSlateFontInfo TitleFont(float Size = 48.0f)
	{
		return FCoreStyle::GetDefaultFontStyle(TEXT("Bold"), Size);
	}

	/** Bold section/heading font. */
	inline FSlateFontInfo HeadingFont(float Size = 28.0f)
	{
		return FCoreStyle::GetDefaultFontStyle(TEXT("Bold"), Size);
	}

	/** Regular-weight body font for prompts, hints and dialogue. */
	inline FSlateFontInfo BodyFont(float Size = 20.0f)
	{
		return FCoreStyle::GetDefaultFontStyle(TEXT("Regular"), Size);
	}

	// ---- Helpers ------------------------------------------------------------

	/**
	 * Apply font, color and a subtle warm drop shadow to a text block.
	 * Null-safe so it can be called unconditionally after ConstructWidget.
	 */
	inline void StyleText(class UTextBlock* T, const FSlateFontInfo& Font, const FLinearColor& Color)
	{
		if (!T)
		{
			return;
		}

		T->SetFont(Font);
		T->SetColorAndOpacity(FSlateColor(Color));

		// A soft 1px shadow lifts cream/gold text off bright noon backdrops.
		T->SetShadowOffset(FVector2D(1.0f, 1.0f));
		T->SetShadowColorAndOpacity(FLinearColor(0.0f, 0.0f, 0.0f, 0.55f));
	}
}
