// My first Unreal Engine 5 Game project


#include "Animations/BaseAnimNotify.h"

void UBaseAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	OnNotified.Broadcast(MeshComp);
	Super::Notify(MeshComp, Animation);
}
