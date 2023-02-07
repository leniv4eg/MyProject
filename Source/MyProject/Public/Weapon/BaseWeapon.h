// My first Unreal Engine 5 Game project

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <BaseCoreTypes.h>
#include "BaseWeapon.generated.h"

class USkeletalMeshComponent;
class AAmmoProjectile;

UCLASS()
class MYPROJECT_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseWeapon();

	FOnClipEmptySignature OnClipEmpty;

	virtual void Fire();
	virtual void StartFire();
	virtual void StopFire();
	bool ReturnIsPistol() { return IsPistol; }
	UAnimMontage* GetEquipAnimMontageIn() { return EquipAnimMontageIn; }
	UAnimMontage* GetEquipAnimMontageOut() { return EquipAnimMontageOut; }

	void ChangeClip();
	bool CanReload() const;

protected:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		TSubclassOf<AAmmoProjectile> ProjectileClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		FName MuzzleSocketName = "MuzzleSocket";
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		FName RootSocketName = "RootSocket_";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "100.0", ClampMax = "20000.0"))
		float TraceMaxDistance = 1500.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		bool IsLauncher = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		bool IsPistol = false;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	//	FAmmoData DefaultAmmo {	15, 10, false };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "1.0", ClampMax = "200.0"))
		float DamageAmount = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "10.0", ClampMax = "10000.0"))
		float DamageSpeed = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "1.0", ClampMax = "20.0"))
		float DamageRadius = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "1", ClampMax = "20"))
		int32 BulletInShot = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "1.0", ClampMax = "10.0"))
		float BulletSpread = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "0.0", ClampMax = "5.0"))
		float TimeBetweenShots = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		bool DoFullDamage = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		UAnimMontage* EquipAnimMontageIn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		UAnimMontage* EquipAnimMontageOut;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		FAmmoData DefaultAmmo {	15, 10, false };

	virtual void BeginPlay() override;
	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;

	virtual void MakeShot();
	//void MakeDamage(const FHitResult& HitResult);
	APlayerController* ABaseWeapon::GetPlayerController() const;
	bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;
	FVector GeMuzzleWorldLocation(int32 AmmoCount) const;
	void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);

	void DecreaseAmmo();
	bool IsAmmoEmpty() const;
	bool IsClipEmpty() const;
	void LogAmmo();

private:
	FAmmoData CurrentAmmo;
};
