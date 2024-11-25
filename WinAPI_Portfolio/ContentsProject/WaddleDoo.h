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
	void AttackStart(float _DeltaTime);
	void Attack(float _DeltaTime);

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	float CurTime = 0.0f;
	void SetAnimation();
	bool CheckDistance();	// 공격 범위 체크
	bool CheckDirect();		// 공격 방향 체크
};

