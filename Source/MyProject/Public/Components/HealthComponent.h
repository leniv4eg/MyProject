// My first Unreal Engine 5 Game project

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <BaseCoreTypes.h>
#include "HealthComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();

	FOnDeathSignature OnDeath;
	FOnHealChangedSignature OnHealChanged;
	FOnArmorChangedSignature OnArmorChanged;

	UFUNCTION(BlueprintCallable)
		bool IsDead() const { return FMath::IsNearlyZero(Health); }

	float GetHealth() const { return Health; }

	float GetArmor() const { return Armor; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0.0", ClampMax = "500.0"))
		float MaxHealth = 100.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0.0", ClampMax = "100.0"))
		float AutoHealMaxHealthInPercent = 75.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
		bool AutoHeal = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal", ClampMin = "0.0", ClapmMax = "1.0"))
		float HealUpdateTime = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal", ClampMin = "0.0", ClapmMax = "5.0"))
		float HealDelay = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal", ClampMin = "1.0", ClampMax = "10.0"))
		float HealModifier = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Armor", meta = (ClampMin = "0.0", ClampMax = "100.0"))
		float MaxArmor = 100.0f;
	
	virtual void BeginPlay() override;

private:
	float Health = 0.0f;
	float Armor = 0.0f;
	FTimerHandle HealTimerHandle;

	UFUNCTION()
		void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	void SetHealth(float NewHealth, bool ItIsAutoHeal = false);
	void HealUpdate();

	void SetArmor(float NewArmor);
};
