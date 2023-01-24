// My first Unreal Engine 5 Game project

#pragma once

#include "CoreMinimal.h"
#include "Weapon/BaseWeapon.h"
#include "ShotgunWeapon.generated.h"

UCLASS()
class MYPROJECT_API AShotgunWeapon : public ABaseWeapon
{
	GENERATED_BODY()
public:
	virtual void StartFire() override;
	virtual void StopFire() override;

protected:
	virtual void MakeShot();
	
private:
	FTimerHandle ShotTimerHandle;
};
