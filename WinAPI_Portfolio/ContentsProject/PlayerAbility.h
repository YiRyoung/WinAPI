#pragma once
#include <EngineCore/Actor.h>

class APlayerAbility : public AActor
{
public:
	// constrcuter destructer
	APlayerAbility();
	~APlayerAbility();

	// delete Function
	APlayerAbility(const APlayerAbility& _Other) = delete;
	APlayerAbility(APlayerAbility&& _Other) noexcept = delete;
	APlayerAbility& operator=(const APlayerAbility& _Other) = delete;
	APlayerAbility& operator=(APlayerAbility&& _Other) noexcept = delete;

	void SetSprite(std::string _Ability);

protected:

private:
	class USpriteRenderer* AbilityRenderer;
	FVector2D TextScale;
};

