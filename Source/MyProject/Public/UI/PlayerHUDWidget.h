// My first Unreal Engine 5 Game project

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <BaseCoreTypes.h>
#include "PlayerHUDWidget.generated.h"

UCLASS()
class MYPROJECT_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "UI")
		float GetHealthPercent() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
		float GetArmorValue() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
		bool GetCurrentWeaponUIData(FWeaponUIData& UIData) const;

	UFUNCTION(BlueprintCallable, Category = "UI")
		bool GetCurrentWeaponAmmo(FAmmoData& AmmoData) const;

	UFUNCTION(BlueprintCallable, Category = "UI")
		bool IsPlayerAlive() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
		bool IsPlayerSpectating() const;
};
