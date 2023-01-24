// My first Unreal Engine 5 Game project

#pragma once

#include "CoreMinimal.h"
#include "Weapon/BaseWeapon.h"
#include "PistolWeapon.generated.h"

//class AAmmoProjectile;

UCLASS()
class MYPROJECT_API APistolWeapon : public ABaseWeapon
{
	GENERATED_BODY()

public:
	virtual void StartFire() override { MakeShot(); }

protected:
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	//	TSubclassOf<AAmmoProjectile> ProjectileClass;

	//virtual void MakeShot() override;

//private:
//	void MakeDamage(FHitResult& HitResult);
};
