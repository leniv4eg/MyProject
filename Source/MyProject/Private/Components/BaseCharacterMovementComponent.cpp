// My first Unreal Engine 5 Game project


#include "Components/BaseCharacterMovementComponent.h"
#include "Player/BaseGameCharacter.h"

float UBaseCharacterMovementComponent::GetMaxSpeed() const
{
	const float MaxSpeed = Super::GetMaxSpeed();
	const ABaseGameCharacter* Player = Cast<ABaseGameCharacter>(GetPawnOwner());

	if (Player && Player->IsSprinting())
		return MaxSpeed * SprintSpeedModifier;
	else if (Player && Player->IsCrawling())
		return MaxSpeed * CrawlSpeedModifier;
	else if (Player && Player->IsWalking())
		return MaxSpeed * WalkSpeedModifier;
	else 
		return MaxSpeed;
	}