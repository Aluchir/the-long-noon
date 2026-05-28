#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LoreFragmentDef.generated.h"

/**
 * A piece of rediscovered world memory for the codex. Readability scales with the
 * player's forgotten-script literacy tier. See docs/lore/codex-catalogue.md and
 * docs/lore/forgotten-script.md.
 */
UENUM(BlueprintType)
enum class ELoreFragmentType : uint8
{
	Inscription UMETA(DisplayName = "Inscription"),
	Artifact    UMETA(DisplayName = "Artifact"),
	Observation UMETA(DisplayName = "Observation"),
	Testimony   UMETA(DisplayName = "Testimony"),
	TrueName    UMETA(DisplayName = "True Name")
};

UCLASS(BlueprintType)
class THELONGNOON_API ULoreFragmentDef : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lore")
	FName FragmentId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lore")
	ELoreFragmentType Type = ELoreFragmentType::Observation;

	/** Narrative thread this fragment belongs to (Monument, Tally, Machines, TrueNames, Gardener, Rememberer). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lore")
	FName Thread;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lore", meta = (ClampMin = "1", ClampMax = "5"))
	int32 RegionTier = 1;

	/** Glyph (untranslated) text shown below the required literacy tier. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lore", meta = (MultiLine = true))
	FText GlyphText;

	/** Translation revealed once the player reaches RequiredLiteracyTier. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lore", meta = (MultiLine = true))
	FText TranslatedText;

	/** Literacy tier (0..4) required to read TranslatedText. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lore", meta = (ClampMin = "0", ClampMax = "4"))
	int32 RequiredLiteracyTier = 0;
};
