#include "PreCompile.h"
#include "HotHead.h"

#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/2DCollision.h>

#include "FireBall.h"

AHotHead::AHotHead()
{
	SetMonster("HotHead_Left.png", { 50, 51 }, EAbilityType::FIRE);
	SetCollision({ 54, 54 });
	SetAnimation();
	DebugOn();
}

AHotHead::~AHotHead()
{
}

void AHotHead::BeginPlay()
{
	AMonster::BeginPlay();
	SetMonsterState(EMonsterState::CHASE);
	return;
}

void AHotHead::Tick(float _DeltaTime)
{
	AMonster::Tick(_DeltaTime);
	CurTime += _DeltaTime;

	if (EMonsterState::INHALE == GetMonsterState() && nullptr != NewFireBall)
	{
		NewFireBall->Destroy();
	}
}

void AHotHead::SetAnimation()
{
	// Walk
	SpriteRenderer->CreateAnimation("Walk_Left", "HotHead_Left.png", 0, 1, 0.5f);
	SpriteRenderer->CreateAnimation("Walk_Right", "HotHead_Right.png", 0, 1, 0.5f);

	// Attack
	SpriteRenderer->CreateAnimation("AttackStart_Left", "HotHead_Left.png", 2, 3, 0.1f, false);
	SpriteRenderer->CreateAnimation("AttackStart_Right", "HotHead_Right.png", 2, 3, 0.1f, false);
	SpriteRenderer->CreateAnimation("Attack_Left", "HotHead_Left.png", 4, 5, 0.08f);
	SpriteRenderer->CreateAnimation("Attack_Right", "HotHead_Right.png", 4, 5, 0.08f);
	SpriteRenderer->CreateAnimation("AttackEnd_Left", "HotHead_Left.png", { 3, 2 }, 0.07f);
	SpriteRenderer->CreateAnimation("AttackEnd_Right", "HotHead_Right.png", { 3, 2 }, 0.07f);

	// Destroy
	SpriteRenderer->CreateAnimation("Destroy", "Destory.png", 4, 6, 0.07f, false);

	// Start Animation
	SpriteRenderer->ChangeAnimation("Walk_Left");
}

bool AHotHead::CheckDistance()
{
	float Dis = AMonster::PlayerDistance;
	bool IsTrue = (abs(Dis) <= 200.0f) ? true : false;
	return IsTrue;
}

bool AHotHead::CheckDirect()
{
	float Dis = AMonster::PlayerDistance;

	if (("_Left" == AnimDir && Dis >= 0) || ("_Right" == AnimDir && Dis < 0))
	{
		return true;
	}
	return false;
}

void AHotHead::Pause(float _DeltaTime)
{
	SpriteRenderer->SetSprite("HotHead" + AnimDir + ".png", 1);
	if (CurTime >= 3.0f)
	{
		CurTime = 0.0f;
		SetMonsterState(EMonsterState::CHASE);
		return;
	}
}

void AHotHead::Chase(float _DeltaTime)
{
	AMonster::Chase(_DeltaTime);

	ChangeMonsterAnim("Walk");

	if (CheckDistance() && CheckDirect())
	{
		SetMonsterState(EMonsterState::ATTACKSTART);
		return;
	}
}

void AHotHead::AttackStart(float _DeltaTime)
{
	ChangeMonsterAnim("AttackStart");

	if (SpriteRenderer->IsCurAnimationEnd())
	{
		ChangeMonsterAnim("Attack");
		SetMonsterState(EMonsterState::ATTACK);
		return;
	}
}

void AHotHead::Attack(float _DeltaTime)
{
	NewFireBall = GetWorld()->SpawnActor<AFireBall>();
	FVector2D Vector = ("_Left" == AnimDir) ? FVector2D::LEFT : FVector2D::RIGHT;
	NewFireBall->SetActorLocation(GetActorLocation() + (Vector * 80.0f) + FVector2D({ 0.0f, -10.0f }));
	NewFireBall->SetMosnterSkillCollision();
	NewFireBall->SetDir(Vector);
	NewFireBall->ChangeAnimation("FireBall");

	CurTime = 0.0f;
	SetMonsterState(EMonsterState::ATTACKEND);
	return;
}

void AHotHead::AttackEnd(float _DeltaTime)
{
	if (CurTime >= 3.8f)
	{
		ChangeMonsterAnim("AttackEnd");
		if (SpriteRenderer->IsCurAnimationEnd())
		{
			CurTime = 0.0f;
			SetMonsterState(EMonsterState::PAUSE);
			return;
		}
	}
}

void AHotHead::Die(float _DeltaTime)
{
	AMonster::Die(_DeltaTime);
	NewFireBall->SetActive(false);
}
