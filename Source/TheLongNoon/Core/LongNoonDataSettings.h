#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "LongNoonDataSettings.generated.h"

class UDataTable;

/**
 * Project Settings entry (Project -> The Long Noon) holding the content
 * DataTables. Assign the tables imported from Content/Data/*.csv here; the
 * ULongNoonDataRegistry reads them on init. See docs/tech/data-authoring-guide.md.
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "The Long Noon Data"))
class THELONGNOON_API ULongNoonDataSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	virtual FName GetCategoryName() const override { return FName("Game"); }

	UPROPERTY(Config, EditAnywhere, Category = "Data Tables", meta = (AllowedClasses = "/Script/Engine.DataTable"))
	TSoftObjectPtr<UDataTable> ItemTable;

	UPROPERTY(Config, EditAnywhere, Category = "Data Tables", meta = (AllowedClasses = "/Script/Engine.DataTable"))
	TSoftObjectPtr<UDataTable> ToolTable;

	UPROPERTY(Config, EditAnywhere, Category = "Data Tables", meta = (AllowedClasses = "/Script/Engine.DataTable"))
	TSoftObjectPtr<UDataTable> RecipeTable;

	UPROPERTY(Config, EditAnywhere, Category = "Data Tables", meta = (AllowedClasses = "/Script/Engine.DataTable"))
	TSoftObjectPtr<UDataTable> RegionTable;

	UPROPERTY(Config, EditAnywhere, Category = "Data Tables", meta = (AllowedClasses = "/Script/Engine.DataTable"))
	TSoftObjectPtr<UDataTable> LoreFragmentTable;

	UPROPERTY(Config, EditAnywhere, Category = "Data Tables", meta = (AllowedClasses = "/Script/Engine.DataTable"))
	TSoftObjectPtr<UDataTable> NpcTable;

	UPROPERTY(Config, EditAnywhere, Category = "Data Tables", meta = (AllowedClasses = "/Script/Engine.DataTable"))
	TSoftObjectPtr<UDataTable> DialogueLineTable;
};
