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

	void Chase(float _DeltaTime) override;

protected:

private:
	void SetAnimation();
};

