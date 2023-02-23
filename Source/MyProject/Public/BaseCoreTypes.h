#pragma once

#include <BaseCoreTypes.generated.h>

// Weapon

class ABaseWeapon;

DECLARE_MULTICAST_DELEGATE(FOnClipEmptySignature);

USTRUCT(BlueprintType)
struct FAmmoData
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		int32 Bullets;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (EditCondition = "!Infinite"))
		int32 Clips;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		bool Infinite;
};

USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		TSubclassOf<ABaseWeapon> WeaponClass;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	//	ABaseWeapon* WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		UAnimMontage* ReloadAnimMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		UAnimMontage* EquipAnimMontageIn;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		UAnimMontage* EquipAnimMontageOut;
};

USTRUCT(BlueprintType)
struct FWeaponUIData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		UTexture2D* WeaponIcon;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		UTexture2D* CrossHairWeaponIcon;
};

//Health
DECLARE_MULTICAST_DELEGATE(FOnDeathSignature);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealChangedSignature, float);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnArmorChangedSignature, float);

//Walk
UENUM(BlueprintType)
enum class EMovementSpeedType :uint8
{
	Crawl,
	Walk,
	Base,
	Sprint
};
