// My first Unreal Engine 5 Game project

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AmmoProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class MYPROJECT_API AAmmoProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AAmmoProjectile();

	void SetProjectileParams(const float AmmoAmount, const float AmmoSpeed, const float AmmoRadius, const bool AmmoFullDamage, const bool Launcher);
	void SetShotDirection(const FVector Direction) { ShotDirection = Direction; }

protected:
	
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
		USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
		UProjectileMovementComponent* MovementComponent;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	//	//float DamageAmount = 50.0f;
	//	float ProjectileDamage;
	//
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	//	//float AmmoSpeed = 50.0f;
	//	float ProjectileSpeed;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	//	//float DamageRadius = 200.0f;
	//	float ProjectileRadius;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	//	//bool DoFullDamage = false;
	//	bool ProjectileDoFullDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		float LifeSeconds;

	virtual void BeginPlay() override;  

private:
	FVector ShotDirection;
	float ProjectileDamage;
	float ProjectileRadius;
	bool ProjectileDoFullDamage;
	bool ProjectileLauncher;

	UFUNCTION()
		void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	AController* GetController() const;

};
