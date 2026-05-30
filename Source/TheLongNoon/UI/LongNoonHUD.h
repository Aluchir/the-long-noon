#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LongNoonHUD.generated.h"

class ULongNoonHUDWidget;

/**
 * Creates and owns the in-game HUD widget. Assign HUDWidgetClass to a WBP that
 * derives from ULongNoonHUDWidget. Gameplay systems push updates through the thin
 * forwarding methods below (the character wires its components to these on BeginPlay).
 * See the UI module and docs/art/style-guide.md.
 */
UCLASS()
class THELONGNOON_API ALongNoonHUD : public AHUD
{
	GENERATED_BODY()

public:
	ALongNoonHUD();

	/** Push the Tend meters to the widget (no-op until the widget exists). */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateTend(float Stamina, float Focus, float Comfort);

	/** Set (or clear, with empty text) the transient interact prompt. */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetInteractPrompt(const FText& Prompt);

	/** Notify the widget that a lore fragment was found (a gentle toast). */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void NotifyLoreFound(FName FragmentId);

	/** Show arbitrary toast text on the widget (ending beats, hints). */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowToast(const FText& Message);

	/** Set the persistent objective tracker line (empty hides it). */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetObjective(const FText& Objective);

	/** Set the NPC dialogue line (empty hides it). */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetDialogue(const FText& Line);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
	TSubclassOf<ULongNoonHUDWidget> HUDWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "HUD")
	TObjectPtr<ULongNoonHUDWidget> HUDWidget;
};
