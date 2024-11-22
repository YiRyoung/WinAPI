#pragma once
#include "Monster.h"

class AWaddleDoo : public AMonster
{
public:
	// constrcuter destructer
	AWaddleDoo();
	~AWaddleDoo();

	// delete Function
	AWaddleDoo(const AWaddleDoo& _Other) = delete;
	AWaddleDoo(AWaddleDoo&& _Other) noexcept = delete;
	AWaddleDoo& operator=(const AWaddleDoo& _Other) = delete;
	AWaddleDoo& operator=(AWaddleDoo&& _Other) noexcept = delete;

	void Pause(float _DeltaTime) override;
	void Chase(float _DeltaTime) override;
	void AttackStart(float _DeltaTime) override;
	void Attack(float _DeltaTime) override;
	void AttackEnd(float _DeltaTime) override;

	FVector2D GetPos()
	{
		return GetActorLocation();
	}

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

