// My first Unreal Engine 5 Game project


#include "Player/BaseGameCharacter.h"
#include "Camera/CameraComponent.h"
#include <Components/InputComponent.h>
#include "GameFramework/SpringArmComponent.h"
#include "Components/BaseCharacterMovementComponent.h"
#include "Components/HealthComponent.h"
#include <Components/TextRenderComponent.h>
#include <Components/BaseWeaponComponent.h>
#include <Components/CapsuleComponent.h>
#include <BaseCoreTypes.h>
#include <GameFramework/Controller.h>


DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All);

// Sets default values
ABaseGameCharacter::ABaseGameCharacter(const FObjectInitializer& ObjInit) :Super(ObjInit.SetDefaultSubobjectClass<UBaseCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
	
	HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
	HealthTextComponent->SetupAttachment(GetRootComponent());
	//HealthTextComponent->SetOwnerNoSee(true);

	ArmorTextComponent = CreateDefaultSubobject<UTextRenderComponent>("ArmorTextComponent");
	ArmorTextComponent->SetupAttachment(GetRootComponent());
	//ArmorTextComponent->SetOwnerNoSee(true);

	WeaponComponent = CreateDefaultSubobject<UBaseWeaponComponent>("WeaponComponent");
}

// Called when the game starts or when spawned
void ABaseGameCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	check(HealthComponent);
	check(HealthTextComponent);
	check(ArmorTextComponent);
	check(GetCharacterMovement());
	check(WeaponComponent);

	OnHealthChanged(HealthComponent->GetHealth());
	OnArmorChanged(HealthComponent->GetArmor());
	HealthComponent->OnDeath.AddUObject(this, &ABaseGameCharacter::OnDeath);
	HealthComponent->OnHealChanged.AddUObject(this, &ABaseGameCharacter::OnHealthChanged);
	HealthComponent->OnArmorChanged.AddUObject(this, &ABaseGameCharacter::OnArmorChanged);

	LandedDelegate.AddDynamic(this, &ABaseGameCharacter::OnGroundLanded);
}

// Called every frame
void ABaseGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	check(WeaponComponent);

	PlayerInputComponent->BindAxis("CharMoveForward",	this, &ABaseGameCharacter::MoveForward);
	PlayerInputComponent->BindAxis("CharMoveRight",		this, &ABaseGameCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp",			this, &ABaseGameCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnAround",		this, &ABaseGameCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Jump",	IE_Pressed,	this, &ABaseGameCharacter::Jump);
	PlayerInputComponent->BindAction("Crawl",	IE_Pressed,	this, &ABaseGameCharacter::SetIsCrawl);
	PlayerInputComponent->BindAction("Walk",	IE_Pressed,	this, &ABaseGameCharacter::SetIsWalk);
	PlayerInputComponent->BindAction("Sprint",	IE_Pressed,	this, &ABaseGameCharacter::SetIsSprint);
	/* Было изначально так
	//PlayerInputComponent->BindAction("Crawl", IE_Pressed, this, &ABaseGameCharacter::OnStartCrawling);
	//PlayerInputComponent->BindAction("Crawl", IE_Released, this, &ABaseGameCharacter::OnStopCrawling);
	//PlayerInputComponent->BindAction("Walk", IE_Pressed, this, &ABaseGameCharacter::OnStartWalking);
	//PlayerInputComponent->BindAction("Walk", IE_Released, this, &ABaseGameCharacter::OnStopWalking);
	//PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ABaseGameCharacter::OnStartSprinting);
	//PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ABaseGameCharacter::OnStopSprinting);*/

	//PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &UBaseWeaponComponent::Fire);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &UBaseWeaponComponent::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &UBaseWeaponComponent::StopFire);
	PlayerInputComponent->BindAction("Aimed", IE_Pressed, this, &ABaseGameCharacter::SetAimed);
	PlayerInputComponent->BindAction("FirstWeapon", IE_Pressed, WeaponComponent, &UBaseWeaponComponent::SetFirstWeapon);
	PlayerInputComponent->BindAction("SecondWeapon", IE_Pressed, WeaponComponent, &UBaseWeaponComponent::SetSecondWeapon);
	PlayerInputComponent->BindAction("NoneWeapon", IE_Pressed, WeaponComponent, &UBaseWeaponComponent::SetNoneWeapon);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &UBaseWeaponComponent::Reload);
}

bool ABaseGameCharacter::IsCrawling() const
{
	return IsCrawl /* && !GetVelocity().IsZero()*/;
}

