#pragma once
#include "Monster.h"

class AHotHead : public AMonster
{
public:
	// constrcuter destructer
	AHotHead();
	~AHotHead();

	// delete Function
	AHotHead(const AHotHead& _Other) = delete;
	AHotHead(AHotHead&& _Other) noexcept = delete;
	AHotHead& operator=(const AHotHead& _Other) = delete;
	AHotHead& operator=(AHotHead&& _Other) noexcept = delete;

	void Pause(float _DeltaTime) override;
	void Chase(float _DeltaTime) override;
	void AttackStart(float _DeltaTime) override;
	void Attack(float _DeltaTime) override;
	void AttackEnd(float _DeltaTime) override;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	float Time = 0.0f;

	void SetAnimation();
	void SkillCollisionSwitch(bool _IsOn)
	{
		if ("_Left" == GetAnimDir())
		{
			SetLeftSkillSwitch(_IsOn);
		}
		else
		{
			SetRightSkillSwitch(_IsOn);
		}
	}

	bool IsLook()
	{
		if ("_Left" == GetAnimDir() && Pos.X >= 0)
		{
			return true;
		}
		else if ("_Right" == GetAnimDir() && Pos.X < 0)
		{
			return true;
		}

		return false;
	}
};