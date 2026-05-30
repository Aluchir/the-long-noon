#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LongNoonSettingsWidget.generated.h"

class UTextBlock;

/**
 * Keyboard-driven settings overlay, built in C++ (no WBP, no UButton -> avoids the
 * HittestGrid C4459 issue). Shows a dimmed panel with a small list of rows that the
 * player steers entirely with the keyboard: Up/Down move the selection cursor between
 * [Master Volume, Mouse Sensitivity, Back]; Left/Right adjust the selected value;
 * Esc (or Enter on Back) closes the overlay and hands focus back to the game.
 *
 * Master volume is applied live via the au.MasterVolume console command. Mouse
 * sensitivity is stored on the widget (and applied best-effort to the player); read it
 * back through GetMouseSensitivity(). Values are plain members with sensible defaults;
 * config persistence can be wired later. See docs/art/style-guide.md.
 */
UCLASS()
class THELONGNOON_API ULongNoonSettingsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Current mouse sensitivity multiplier (0.2 - 3.0). */
	UFUNCTION(BlueprintCallable, Category = "Settings")
	float GetMouseSensitivity() const { return MouseSensitivity; }

	/** Remove the overlay and restore game focus/input. */
	UFUNCTION(BlueprintCallable, Category = "Settings")
	void Close();

protected:
	virtual void NativeConstruct() override;
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	void BuildTree();

	/** Re-render every row's text to reflect the current selection and values. */
	void RefreshRows();

	/** Move the selection cursor by Delta rows, wrapping within the list. */
	void MoveSelection(int32 Delta);

	/** Nudge the value on the currently selected row by Direction (-1 / +1). */
	void AdjustSelected(int32 Direction);

	/** Push the current master volume to the audio engine via console command. */
	void ApplyMasterVolume();

	/** Best-effort apply of the current mouse sensitivity to the owning player. */
	void ApplyMouseSensitivity();

	/** Rows in selection order. Keep in sync with the SettingsRow enum below. */
	enum class ESettingsRow : uint8
	{
		MasterVolume = 0,
		MouseSensitivity,
		Back,
		Count
	};

	int32 SelectedRow = 0;

	/** Master volume in percent (0 - 100). */
	float MasterVolume = 70.0f;

	/** Mouse sensitivity multiplier (0.2 - 3.0). */
	float MouseSensitivity = 1.0f;

	UPROPERTY(Transient) TObjectPtr<UTextBlock> TitleText;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> VolumeRow;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> SensitivityRow;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> BackRow;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> HintText;
};
