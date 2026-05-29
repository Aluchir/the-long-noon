#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Data/LongNoonRowTypes.h"
#include "LongNoonDataRegistry.generated.h"

class UDataTable;

/**
 * Loads the content DataTables (from ULongNoonDataSettings) and serves rows by id,
 * plus parses the packed string columns (tool verbs, recipe inputs). This is the
 * connective tissue between the CSVs and the gameplay systems.
 * See docs/tech/data-authoring-guide.md and docs/tech/coding-standards-and-architecture.md.
 */
UCLASS()
class THELONGNOON_API ULongNoonDataRegistry : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	const FItemRow* GetItem(FName ItemId) const;
	const FToolRow* GetTool(FName ToolId) const;
	const FRecipeRow* GetRecipe(FName RecipeId) const;
	const FRegionRow* GetRegion(FName RegionId) const;
	const FLoreFragmentRow* GetFragment(FName FragmentId) const;
	const FNpcRow* GetNpc(FName NpcId) const;
	const FDialogueLineRow* GetDialogueLine(FName LineId) const;

	/** Resolve a pipe-separated list of dialogue line ids to their texts. */
	UFUNCTION(BlueprintCallable, Category = "Data")
	TArray<FText> GetDialogueLines(const FString& PackedLineIds) const;

	/** Parse a pipe-separated verb list ("Seal|Prune") into verbs. */
	UFUNCTION(BlueprintCallable, Category = "Data")
	static TArray<EReclamationVerb> ParseVerbs(const FString& Packed);

	/** Parse "itemId:qty;itemId:qty" into item/quantity pairs. */
	UFUNCTION(BlueprintCallable, Category = "Data")
	static TArray<FParsedRecipeInput> ParseRecipeInputs(const FString& Packed);

private:
	UPROPERTY() TObjectPtr<UDataTable> ItemTable;
	UPROPERTY() TObjectPtr<UDataTable> ToolTable;
	UPROPERTY() TObjectPtr<UDataTable> RecipeTable;
	UPROPERTY() TObjectPtr<UDataTable> RegionTable;
	UPROPERTY() TObjectPtr<UDataTable> LoreFragmentTable;
	UPROPERTY() TObjectPtr<UDataTable> NpcTable;
	UPROPERTY() TObjectPtr<UDataTable> DialogueLineTable;
};
