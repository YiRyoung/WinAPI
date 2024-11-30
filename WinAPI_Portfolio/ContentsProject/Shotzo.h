#pragma once
#include "Monster.h"

#include <EnginePlatform/EngineSound.h>

#include "SoundManager.h"

enum class EShotDirect
{
	LEFT,
	LEFTUPPER,
	UP,
	RIGHTUPPER,
	RIGHT
};

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

	void Pause(float _DeltaTime) override;
	void Chase(float _DetlaTime) override;
	void AttackStart(float _DetlaTime) override;
	void Attack(float _DetlaTime) override;
	
	void CollisionEnter(AActor* _ColActor);

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime);

private:
	bool IsAttack = true;
	float CurTime = 0.0f;
	float PauseTime = 0.0f;
	FVector2D TempLocation = FVector2D::ZERO;	// 플레이어의 위치 임시저장
	EShotDirect ShotDir = EShotDirect::LEFT;

	bool CheckDistance();	// 공격 범위 체크
	void CheckDirect();		// 공격 방향 체크

	USoundPlayer BGMPlayer;
	class AShotBullet* NewBullet = nullptr;
};

