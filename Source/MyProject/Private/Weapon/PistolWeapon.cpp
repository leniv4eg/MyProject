// My first Unreal Engine 5 Game project


#include "Weapon/PistolWeapon.h"
//#include <Weapon/AmmoProjectile.h>

//void APistolWeapon::StartFire()
//{
//	MakeShot();
//}

//void APistolWeapon::MakeShot()
//{
//	if (!GetWorld()) return;
//
//	FVector TraceStart, TraceEnd;
//	if (!GetTraceData(TraceStart, TraceEnd)) return;
//
//	FHitResult HitResult;
//	MakeHit(HitResult, TraceStart, TraceEnd);
//
//	const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
//	const FVector Direction = (EndPoint - GeMuzzleWorldLocation()).GetSafeNormal();
//
//	const FTransform SpawnTransform(FRotator::ZeroRotator, GeMuzzleWorldLocation());
//	AAmmoProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAmmoProjectile>(ProjectileClass, SpawnTransform);
//	if(Projectile)
//	{
//		Projectile->SetProjectileParams(DamageAmount, DamageSpeed, DamageRadius, DoFullDamage, IsLauncher);
//		Projectile->SetShotDirection(Direction);
//		Projectile->SetOwner(GetOwner());
//		Projectile->FinishSpawning(SpawnTransform);
//	}
//}

/*

void APistolWeapon::MakeDamage(FHitResult& HitResult)
{

}*/
