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
	void AttackStart(float _DeltaTime);
	void Attack(float _DeltaTime) override;
	void Died(float _DeltaTime) override;

protected:

private:
	void SetAnimation();
};