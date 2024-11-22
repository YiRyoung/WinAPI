#include "PreCompile.h"
#include "WaddleDoo.h"

#include <EngineCore/EngineAPICore.h>
#include <EngineCore/SpriteRenderer.h>

#include "WaddleBeam.h"

AWaddleDoo::AWaddleDoo()
{
	SetMonster("WaddleDoo_Left.png", { 50, 51 });
	SetCollision({ 54, 54 });
	SetAnimation();

	// 아직 미구현
	/*SetLeftSkillCollision(GetActorLocation() + FVector2D{ -100 , 0 }, FVector2D{ 145, 50 }, ECollisionType::Rect);
	SetRightSkillCollision(GetActorLocation() + FVector2D{ 100 , 0 }, FVector2D{ 145, 50 }, ECollisionType::Rect);*/
	DebugOn();
}

AWaddleDoo::~AWaddleDoo()
{
}

void AWaddleDoo::Pause(float _DeltaTime)
{
	if (abs(Pos.X) <= 200.0f && IsLook())
	{
		SetMonsterState(MonsterState::ATTACKSTART);
		return;
	}
	else
	{
		SetMonsterState(MonsterState::CHASE);
		return;
	}
}

void AWaddleDoo::Chase(float _DeltaTime)
{
	AMonster::Chase(_DeltaTime);

	ChangeMonsterAnim("Walk");

	if (abs(Pos.X) <= 200.0f && IsLook())
	{
		Time = 0.0f;
		SetMonsterState(MonsterState::ATTACKSTART);
		return;
	}
}

void AWaddleDoo::AttackStart(float _DeltaTime)
{
	ChangeMonsterAnim("Attack");
	FVector2D MonsterScale = SpriteRenderer->GetActorTransform().Scale;

	AWaddleBeam* NewWaddleBeam = GetWorld()->SpawnActor<AWaddleBeam>();

	//                              몬스터의 위치
	NewWaddleBeam->SetActorLocation(GetActorLocation() + FVector2D({ 0.0f, MonsterScale.Y * -1.0f }));
	FVector2D Vector = FVector2D::ZERO;

	if ("_Left" == GetAnimDir())
	{
		Vector = FVector2D::LEFT;
	}
	else if ("_Right" == GetAnimDir())
	{
		Vector = FVector2D::RIGHT;
	}
	
	NewWaddleBeam->SetDir(Vector);
	SetMonsterState(MonsterState::ATTACK);
	return;
}

void AWaddleDoo::Attack(float _DeltaTime)
{

	if (Time <= 2.0f)
	{
		//SkillCollisionSwitch(true);
	}
	else
	{
		//SkillCollisionSwitch(false);
		Time = 0.0f;

		SetMonsterState(MonsterState::ATTACKEND);
		return;
	}
}

void AWaddleDoo::AttackEnd(float _DeltaTime)
{
}

void AWaddleDoo::BeginPlay()
{
	AMonster::BeginPlay();
}

void AWaddleDoo::Tick(float _DeltaTime)
{
	AMonster::Tick(_DeltaTime);
	Time += _DeltaTime;
}


void AWaddleDoo::SetAnimation()
{
	// Walk
	SpriteRenderer->CreateAnimation("Walk_Left", "WaddleDoo_Left.png", 0, 1, 0.5f);
	SpriteRenderer->CreateAnimation("Walk_Right", "WaddleDoo_Right.png", 0, 1, 0.5f);

	// Attack
	SpriteRenderer->CreateAnimation("Attack_Left", "WaddleDoo_Left.png", 2, 3, 0.5f);
	SpriteRenderer->CreateAnimation("Attack_Right", "WaddleDoo_Right.png", 2, 3, 0.5f);

	// Destroy
	SpriteRenderer->CreateAnimation("Destroy", "Destory.png", 4, 6, 0.07f, false);

	// Start Animation
	SpriteRenderer->ChangeAnimation("Walk_Left");
}
