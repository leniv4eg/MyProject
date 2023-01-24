// My first Unreal Engine 5 Game project


#include "Weapon/ShotgunWeapon.h"
#include <Engine/World.h>

void AShotgunWeapon::StartFire()
{
	MakeShot();
	GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &AShotgunWeapon::MakeShot, TimeBetweenShots, true);
}

void AShotgunWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(ShotTimerHandle);
}

void AShotgunWeapon::MakeShot()
{
	ABaseWeapon::MakeShot();
}
