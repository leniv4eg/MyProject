// My first Unreal Engine 5 Game project

#include "Weapon/BaseWeapon.h"
#include <Components/SkeletalMeshComponent.h>
#include <Weapon/AmmoProjectile.h>
#include <GameFramework/Character.h>
#include <GameFramework/Controller.h>

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All);

class AAmmoProjectile;

ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();	

	check(WeaponMesh);
	checkf(DefaultAmmo.Bullets > 0, TEXT("Bullets must be more than zero"));
	checkf(DefaultAmmo.Clips > 0, TEXT("Clips must be more than zero"));
	CurrentAmmo = DefaultAmmo;
}

bool ABaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if(!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

	/*ViewLocation = this->GetActorLocation();
	ViewRotation = this->GetActorRotation();*/

	TraceStart = ViewLocation;
	const auto Spread = FMath::DegreesToRadians(BulletSpread);
	const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), Spread);
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	return true;
}

void ABaseWeapon::Fire()
{
	MakeShot();
}

void ABaseWeapon::StartFire()
{
}

void ABaseWeapon::StopFire()
{
}

void ABaseWeapon::MakeShot()
{
	UE_LOG(LogTemp, Display, TEXT("Make shot"));
	if (!GetWorld() || IsAmmoEmpty())
	{
		StopFire();
		return;
	}
	for (int32 i = 0; i < BulletInShot; i++)
	{
		FVector TraceStart, TraceEnd;
		//Надо изменить логику, чтобы патрон летел по направлению положения оружия
		if (!GetTraceData(TraceStart, TraceEnd)) 
		{
			StopFire();
			return;
		}

		FHitResult HitResult;
		MakeHit(HitResult, TraceStart, TraceEnd);

		const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
		const FVector Direction = (EndPoint - GeMuzzleWorldLocation(i)).GetSafeNormal();

		const FTransform SpawnTransform(FRotator::ZeroRotator, GeMuzzleWorldLocation(i));
		AAmmoProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAmmoProjectile>(ProjectileClass, SpawnTransform);
		if (Projectile)
		{
			Projectile->SetProjectileParams(DamageAmount, DamageSpeed, DamageRadius, DoFullDamage, IsLauncher);
			Projectile->SetShotDirection(Direction);
			Projectile->SetOwner(GetOwner());
			Projectile->FinishSpawning(SpawnTransform);
		}
		DecreaseAmmo();
	}
}

APlayerController* ABaseWeapon::GetPlayerController() const
{
	const auto Player = Cast<ACharacter>(GetOwner());
	if (!Player) return nullptr;
	return Player->GetController<APlayerController>();
}

bool ABaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
	const auto Controller = GetPlayerController();
	if (!Controller) return false;

	Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
	return true;
}

FVector ABaseWeapon::GeMuzzleWorldLocation(int32 AmmoCount) const
{
	FName FinalMuzzleSocketName = AmmoCount > 0 ? MuzzleSocketName : FName::FName(RootSocketName, AmmoCount);
	return WeaponMesh->GetSocketLocation(FinalMuzzleSocketName);
}

void ABaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
	if (!GetWorld()) return;

	FCollisionQueryParams CollisionParameters;
	CollisionParameters.AddIgnoredActor(GetOwner());

	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParameters);
}

void ABaseWeapon::DecreaseAmmo()
{
	if (CurrentAmmo.Bullets == 0)
	{
		UE_LOG(LogBaseWeapon, Warning, TEXT("Clip is empty"));
		return;
	}
	CurrentAmmo.Bullets--;
	LogAmmo();
	if (IsClipEmpty() && !IsAmmoEmpty())
	{
		//StopFire(); нужна ли, если без неё всё работает
		OnClipEmpty.Broadcast();
	}
}

bool ABaseWeapon::IsAmmoEmpty() const
{
	return !CurrentAmmo.Infinite && CurrentAmmo.Clips == 0 && IsClipEmpty();
}

bool ABaseWeapon::IsClipEmpty() const
{
	return CurrentAmmo.Bullets == 0;;
}

void ABaseWeapon::ChangeClip()
{
	if (!CurrentAmmo.Infinite)
	{
		if (CurrentAmmo.Clips == 0)
		{
			UE_LOG(LogBaseWeapon, Warning, TEXT("No clips"));
			return;
		}
		CurrentAmmo.Clips--;
	}
	CurrentAmmo.Bullets = DefaultAmmo.Bullets;
	UE_LOG(LogBaseWeapon, Display, TEXT("--- Change clip ---"));
}

bool ABaseWeapon::CanReload() const
{
	return CurrentAmmo.Bullets < DefaultAmmo.Bullets&& CurrentAmmo.Clips > 0;
}

void ABaseWeapon::LogAmmo()
{
	FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.Bullets) + "/";
	AmmoInfo += CurrentAmmo.Infinite ? "Infinite" : FString::FromInt(CurrentAmmo.Clips);
	UE_LOG(LogBaseWeapon, Display, TEXT("%s"), *AmmoInfo);
}
