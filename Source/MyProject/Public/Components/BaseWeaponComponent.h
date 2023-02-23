// My first Unreal Engine 5 Game project

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <BaseCoreTypes.h>
#include "BaseWeaponComponent.generated.h"

class ABaseWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UBaseWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBaseWeaponComponent();

	void Fire();
	void StartFire();
	void StopFire();
	void Reload();
	void PickUpWeapon();
	void ChangeWeapon(); //подобрать новое оружие взамен текущего
	void SetFirstWeapon();
	void SetSecondWeapon();
	void SetNoneWeapon();

	bool GetCurrentWeaponUIData(FWeaponUIData& UIData) const;
	bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		FName WeaponEquipSocketName = "weaponSocket";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		FName WeaponFirstArmorySocketName = "firstArmorySocket";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		FName WeaponSecondArmorySocketName = "secondArmorySocket";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		FName WeaponPistolSocketName = "pistolSocket";

	//UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	//	TSubclassOf<ABaseWeapon> WeaponClass;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		TArray<FWeaponData> WeaponData;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		UAnimMontage* EquipAnimMontage;*/

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY()
		ABaseWeapon* CurrentWeapon = nullptr;

	UPROPERTY()
		TArray<ABaseWeapon*> Weapons;

	UPROPERTY()
		UAnimMontage* CurrentEquioAnimMontageIn = nullptr;
	//
	//UPROPERTY()
	//	UAnimMontage* CurrentEquioAnimMontageOut = nullptr;
	
	UPROPERTY()
		UAnimMontage* CurrentReloadAnimMontage = nullptr;

	int32 CurrentWeaponIndex = 0;

	bool EquipAnimInProgress = false;
	bool ReloadAnimInProgress = false;

	void SpawnWeapons();
	void AttachWeaponToSocket(ABaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);
	void EquipWeapon(int32 WeaponIndex);
	void EquipWeaponHelper(ACharacter* Character, int32 WeaponIndex, FName SecondArmorySocketName);

	void PlayAnimMontage(UAnimMontage* Animation);
	void InitAnimations();
	
	void OnEquipFinished(USkeletalMeshComponent* MeshComponent);
	void OnReloadFinished(USkeletalMeshComponent* MeshComponent);

	bool CanFire() const;
	bool CanEquip() const;
	bool CanReload() const;

	void OnEmptyClip();
	void ChangeClip();
	

	void SwitchWeapon(int32 WeaponIndex); //смена между оружиями
};
