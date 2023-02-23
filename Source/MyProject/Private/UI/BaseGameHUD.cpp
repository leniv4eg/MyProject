// My first Unreal Engine 5 Game project


#include "UI/BaseGameHUD.h"
#include <Engine/Canvas.h>
#include <Blueprint/UserWidget.h>

void ABaseGameHUD::DrawHUD()
{
	Super::DrawHUD();
	//DrawCrossHair();
}

void ABaseGameHUD::BeginPlay()
{
	Super::BeginPlay();
	auto PlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass);
	if (PlayerHUDWidget)
	{
		PlayerHUDWidget->AddToViewport();
	}
}
