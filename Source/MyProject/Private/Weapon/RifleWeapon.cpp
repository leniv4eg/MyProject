// My first Unreal Engine 5 Game project


#include "Weapon/RifleWeapon.h"
#include <Engine/World.h>

void ARifleWeapon::StartFire()
{
	GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ARifleWeapon::MakeShot, TimeBetweenShots, true);
	MakeShot();
}

void ARifleWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(ShotTimerHandle);
}

void ARifleWeapon::MakeShot()
{
	ABaseWeapon::MakeShot();
}
