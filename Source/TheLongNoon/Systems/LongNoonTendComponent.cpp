#include "Systems/LongNoonTendComponent.h"

ULongNoonTendComponent::ULongNoonTendComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void ULongNoonTendComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const float NewStamina = FMath::Min(MaxStamina, Stamina + StaminaRegenPerSecond * DeltaTime);
	const float NewFocus = FMath::Min(MaxFocus, Focus + FocusRegenPerSecond * DeltaTime);

	if (!FMath::IsNearlyEqual(NewStamina, Stamina) || !FMath::IsNearlyEqual(NewFocus, Focus))
	{
		Stamina = NewStamina;
		Focus = NewFocus;
		OnTendChanged.Broadcast(Stamina, Focus);
	}
}

bool ULongNoonTendComponent::SpendStamina(float Amount)
{
	const bool bHadEnough = Stamina >= Amount;
	Stamina = FMath::Max(0.0f, Stamina - Amount);
	OnTendChanged.Broadcast(Stamina, Focus);
	return bHadEnough;
}

void ULongNoonTendComponent::Rest()
{
	Stamina = MaxStamina;
	Focus = MaxFocus;
	OnTendChanged.Broadcast(Stamina, Focus);
}
