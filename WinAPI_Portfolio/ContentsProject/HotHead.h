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
	void Die(float _DeltaTime) override;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	float CurTime = 0.0f;
	void SetAnimation();
	bool CheckDistance();	// ���� ���� üũ
	bool CheckDirect();		// ���� ���� üũ

	class AFireBall* NewFireBall = nullptr;
};

