// My first Unreal Engine 5 Game project


#include "Components/HealthComponent.h"
#include <GameFramework/Actor.h>
#include <Engine/World.h>
#include <TimerManager.h>

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All);

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	SetHealth(MaxHealth);
	Armor = MaxArmor;

	AActor* ComponentOwner = GetOwner();
	if (ComponentOwner)
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnTakeAnyDamage);
}

void UHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (!GetWorld() || IsDead() || Damage <= 0) return;
	UE_LOG(LogHealthComponent, Error, TEXT("Damage: %f"), Damage);
	if (Armor)
		SetArmor(Damage);
	else
		SetHealth(Health - Damage);

	if (IsDead())
		OnDeath.Broadcast();
	else if (AutoHeal)
		GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &UHealthComponent::HealUpdate, HealUpdateTime, true, HealDelay);
}

void UHealthComponent::SetHealth(float NewHealth, bool ItIsAutoHeal)
{
	if (ItIsAutoHeal)
		Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth / 100.0f * AutoHealMaxHealthInPercent);
	else
		Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
	OnHealChanged.Broadcast(Health);
}

void UHealthComponent::HealUpdate()
{
	if (Health < MaxHealth / 100.0f * AutoHealMaxHealthInPercent + HealModifier)
		SetHealth(Health + HealModifier, true);

	if (FMath::IsNearlyEqual(Health, MaxHealth) && GetWorld())
		GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
}

void UHealthComponent::SetArmor(float Damage)
{
	if (Damage > Armor)
	{
		float DelHealth = Damage - Armor;
		SetHealth(Health- DelHealth);
		Armor = FMath::Clamp(Armor - Damage, 0.0f, MaxArmor);
	}
	else
		Armor = FMath::Clamp(Armor - Damage, 0.0f, MaxArmor);

	//Armor = FMath::Clamp(Armor - Damage, 0.0f, MaxArmor);
	OnArmorChanged.Broadcast(Armor);
}


/*
ѕримечани€
MaxHealth / 100.0f * AutoHealMaxHealthInPercent --> как-то использовать это конструкцию, чтобы с автохилом восполн€лись жизни не полностью, а с аптечкой полностью
*/