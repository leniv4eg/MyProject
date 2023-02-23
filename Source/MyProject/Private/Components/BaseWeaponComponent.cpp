// My first Unreal Engine 5 Game project


#include "Components/BaseWeaponComponent.h"
#include <Weapon/BaseWeapon.h>
#include "GameFramework/Character.h"
#include <Animations/EquipFinishedAnimNotify.h>
#include "Animations/ReloadFinishedAnimNotify.h"
#include <Animations/AnimUtils.h>

constexpr static int32 WeaponNum = 2;

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All);

UBaseWeaponComponent::UBaseWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UBaseWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	checkf(WeaponData.Num() == WeaponNum, TEXT("Can hold only %i weapon"), WeaponNum);

	CurrentWeaponIndex = 0;
	InitAnimations();	
	//Убрать SpawnWeapon(), когда сделаем обработку события по его пересечению.
	SpawnWeapons();
	EquipWeapon(CurrentWeaponIndex);
}

void UBaseWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason) 
{
	CurrentWeapon = nullptr;
	for (auto Weapon : Weapons)
	{
		Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Weapon->Destroy();
	}
	Weapons.Empty();
	Super::EndPlay(EndPlayReason);
}

void UBaseWeaponComponent::SpawnWeapons()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || !GetWorld()) return;

	for (auto OneWeaponData : WeaponData)
	{
		//auto Weapon = GetWorld()->SpawnActor<ABaseWeapon>(OneWeaponData.WeaponClass);
		auto Weapon = GetWorld()->SpawnActor<ABaseWeapon>(OneWeaponData.WeaponClass);
		if (!Weapon) continue;

		Weapon->OnClipEmpty.AddUObject(this, &UBaseWeaponComponent::OnEmptyClip);
		Weapon->SetOwner(Character);
		Weapons.Add(Weapon);

		AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponFirstArmorySocketName);
	}
}

//void UBaseWeaponComponent::Fire()
//{
//	if (!CurrentWeapon) return;
//	CurrentWeapon->Fire();
//}

void UBaseWeaponComponent::StartFire()
{
	if (!CanFire()) return;
	CurrentWeapon->StartFire();
}

void UBaseWeaponComponent::StopFire()
{
	if (!CurrentWeapon) return;
	CurrentWeapon->StopFire();
}

void UBaseWeaponComponent::Reload()
{
	ChangeClip();
}

void UBaseWeaponComponent::PickUpWeapon()
{

}

void UBaseWeaponComponent::SwitchWeapon(int32 WeaponIndex) //смена между оружиями
{
	if (!CanEquip()) return;
	//CurrentWeaponIndex = WeaponIndex;
	EquipWeapon(WeaponIndex);
}

void UBaseWeaponComponent::ChangeWeapon() //подобрать новое оружие взамен текущего
{

}

