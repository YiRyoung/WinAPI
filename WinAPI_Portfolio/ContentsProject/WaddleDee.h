#pragma once
#include "Monster.h"

class AWaddleDee : public AMonster
{
public:
	// constrcuter destructer
	AWaddleDee();
	~AWaddleDee();

	// delete Function
	AWaddleDee(const AWaddleDee& _Other) = delete;
	AWaddleDee(AWaddleDee&& _Other) noexcept = delete;
	AWaddleDee& operator=(const AWaddleDee& _Other) = delete;
	AWaddleDee& operator=(AWaddleDee&& _Other) noexcept = delete;

	void Pause(float _DeltaTime);
	void Chase(float _DeltaTime);
	void Attack(float _DeltaTime);
	void Inhale(float _DeltaTime);
	void Died(float _DeltaTime);

protected:

private:
	void SetAnimation();
};

