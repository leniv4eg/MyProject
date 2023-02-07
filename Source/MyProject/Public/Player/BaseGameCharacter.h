// My first Unreal Engine 5 Game project

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <BaseCoreTypes.h>
#include "BaseGameCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UHealthComponent;
class UTextRenderComponent;
class UBaseWeaponComponent;
class ABaseWeapon;

UCLASS()
class MYPROJECT_API ABaseGameCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseGameCharacter(const FObjectInitializer& ObjInit);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UTextRenderComponent* HealthTextComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UTextRenderComponent* ArmorTextComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
		UAnimMontage* DeathAnimMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UBaseWeaponComponent* WeaponComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Damage", meta = (ClampMin = "1.0", ClampMax = "5.0"))
		float LifeSpanDeath = 3.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		FVector2D LandedDamageVelocity = FVector2D(900.0f, 1200.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		FVector2D LandedDamage = FVector2D(10.0f, 100.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		TSubclassOf<ABaseWeapon> WeaponClass;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	EMovementSpeedType SpeedType;

	UFUNCTION(BlueprintCallable, Category = "Movement")
		bool IsCrawling() const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
		bool IsWalking() const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
		bool IsSprinting() const;
	
	UFUNCTION(BlueprintCallable, Category = "Movement")
		bool IsAimed() const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
		float GetMovementDirection() const;

private:
	bool IsCrawl = false;
	bool IsWalk = false;
	bool IsSprint = false;
	bool IsArmed = false;

	void MoveForward(float Amount);
	void MoveRight(float Amount);

	void SetIsCrawl();
	void SetIsWalk();
	void SetIsSprint();
	void SetStatus(bool StatusToCheck, bool FirstStatusCheck, bool SecondStatusCheck, EMovementSpeedType SpeedTypeToSetup, EMovementSpeedType SpeedType);
	void SetAimed();

	void OnDeath();
	void OnHealthChanged(float Health);
	void OnArmorChanged(float Armor);

	UFUNCTION()
		void OnGroundLanded(const FHitResult& Hit);
};