void UBaseWeaponComponent::AttachWeaponToSocket(ABaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName)
{
	if (!Weapon || !SceneComponent) return;
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

void UBaseWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
	//Не даёт убрать всё оружие. Изменить логику!
	/*if (WeaponIndex < 0 || WeaponIndex >= Weapons.Num())
	{
		UE_LOG(LogWeaponComponent, Warning, TEXT("Invalid weapon index"));
		return;
	}*/

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	switch (WeaponIndex)
	{
	case 0:
		/*if (CurrentWeapon)
		{
			CurrentWeapon->StopFire();
			auto WeaponSocketName = CurrentWeapon->ReturnIsPistol() ? WeaponPistolSocketName : WeaponSecondArmorySocketName;
			AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponSocketName);
			EquipAnimInProgress = true;
			//PlayAnimMontage(EquipAnimMontage);
			PlayAnimMontage(CurrentWeapon->GetEquipAnimMontageOut());
		}

		CurrentWeapon = Weapons[WeaponIndex];
		//CurrentReloadAnimMontage = WeaponData[WeaponIndex].ReloadAnimMontage;
		const auto CurrentWeaponData = WeaponData.FindByPredicate([&](const FWeaponData& Data) {
			return Data.WeaponClass == CurrentWeapon->GetClass(); 
			});
		CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;
		
		AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);
		PlayAnimMontage(CurrentWeapon->GetEquipAnimMontageIn()); */
		EquipWeaponHelper( Character, WeaponIndex, WeaponSecondArmorySocketName);
		break;

	case 1:
		/*if (CurrentWeapon)
		{
			CurrentWeapon->StopFire();
			auto WeaponSocketName = CurrentWeapon->ReturnIsPistol() ? WeaponPistolSocketName : WeaponFirstArmorySocketName;
			AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponSocketName);
			EquipAnimInProgress = true;
			//PlayAnimMontage(EquipAnimMontage);
			PlayAnimMontage(CurrentWeapon->GetEquipAnimMontageOut());
		}

		CurrentWeapon = Weapons[WeaponIndex];
		//CurrentReloadAnimMontage = WeaponData[WeaponIndex].ReloadAnimMontage;
		const auto CurrentWeaponData = WeaponData.FindByPredicate([&](const FWeaponData& Data) {
			return Data.WeaponClass == CurrentWeapon->GetClass();
			});
		CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;

		AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);
		PlayAnimMontage(CurrentWeapon->GetEquipAnimMontageIn());*/
		EquipWeaponHelper(Character, WeaponIndex, WeaponFirstArmorySocketName);
		break;
	//
	case 2:
		if (CurrentWeapon)
		{
			//if (CurrentWeaponIndex == 0)
			//{
				auto WeaponSocketName = CurrentWeapon->ReturnIsPistol() ? WeaponPistolSocketName : (CurrentWeaponIndex == 0 ? WeaponFirstArmorySocketName : WeaponSecondArmorySocketName);
				EquipAnimInProgress = true;
				PlayAnimMontage(CurrentWeapon->GetEquipAnimMontageOut());
				AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponSocketName);
			//}
			//else if (CurrentWeaponIndex == 1)
			//{
			//	auto WeaponSocketName = CurrentWeapon->ReturnIsPistol() ? WeaponPistolSocketName : WeaponSecondArmorySocketName;
			//	EquipAnimInProgress = true;
			//	//PlayAnimMontage(EquipAnimMontage);
			//	PlayAnimMontage(CurrentWeapon->GetEquipAnimMontageOut());
			//	AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponSocketName);
			//}
			CurrentWeapon = nullptr;
			CurrentWeaponIndex = WeaponIndex;
			CurrentReloadAnimMontage = nullptr;
			
			//const auto CurrentWeaponData = WeaponData.FindByPredicate([&](const FWeaponData& Data) {
			//	return Data.WeaponClass == CurrentWeapon->GetClass();
			//	});
			//CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;
		}
	}

	/*//if (CurrentWeapon)
	//	AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponFirstArmorySocketName);

	//CurrentWeapon = Weapons[WeaponIndex];
	//AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);*/
}

void UBaseWeaponComponent::EquipWeaponHelper(ACharacter* Character, int32 WeaponIndex, FName SecondArmorySocketName)
{
	//Здесь сломана логика воспроизведения анимаций!!!
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
		auto WeaponSocketName = CurrentWeapon->ReturnIsPistol() ? WeaponPistolSocketName : SecondArmorySocketName;
		EquipAnimInProgress = true;
		PlayAnimMontage(CurrentWeapon->GetEquipAnimMontageOut());
		//PlayAnimMontage(CurrentEquioAnimMontageIn);
		AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponSocketName);
	}

	CurrentWeapon = Weapons[WeaponIndex];
	/*const auto CurrentWeaponData = WeaponData.FindByPredicate([&](const FWeaponData& Data) {
		return Data.WeaponClass == CurrentWeapon->GetClass();
		});
	CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;*/
	CurrentReloadAnimMontage = CurrentWeapon->GetReloadAnimMontage();
	//CurrentEquioAnimMontageOut = CurrentWeapon->GetEquipAnimMontageOut();
	EquipAnimInProgress = true;
	PlayAnimMontage(CurrentWeapon->GetEquipAnimMontageIn());
	//PlayAnimMontage(CurrentEquioAnimMontageOut);
	AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);
}

void UBaseWeaponComponent::PlayAnimMontage(UAnimMontage* Animation)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	Character->PlayAnimMontage(Animation);
}

