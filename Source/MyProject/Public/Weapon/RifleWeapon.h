// My first Unreal Engine 5 Game project

#pragma once

#include "CoreMinimal.h"
#include "Weapon/BaseWeapon.h"
#include "RifleWeapon.generated.h"

UCLASS()
class MYPROJECT_API ARifleWeapon : public ABaseWeapon
{
	GENERATED_BODY()

public:
	virtual void StartFire() override;
	virtual void StopFire() override;
	
protected:
	virtual void MakeShot();
	
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	//	float TimeBetweenShots = 0.1f;

private:
	FTimerHandle ShotTimerHandle;
};