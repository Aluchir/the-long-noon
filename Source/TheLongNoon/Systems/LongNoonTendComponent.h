#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LongNoonTendComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTendChanged, float, Stamina, float, Focus);

/**
 * "Tend, not Survive": upkeep meters with no death state. Stamina/Focus gate
 * exertion and regenerate; Comfort is a soft reward meter. Hitting zero slows the
 * player, never kills. See docs/SPRINT-1-DECISIONS.md section 4.
 */
UCLASS(ClassGroup = (LongNoon), meta = (BlueprintSpawnableComponent))
class THELONGNOON_API ULongNoonTendComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULongNoonTendComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable, Category = "Tend")
	FOnTendChanged OnTendChanged;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tend")
	float MaxStamina = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tend")
	float MaxFocus = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tend")
	float StaminaRegenPerSecond = 12.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tend")
	float FocusRegenPerSecond = 8.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tend")
	float Stamina = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tend")
	float Focus = 100.0f;

	/** Soft reward meter (0..1): good food, a cozy home. Grants gentle buffs, never punishes. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tend", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float Comfort = 0.5f;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Tend")
	bool HasStamina(float Amount) const { return Stamina >= Amount; }

	/** Spend stamina, clamped at 0 (never fails fatally). Returns true if full amount was available. */
	UFUNCTION(BlueprintCallable, Category = "Tend")
	bool SpendStamina(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Tend")
	void Rest();
};
