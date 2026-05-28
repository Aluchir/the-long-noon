#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/ToolDef.h"
#include "BloomActor.generated.h"

class UStaticMeshComponent;

UENUM(BlueprintType)
enum class EBloomState : uint8
{
	Active  UMETA(DisplayName = "Active"),
	Dormant UMETA(DisplayName = "Dormant (stilled)"),
	Settled UMETA(DisplayName = "Settled (asleep)")
};

/**
 * A patch of the undying Bloom: the reclamation target. Never "dies" — it is
 * pruned, stilled (dormant), or settled (asleep), then regrows. Requires a specific
 * verb to subdue. See docs/design/reclamation-combat.md and docs/lore/metaphysics-bible.md.
 */
UCLASS()
class THELONGNOON_API ABloomActor : public AActor
{
	GENERATED_BODY()

public:
	ABloomActor();

	/** Verb that subdues this growth (Prune for inert, Settle for animate, etc.). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bloom")
	EReclamationVerb RequiredVerb = EReclamationVerb::Prune;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bloom", meta = (ClampMin = "1", ClampMax = "5"))
	int32 BloomTier = 1;

	/** Materials granted when subdued (item id -> quantity). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bloom")
	TMap<FName, int32> DropTable;

	/** Seconds before a subdued patch regrows to Active. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bloom")
	float RegrowSeconds = 60.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bloom")
	EBloomState State = EBloomState::Active;

	/**
	 * Attempt to reclaim with the given verb. Returns true and fills OutDrops if the
	 * verb matches and the patch was Active. Never destroys the actor.
	 */
	UFUNCTION(BlueprintCallable, Category = "Bloom")
	bool TryReclaim(EReclamationVerb Verb, TMap<FName, int32>& OutDrops);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bloom")
	TObjectPtr<UStaticMeshComponent> Mesh;

	void Regrow();

	FTimerHandle RegrowTimer;
};
