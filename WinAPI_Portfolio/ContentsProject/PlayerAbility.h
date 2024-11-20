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
	void ChangeAnimation(std::string _Anim)
	{
		Player->ChangeAnimation(_Anim + Player->GetAnimDir());
	}

	// Collision
	void SetInhaleCollision(bool _OnOff)
	{
		if ("_Right" == Player->GetAnimDir())
		{
			Player->SetInhaleRight(_OnOff);
		}
		else
		{
			Player->SetInhaleLeft(_OnOff);
		}
	}

	// Ability
	EAblityType GetAbility()
	{
		return Player->GetAbility();
	}

	void NormalAttackStart(float _DeltaTime);
	void NormalAttackEnd(float _DeltaTime);
	void FireAttack(float _DeltaTime);
	void CutterAttack(float _DeltaTime);
	void SparkAttack(float _DeltaTime);
};

