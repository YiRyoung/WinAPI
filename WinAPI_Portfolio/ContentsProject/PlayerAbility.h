#pragma once
#include "Player.h"

class PlayerAbility
{
public:
	// constrcuter destructer
	PlayerAbility();
	PlayerAbility(APlayer* _Player);
	~PlayerAbility();

	// delete Function
	PlayerAbility(const PlayerAbility& _Other) = delete;
	PlayerAbility(PlayerAbility&& _Other) noexcept = delete;
	PlayerAbility& operator=(const PlayerAbility& _Other) = delete;
	PlayerAbility& operator=(PlayerAbility&& _Other) noexcept = delete;

	void Attack(float _DeltaTime);

protected:

private:
	APlayer* Player = nullptr;

	// Animation
	std::string GetAnimDir()
	{
		return Player->GetAnimDir();
	}
	void ChangeAnimation(std::string _Anim)
	{
		Player->ChangeAnimation(_Anim + Player->GetAnimDir());
	}

	// Collision
	void SetInhaleLeft(bool _OnOff)
	{
		Player->SetInhaleLeft(_OnOff);
	}
	void SetInhaleRight(bool _OnOff)
	{
		Player->SetInhaleRight(_OnOff);
	}

	// Ability
	EAblityType GetAbility()
	{
		return Player->GetAbility();
	}

	void NormalAttack(float _DeltaTime);
	void FireAttack(float _DeltaTime);
	void CutterAttack(float _DeltaTime);
	void SparkAttack(float _DeltaTime);
};

