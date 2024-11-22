#include "PreCompile.h"
#include "HotHead.h"

#include <EngineCore/EngineAPICore.h>
#include <EngineCore/SpriteRenderer.h>

AHotHead::AHotHead()
{
	SetMonster("HotHead_Left.png", { 50, 51 });
	SetCollision({ 54, 54 });
	SetAnimation();

	SetLeftSkillCollision(GetActorLocation() + FVector2D{-100 , 0}, FVector2D{ 145, 50 }, ECollisionType::Rect);
	SetRightSkillCollision(GetActorLocation() + FVector2D{ 100 , 0 }, FVector2D{ 145, 50 }, ECollisionType::Rect);
	DebugOn();
}

AHotHead::~AHotHead()
{
}

void AHotHead::SetAnimation()
{
	// Walk
	SpriteRenderer->CreateAnimation("Walk_Left", "HotHead_Left.png", 0, 1, 0.5f);
	SpriteRenderer->CreateAnimation("Walk_Right", "HotHead_Right.png", 0, 1, 0.5f);

	// AttackStart
	SpriteRenderer->CreateAnimation("AttackStart_Left", "HotHead_Left.png", 2, 3, 0.4f, false);
	SpriteRenderer->CreateAnimation("AttackStart_Right", "HotHead_Right.png", 2, 3, 0.4f, false);

	// Attack
	SpriteRenderer->CreateAnimation("Attack_Left", "HotHead_Left.png", 4, 5, 0.05f);
	SpriteRenderer->CreateAnimation("Attack_Right", "HotHead_Right.png", 4, 5, 0.05f);

	// AttackEnd
	SpriteRenderer->CreateAnimation("AttackEnd_Left", "HotHead_Left.png", 4, 4, 1.0f, false);
	SpriteRenderer->CreateAnimation("AttackEnd_Right", "HotHead_Right.png", 4, 4, 1.0f, false);

	// Destroy
	SpriteRenderer->CreateAnimation("Destroy", "Destory.png", 4, 6, 0.07f, false);

	// Start Animation
	SpriteRenderer->ChangeAnimation("Walk_Left");
}

void AHotHead::Pause(float _DeltaTime)
{;

	if (abs(Pos.X) <= 200.0f && IsLook())
	{
		// 딜레이 2초 넣어주기
		SetMonsterState(MonsterState::ATTACKSTART);
		return;
	}
	else
	{
		SetMonsterState(MonsterState::CHASE);
		return;
	}
}

void AHotHead::Chase(float _DeltaTime)
{
	AMonster::Chase(_DeltaTime);

	ChangeMonsterAnim("Walk");

	if (abs(Pos.X) <= 200.0f && IsLook())
	{
		SetMonsterState(MonsterState::ATTACKSTART);
		return;
	}
}

void AHotHead::AttackStart(float _DeltaTime)
{
	ChangeMonsterAnim("AttackStart");
	Time = 0.0f;

	if (MonsterAnimFinish())
	{
		SetMonsterState(MonsterState::ATTACK);
		return;
	}
}

void AHotHead::Attack(float _DeltaTime)
{
	ChangeMonsterAnim("Attack");

	if (Time <= 2.0f)
	{
		SkillCollisionSwitch(true);
	}
	else
	{
		SkillCollisionSwitch(false);
		Time = 0.0f;

		SetMonsterState(MonsterState::ATTACKEND);
		return;
	}
}

void AHotHead::AttackEnd(float _DeltaTime)
{
	ChangeMonsterAnim("AttackEnd");

	if (Time >= 0.7f)
	{
		SetMonsterState(MonsterState::CHASE);
		return;
	}
}

void AHotHead::BeginPlay()
{
	AMonster::BeginPlay();
}

void AHotHead::Tick(float _DeltaTime)
{
	AMonster::Tick(_DeltaTime);
	Time += _DeltaTime;


}
