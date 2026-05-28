#include "Systems/LongNoonDataRegistry.h"
#include "Core/LongNoonDataSettings.h"
#include "Core/LongNoonLog.h"
#include "Engine/DataTable.h"

void ULongNoonDataRegistry::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const ULongNoonDataSettings* Settings = GetDefault<ULongNoonDataSettings>();
	if (!Settings)
	{
		return;
	}

	ItemTable = Settings->ItemTable.LoadSynchronous();
	ToolTable = Settings->ToolTable.LoadSynchronous();
	RecipeTable = Settings->RecipeTable.LoadSynchronous();
	RegionTable = Settings->RegionTable.LoadSynchronous();
	LoreFragmentTable = Settings->LoreFragmentTable.LoadSynchronous();

	UE_LOG(LogLongNoon, Log, TEXT("[DataRegistry] Tables loaded (items=%d, tools=%d, recipes=%d, regions=%d, fragments=%d)."),
		ItemTable ? ItemTable->GetRowNames().Num() : 0,
		ToolTable ? ToolTable->GetRowNames().Num() : 0,
		RecipeTable ? RecipeTable->GetRowNames().Num() : 0,
		RegionTable ? RegionTable->GetRowNames().Num() : 0,
		LoreFragmentTable ? LoreFragmentTable->GetRowNames().Num() : 0);
}

const FItemRow* ULongNoonDataRegistry::GetItem(FName ItemId) const
{
	return ItemTable ? ItemTable->FindRow<FItemRow>(ItemId, TEXT("GetItem"), false) : nullptr;
}

const FToolRow* ULongNoonDataRegistry::GetTool(FName ToolId) const
{
	return ToolTable ? ToolTable->FindRow<FToolRow>(ToolId, TEXT("GetTool"), false) : nullptr;
}

const FRecipeRow* ULongNoonDataRegistry::GetRecipe(FName RecipeId) const
{
	return RecipeTable ? RecipeTable->FindRow<FRecipeRow>(RecipeId, TEXT("GetRecipe"), false) : nullptr;
}

const FRegionRow* ULongNoonDataRegistry::GetRegion(FName RegionId) const
{
	return RegionTable ? RegionTable->FindRow<FRegionRow>(RegionId, TEXT("GetRegion"), false) : nullptr;
}

const FLoreFragmentRow* ULongNoonDataRegistry::GetFragment(FName FragmentId) const
{
	return LoreFragmentTable ? LoreFragmentTable->FindRow<FLoreFragmentRow>(FragmentId, TEXT("GetFragment"), false) : nullptr;
}

TArray<EReclamationVerb> ULongNoonDataRegistry::ParseVerbs(const FString& Packed)
{
	TArray<EReclamationVerb> Result;
	if (Packed.IsEmpty())
	{
		return Result;
	}

	const UEnum* VerbEnum = StaticEnum<EReclamationVerb>();
	TArray<FString> Parts;
	Packed.ParseIntoArray(Parts, TEXT("|"), true);
	for (FString& Part : Parts)
	{
		Part.TrimStartAndEndInline();
		const int64 Value = VerbEnum ? VerbEnum->GetValueByNameString(Part) : INDEX_NONE;
		if (Value != INDEX_NONE)
		{
			Result.Add(static_cast<EReclamationVerb>(Value));
		}
		else
		{
			UE_LOG(LogLongNoon, Warning, TEXT("[DataRegistry] Unknown verb '%s'."), *Part);
		}
	}
	return Result;
}

TArray<FParsedRecipeInput> ULongNoonDataRegistry::ParseRecipeInputs(const FString& Packed)
{
	TArray<FParsedRecipeInput> Result;
	if (Packed.IsEmpty())
	{
		return Result;
	}

	TArray<FString> Pairs;
	Packed.ParseIntoArray(Pairs, TEXT(";"), true);
	for (const FString& Pair : Pairs)
	{
		FString IdPart, QtyPart;
		if (Pair.Split(TEXT(":"), &IdPart, &QtyPart))
		{
			FParsedRecipeInput In;
			In.ItemId = FName(*IdPart.TrimStartAndEnd());
			In.Quantity = FMath::Max(1, FCString::Atoi(*QtyPart));
			Result.Add(In);
		}
	}
	return Result;
}
