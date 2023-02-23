// My first Unreal Engine 5 Game project


#include "UI/PlayerHUDWidget.h"
#include "Components/BaseWeaponComponent.h"
#include "Components/HealthComponent.h"
#include "GameUtils.h"

float UPlayerHUDWidget::GetHealthPercent() const
{
	const auto HealthComponent = GameUtils::GetPlayerComponentTemplate<UHealthComponent>(GetOwningPlayerPawn());
	if (!HealthComponent) return 0.0f;
	return HealthComponent->GetHealthPercent();
}

float UPlayerHUDWidget::GetArmorValue() const
{
	const auto HealthComponent = GameUtils::GetPlayerComponentTemplate<UHealthComponent>(GetOwningPlayerPawn());
	if (!HealthComponent) return 0.0f;
	return HealthComponent->GetArmorPercent();
}

bool UPlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
	const auto WeaponComponent = GameUtils::GetPlayerComponentTemplate<UBaseWeaponComponent>(GetOwningPlayerPawn());
	if (!WeaponComponent) return false;
	return WeaponComponent->GetCurrentWeaponUIData(UIData);
}

bool UPlayerHUDWidget::GetCurrentWeaponAmmo(FAmmoData& AmmoData) const
{
	const auto WeaponComponent = GameUtils::GetPlayerComponentTemplate<UBaseWeaponComponent>(GetOwningPlayerPawn());
	if (!WeaponComponent) return false;
	return WeaponComponent->GetCurrentWeaponAmmoData(AmmoData);
}

bool UPlayerHUDWidget::IsPlayerAlive() const
{
	const auto HealthComponent = GameUtils::GetPlayerComponentTemplate<UHealthComponent>(GetOwningPlayerPawn());
	return HealthComponent && !HealthComponent->IsDead();
}

bool UPlayerHUDWidget::IsPlayerSpectating() const
{
	const auto Controller = GetOwningPlayer();
	return Controller && Controller->GetStateName() == NAME_Spectating;
}