#pragma once
#include <EngineCore/Actor.h>

#include <EnginePlatform/EngineSound.h>

#include "SoundManager.h"
#include "Apple.h"
#include "ContentsEnum.h"

enum class ESavePos
{
	Pos1,
	Pos2,
	Pos3,
	Pos4,
	Pos5
};

enum class EAttackPhase
{
	Phase1,
	Phase2,
	Phase3,
	Phase4,
	Phase5
};

enum class EBossState
{
	PAUSE,
	CHASE,
	ATTACK,
	HURT,
	DIE,
	END
};

class AWisphyWood : public AActor
{
public:
	// constrcuter destructer
	AWisphyWood();
	~AWisphyWood();

	// delete Function
	AWisphyWood(const AWisphyWood& _Other) = delete;
	AWisphyWood(AWisphyWood&& _Other) noexcept = delete;
	AWisphyWood& operator=(const AWisphyWood& _Other) = delete;
	AWisphyWood& operator=(AWisphyWood&& _Other) noexcept = delete;

	void SetColImage(std::string _ColImageName);
	void SetPlayerLocation(FVector2D _KirbyPos);

	void Pause(float _DeltaTime);
	void Chase(float _DeltaTime);
	void Attack(float _DeltaTime);
	void HurtStart(float _DeltaTime);
	void Hurt(float _DeltaTime);
	void Died(float _DeltaTime);
	void NextLevel();

	void CollisionEnter(AActor* _ColActor);

protected:
	void Tick(float _DeltaTime) override;
private:
	int BossHP = 5;
	float CurTime = 0.0f;
	void SetAnimation();

	ESavePos CurSavePos = ESavePos::Pos1;
	void Pos1();
	void Pos2();
	void Pos3();
	void Pos4();
	void Pos5();
	void SavePlayerPos();

	EAttackPhase CurPhase = EAttackPhase::Phase1;
	void Phase1();
	void Phase2();
	void Phase3();
	void Phase4();
	void Phase5();
	void AttackApple();

	EBossState CurState = EBossState::PAUSE;
	void MonsterFSM(float _DeltaTime);

	FVector2D PlayerLocation = FVector2D::ZERO;
	FVector2D PlayerPos[4] = { FVector2D::ZERO, };

	class UEngineWinImage* ColImage = nullptr;
	class USpriteRenderer* SpriteRenderer = nullptr;
	class U2DCollision* CollisionComponent = nullptr;

	class AApple* NewApple = nullptr;
	class AFade* NewFade = nullptr;
	USoundPlayer BGMPlayer;
};