bool ABaseGameCharacter::IsWalking() const
{
	return IsWalk && !GetVelocity().IsZero();
}

bool ABaseGameCharacter::IsSprinting() const
{
	return IsSprint && !GetVelocity().IsZero();
}

bool ABaseGameCharacter::IsAimed() const
{
	return IsArmed;
}

float ABaseGameCharacter::GetMovementDirection() const
{
	if (GetVelocity().IsZero()) return 0.0f;

	const auto VelocityNormal = GetVelocity().GetSafeNormal();
	const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
	const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
	const auto Degrees = FMath::RadiansToDegrees(AngleBetween);

	return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}

void ABaseGameCharacter::MoveForward(float Amount)
{
	if (Amount == 0.0f) return;
	AddMovementInput(GetActorForwardVector(), Amount);
}

void ABaseGameCharacter::MoveRight(float Amount)
{
	if (Amount == 0.0f) return;
	AddMovementInput(GetActorRightVector(), Amount);
}

void ABaseGameCharacter::SetIsCrawl()
{
	/* Было изначально так
	//IsCrawl = IsCrawl ? false : true;*/

	if (IsCrawl)
	{
		IsCrawl = false;
		SpeedType = EMovementSpeedType::Base;
	}
	else
	{
		if (IsSprint)
			IsSprint = false;
		if (IsWalk)
			IsWalk = false;
		IsCrawl = true;
		SpeedType = EMovementSpeedType::Crawl;
	}

	//SetStatus(IsCrawl, IsSprint, IsWalk, SpeedType, EMovementSpeedType::Crawl);
}

void ABaseGameCharacter::SetIsWalk()
{
	/* Было изначально так
	//IsWalk = IsWalk ? false : true;*/

	if (IsWalk)
	{
		IsWalk = false;
		SpeedType = EMovementSpeedType::Base;
	}
	else
	{
		if (IsSprint)
			IsSprint = false;
		if (IsCrawl)
			IsCrawl = false;
		IsWalk = true;
		SpeedType = EMovementSpeedType::Walk;
	}

	//SetStatus(IsWalk, IsSprint, IsCrawl, SpeedType, EMovementSpeedType::Walk);
}

void ABaseGameCharacter::SetIsSprint()
{
	/* Было изначально так
	//IsSprint = IsSprint ? false : true;*/

	if (IsSprint)
	{
		IsSprint = false;
		SpeedType = EMovementSpeedType::Base;
	}
	else
	{
		if (IsCrawl)
			IsCrawl = false;
		if (IsWalk)
			IsWalk = false;
		IsSprint = true;
		SpeedType = EMovementSpeedType::Sprint;
	}

	//SetStatus(IsSprint, IsCrawl, IsWalk, SpeedType, EMovementSpeedType::Sprint);
}

void ABaseGameCharacter::SetStatus(bool StatusToCheck, bool FirstStatusCheck, bool SecondStatusCheck, EMovementSpeedType SpeedTypeToSetup, EMovementSpeedType SpeedTypeToChange)
{
	if (StatusToCheck)
	{
		StatusToCheck = false;
		SpeedTypeToSetup = EMovementSpeedType::Base;
	}
	else
	{
		if (FirstStatusCheck)
			FirstStatusCheck = false;
		if (SecondStatusCheck)
			SecondStatusCheck = false;
		StatusToCheck = true;
		SpeedTypeToSetup = SpeedTypeToChange;
	}
}

void ABaseGameCharacter::SetAimed()
{
	IsArmed = IsArmed ? false : true;
}

void ABaseGameCharacter::OnDeath()
{
	WeaponComponent->StopFire();
	PlayAnimMontage(DeathAnimMontage);

	GetCharacterMovement()->DisableMovement();

	SetLifeSpan(LifeSpanDeath);
	
	if (Controller)
		Controller->ChangeState(NAME_Spectating);

	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void ABaseGameCharacter::OnHealthChanged(float Health)
{
	HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}

void ABaseGameCharacter::OnArmorChanged(float Armor)
{
	ArmorTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Armor)));
}

void ABaseGameCharacter::OnGroundLanded(const FHitResult& Hit)
{
	const auto FallVelocityZ = -GetVelocity().Z;
	UE_LOG(LogHealthComponent, Error, TEXT("FallVelocityZ: %f"), FallVelocityZ);

	if (FallVelocityZ < LandedDamageVelocity.X) return;

	const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);
	TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);
}