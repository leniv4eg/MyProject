// My first Unreal Engine 5 Game project

#include "Damage/DevDamageActor.h"
#include <DrawDebugHelpers.h>
#include <Kismet/GameplayStatics.h>

ADevDamageActor::ADevDamageActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(SceneComponent);
}

void ADevDamageActor::BeginPlay()
{
	Super::BeginPlay();
}

void ADevDamageActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 36, SphereColor);

	UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), Radius, DamageType, {}, this, nullptr, DoFullDamage);
}

