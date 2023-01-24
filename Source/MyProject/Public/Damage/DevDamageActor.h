// My first Unreal Engine 5 Game project

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DevDamageActor.generated.h"

UCLASS()
class MYPROJECT_API ADevDamageActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ADevDamageActor();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool DoFullDamage = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Damage = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Radius = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FColor SphereColor = FColor::Red;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
};
