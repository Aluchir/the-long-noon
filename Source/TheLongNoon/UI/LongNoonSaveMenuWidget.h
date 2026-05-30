#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LongNoonSaveMenuWidget.generated.h"

class UTextBlock;

/**
 * Keyboard-driven Save/Load overlay, built in C++ (no WBP, no UButton -> avoids the
 * HittestGrid C4459 issue, matching ULongNoonPauseWidget). Lists ~3 save slots plus a
 * Back row, each a UTextBlock inside a dimmed UBorder. Handles its own keys via
 * NativeOnKeyDown: Up/Down move the selection, [S] saves the selected slot, [L] loads
 * it, and Esc/Enter on the Back row closes (RemoveFromParent). Talks to
 * ULongNoonSaveService (a UGameInstanceSubsystem) for the actual save/load.
 * See docs/art/style-guide.md.
 */
UCLASS()
class THELONGNOON_API ULongNoonSaveMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Remove the overlay; the caller restores its own input/focus. */
	UFUNCTION(BlueprintCallable, Category = "Save")
	void Close();

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeConstruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	void BuildTree();

	/** Refresh slot rows + highlight, reading DoesSlotExist for each slot. */
	void RefreshRows();

	/** Slot index -> "SlotN" name expected by the save service. */
	static FString SlotNameForIndex(int32 Index);

private:
	/** Number of save slots shown (rows 0..NumSlots-1); the last row is Back. */
	static constexpr int32 NumSlots = 3;

	/** Currently highlighted row: 0..NumSlots-1 are slots, NumSlots is Back. */
	int32 SelectedRow = 0;

	UPROPERTY(Transient) TObjectPtr<UTextBlock> TitleText;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> HintText;
	UPROPERTY(Transient) TArray<TObjectPtr<UTextBlock>> SlotRows;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> BackRow;
};
