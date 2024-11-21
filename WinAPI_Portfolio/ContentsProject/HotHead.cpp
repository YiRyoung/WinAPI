#include "PreCompile.h"
#include "HotHead.h"

#include <EngineCore/SpriteRenderer.h>

AHotHead::AHotHead()
{
	SetMonster("HotHead_Left.png", { 50, 51 });
	SetCollision({ 54, 54 });
	SetAnimation();
	//SetLeftSkillCollision()
	//SetRightSkillCollision()
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

	// Attack
	SpriteRenderer->CreateAnimation("AttackStart_Left", "HotHead_Left.png", 2, 3, 0.5f, false);
	SpriteRenderer->CreateAnimation("AttackStart_Right", "HotHead_Right.png", 2, 3, 0.5f, false);

	SpriteRenderer->CreateAnimation("Attack_Left", "HotHead_Left.png", 4, 5, 0.5f);
	SpriteRenderer->CreateAnimation("Attack_Right", "HotHead_Right.png", 4, 5, 0.5f);

	// Destroy
	SpriteRenderer->CreateAnimation("Destroy", "Destory.png", 4, 6, 0.07f, false);

	// Start Animation
	SpriteRenderer->ChangeAnimation("Walk_Left");
}

void AHotHead::Pause(float _DeltaTime)
{
	AMonster::Pause(_DeltaTime);
	float Length = abs(Pos.X);

	if (Length <= 100.0f)
	{
		SetMonsterState(MonsterState::ATTACK);
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


}

void AHotHead::AttackStart(float _DeltaTime)
{
	AMonster::Attack(_DeltaTime);
	Time = 0.0f;

	// 플레이어와의 거리가 100 이하일 경우 실행
	SpriteRenderer->ChangeAnimation("AttackStart");

	if (SpriteRenderer->IsCurAnimationEnd())
	{
		SpriteRenderer->ChangeAnimation("Attack");
		// 스킬 콜리전 On
		// 불렛 소환
	}

}

void AHotHead::Attack(float _DeltaTime)
{
	
}

void AHotHead::Died(float _DeltaTime)
{
	AMonster::Died(_DeltaTime);

	SpriteRenderer->ChangeAnimation("Destroy");

	if (SpriteRenderer->IsCurAnimationEnd())
	{
		Destroy();
	}
}
