// My first Unreal Engine 5 Game project

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BaseCharacterMovementComponent.generated.h"

UENUM(BlueprintType)
enum class ESpeedModifier :uint8
{
	Crawl,
	Walk, 
	Base,
	Sprint
};

UCLASS()
class MYPROJECT_API UBaseCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
		float CrawlSpeedModifier = 0.25f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
		float WalkSpeedModifier = 0.5f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
		float SprintSpeedModifier = 1.5f;

	virtual float GetMaxSpeed() const override;
};
