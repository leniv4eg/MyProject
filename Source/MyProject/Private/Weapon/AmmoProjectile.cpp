// My first Unreal Engine 5 Game project

#include "Weapon/AmmoProjectile.h"
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include <DrawDebugHelpers.h>
#include <Kismet/GameplayStatics.h>

AAmmoProjectile::AAmmoProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	CollisionComponent->InitSphereRadius(5.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	SetRootComponent(CollisionComponent);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	MovementComponent->InitialSpeed = 2000.0f;
	MovementComponent->ProjectileGravityScale = 0.0f;
}

void AAmmoProjectile::SetProjectileParams(const float AmmoAmount, const float AmmoSpeed, const float AmmoRadius, const bool AmmoFullDamage, const bool Launcher)
{
	ProjectileDamage = AmmoAmount;
	MovementComponent->InitialSpeed = AmmoSpeed;
	ProjectileRadius = AmmoRadius;
	ProjectileDoFullDamage = AmmoFullDamage;
	ProjectileLauncher = Launcher;
}

void AAmmoProjectile::BeginPlay()
{
	Super::BeginPlay();

	check(MovementComponent);
	check(CollisionComponent);
	MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;
	CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
	CollisionComponent->OnComponentHit.AddDynamic(this, &AAmmoProjectile::OnProjectileHit);
	SetLifeSpan(LifeSeconds);
}

void AAmmoProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult)
{
	if (!GetWorld()) return;
	MovementComponent->StopMovementImmediately();

	if (ProjectileLauncher)
	{
		UGameplayStatics::ApplyRadialDamage(GetWorld(), //
			ProjectileDamage, //
			GetActorLocation(), //
			ProjectileRadius, //
			UDamageType::StaticClass(), //
			{ GetOwner() }, //
			this, //
			GetController(), //
			ProjectileDoFullDamage);

		DrawDebugSphere(GetWorld(), GetActorLocation(), ProjectileRadius, 24, FColor::Red, false, 5.0f);
	}
	else
	{
		const auto DamagedActor = HitResult.GetActor();
		if (!DamagedActor) return;

		DamagedActor->TakeDamage(ProjectileDamage, FDamageEvent{}, GetController(), this);
	}

	Destroy();
}

AController* AAmmoProjectile::GetController() const
{
	const auto Pawn = Cast<APawn>(GetOwner());
	return Pawn ? Pawn->GetController() : nullptr;
}
