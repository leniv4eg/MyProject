// My first Unreal Engine 5 Game project

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BaseGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API ABaseGameHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	virtual void DrawHUD() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		TSubclassOf<UUserWidget> PlayerHUDWidgetClass;

	virtual void BeginPlay() override;

};
