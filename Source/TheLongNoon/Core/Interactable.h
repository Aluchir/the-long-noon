#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

UINTERFACE(MinimalAPI, BlueprintType)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * Anything the Newcomer can interact with via the interact trace: NPCs, gather
 * nodes, lore fragments, gate-builds. Implemented in C++ (override the
 * _Implementation) or in Blueprint. See docs/design/ and the character's Interact().
 */
class THELONGNOON_API IInteractable
{
	GENERATED_BODY()

public:
	/** Called when the player interacts. Interactor is the player actor. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
	void OnInteract(AActor* Interactor);

	/** Short verb-prompt for the HUD ("Talk", "Gather", "Read"). */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
	FText GetInteractPrompt() const;
};
