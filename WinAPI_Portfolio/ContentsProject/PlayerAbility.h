#pragma once
#include "PlayerState.h"

class PlayerAbility : public PlayerState
{
public:
	// constrcuter destructer
	PlayerAbility();
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
	EStateType GetState()
	{
		return Player->GetState();
	}
	EAblityType GetAbility()
	{
		return Player->GetAbility();
	}

	// Animation
	void ChangeAnimation(std::string _Anim)
	{
		Player->ChangeAnimation(_Anim + Player->GetAnimDir());
	}

	void FireAttack(float _DeltaTime);
	void CutterAttack(float _DeltaTime);
	void SparkAttack(float _DeltaTime);
};

