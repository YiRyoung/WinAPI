#pragma once
#include "Monster.h"

class AKibble : public AMonster
{
public:
	// constrcuter destructer
	AKibble();
	~AKibble();

	// delete Function
	AKibble(const AKibble& _Other) = delete;
	AKibble(AKibble&& _Other) noexcept = delete;
	AKibble& operator=(const AKibble& _Other) = delete;
	AKibble& operator=(AKibble&& _Other) noexcept = delete;

	void Pause(float _DeltaTime) override;
	void Chase(float _DeltaTime) override;
	void AttackStart(float _DeltaTime) override;
	void Attack(float _DeltaTime) override;
	void AttackEnd(float _DeltaTime) override;
	void Die(float _DeltaTime) override;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime);

private:
	float CurTime = 0.0f;
	void SetAnimation();
	bool CheckDistance();	// 공격 범위 체크
	bool CheckDirect();		// 공격 방향 체크
	void LookPlayer();

	class ACutterSlider* NewSlider = nullptr;
};

