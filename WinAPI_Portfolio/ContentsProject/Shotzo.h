#pragma once
#include "Monster.h"

class AShotzo : public AMonster
{
public:
	// constrcuter destructer
	AShotzo();
	~AShotzo();

	// delete Function
	AShotzo(const AShotzo& _Other) = delete;
	AShotzo(AShotzo&& _Other) noexcept = delete;
	AShotzo& operator=(const AShotzo& _Other) = delete;
	AShotzo& operator=(AShotzo&& _Other) noexcept = delete;

	void Pause(float _DetlaTime) override;
	void Chase(float _DetlaTime) override;
	void AttackStart(float _DetlaTime) override;
	void Attack(float _DetlaTime) override;
	void AttackEnd(float _DetlaTime) override;
	
protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime);

private:
	void SetAnimation();
};