void UBaseWeaponComponent::InitAnimations()
{
	/*auto EquipFinishedNotify = AnimUtils::FindNotifyByClass<UEquipFinishedAnimNotify>(EquipAnimMontage);
	if (EquipFinishedNotify)
	{
		EquipFinishedNotify->OnNotified.AddUObject(this, &UBaseWeaponComponent::OnEquipFinished);
	}
	else
	{
		UE_LOG(LogWeaponComponent, Error, TEXT("Equip anim notify is forgotten to set"));
		checkNoEntry();
	}*/

	/*Надо как-то обратиться к WeaponClass, чтобы получить анимациb EquipAnimMontageIn и ReloadAnimMontage, а из структуры WeaponData их убрать*/
	for (auto OneWeaponData : WeaponData)
	{
		auto EquipFinishedNotify = AnimUtils::FindNotifyByClass<UEquipFinishedAnimNotify>(OneWeaponData.EquipAnimMontageIn);
		if (EquipFinishedNotify)
		{
			EquipFinishedNotify->OnNotified.AddUObject(this, &UBaseWeaponComponent::OnEquipFinished);
		}
		else
		{
			UE_LOG(LogWeaponComponent, Error, TEXT("Equip anim notify is forgotten to set"));
			checkNoEntry();
		}
		//auto EquipFinishedNotify = AnimUtils::FindNotifyByClass<UEquipFinishedAnimNotify>(Cast<ABaseWeapon>(OneWeaponData.WeaponClass)->GetEquipAnimMontageOut());
		//if (EquipFinishedNotify)
		//{
		//	EquipFinishedNotify->OnNotified.AddUObject(this, &UBaseWeaponComponent::OnEquipFinished);
		//}
		//else
		//{
		//	UE_LOG(LogWeaponComponent, Error, TEXT("Equip anim notify is forgotten to set"));
		//	checkNoEntry();
		//}
	}

	for (auto OneWeaponData : WeaponData)
	{
		auto ReloadFinishedNotify = AnimUtils::FindNotifyByClass<UReloadFinishedAnimNotify>(OneWeaponData.ReloadAnimMontage);
		if (!ReloadFinishedNotify) //continue;
		{
			UE_LOG(LogWeaponComponent, Error, TEXT("Reload anim notify is forgotten to set"));
			checkNoEntry();
		}
		ReloadFinishedNotify->OnNotified.AddUObject(this, &UBaseWeaponComponent::OnReloadFinished);
	}
}

void UBaseWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComponent)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || MeshComponent != Character->GetMesh()) return;

	EquipAnimInProgress = false;
}

void UBaseWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComponent)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || MeshComponent != Character->GetMesh()) return;

	ReloadAnimInProgress = false;
}

bool UBaseWeaponComponent::CanFire() const
{
	return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress;
}

bool UBaseWeaponComponent::CanEquip() const
{
	return !EquipAnimInProgress && !ReloadAnimInProgress;
}

bool UBaseWeaponComponent::CanReload() const
{
	return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress /* && CurrentWeapon->CanReload()*/;
}

void UBaseWeaponComponent::OnEmptyClip()
{
	ChangeClip();
}

void UBaseWeaponComponent::ChangeClip()
{
	if (!CanReload()) return;
	CurrentWeapon->StopFire();
	CurrentWeapon->ChangeClip();
	ReloadAnimInProgress = true;
	//PlayAnimMontage(CurrentReloadAnimMontage);
	PlayAnimMontage(CurrentWeapon->GetReloadAnimMontage());
}

void UBaseWeaponComponent::SetFirstWeapon()
{
	SwitchWeapon(0);
}

void UBaseWeaponComponent::SetSecondWeapon()
{
	SwitchWeapon(1);
}

void UBaseWeaponComponent::SetNoneWeapon()
{
	SwitchWeapon(2);
}

bool UBaseWeaponComponent::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
	if (CurrentWeapon)
	{
		UIData = CurrentWeapon->GetUIData();
		return true;
	}
	return false;
}

bool UBaseWeaponComponent::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
	if (CurrentWeapon)
	{
		AmmoData = CurrentWeapon->GetAmmoData();
		return true;
	}
	return false;
}