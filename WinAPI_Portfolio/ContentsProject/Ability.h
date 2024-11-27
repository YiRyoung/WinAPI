#pragma once
#include <EngineCore/Actor.h>

class AAbility : public AActor
{
public:
	// constrcuter destructer
	AAbility();
	~AAbility();

	// delete Function
	AAbility(const AAbility& _Other) = delete;
	AAbility(AAbility&& _Other) noexcept = delete;
	AAbility& operator=(const AAbility& _Other) = delete;
	AAbility& operator=(AAbility&& _Other) noexcept = delete;

	void SetSprite(std::string _Ability, int _CurAbilityTpye);

protected:

private:
	class USpriteRenderer* AbilityRenderer;
	FVector2D SpriteScale;
};

