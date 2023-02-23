#pragma once

class GameUtils 
{
public:
	template <typename T>
	static T* GetPlayerComponentTemplate(APawn* PlayerPawn)
	{
		if (!PlayerPawn) return nullptr;

		const auto Component = PlayerPawn->GetComponentByClass(T::StaticClass());

		return Cast<T>(Component);
	}
};