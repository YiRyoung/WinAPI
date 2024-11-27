#include "PreCompile.h"
#include "WaddleDoo.h"

#include <EngineCore/SpriteRenderer.h>

#include "WaddleBeam.h"

AWaddleDoo::AWaddleDoo()
{
	SetMonster("WaddleDoo_Left.png", { 50, 50 }, EAbilityType::BEAM);
	SetCollision({ 54, 54 });
	SetAnimation();
	DebugOn();
}

AWaddleDoo::~AWaddleDoo()
{
}

void AWaddleDoo::Pause(float _DeltaTime)
{
	SpriteRenderer->SetSprite("WaddleDoo" + AnimDir + ".png", 1);
	if (CurTime >= 3.0f)
	{
		CurTime = 0.0f;
		SetMonsterState(EMonsterState::CHASE);
		return;
	}
}

void AWaddleDoo::Chase(float _DeltaTime)
{
	AMonster::Chase(_DeltaTime);

	ChangeMonsterAnim("Walk");

	if (CheckDistance() && CheckDirect())
	{
		CurTime = 0.0f;
		SetMonsterState(EMonsterState::ATTACKSTART);
		return;
	}
}

void AWaddleDoo::AttackStart(float _DeltaTime)
{
	ChangeMonsterAnim("Attack");

	if (CurTime >= 1.0f)
	{
		CurTime = 0.0f;
		SetMonsterState(EMonsterState::ATTACK);
		return;
	}
}

void AWaddleDoo::Attack(float _DeltaTime)
{
	NewWaddleBeam = GetWorld()->SpawnActor<AWaddleBeam>();
	NewWaddleBeam->SetMosnterSkillCollision();
	NewWaddleBeam->SetActorLocation(GetActorLocation());
	FVector2D Vector = ("_Left" == AnimDir) ? FVector2D::LEFT : FVector2D::RIGHT;
	NewWaddleBeam->SetDir(Vector);

	SetMonsterState(EMonsterState::ATTACKEND);
	return;
}

void AWaddleDoo::AttackEnd(float _DeltaTime)
{
	if (CurTime >= 1.2f)
	{
		SetMonsterState(EMonsterState::PAUSE);
		return;
	}
}

void AWaddleDoo::Die(float _DeltaTime)
{
	AMonster::Die(_DeltaTime);

	if (NewWaddleBeam != nullptr)
	{
		NewWaddleBeam->Destroy();
	}
}

void AWaddleDoo::Tick(float _DeltaTime)
{
	AMonster::Tick(_DeltaTime);
	CurTime += _DeltaTime;
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

bool AWaddleDoo::CheckDistance()
{
	float Dis = AMonster::PlayerDistance;
	bool IsTrue = (abs(Dis) <= 200.0f) ? true : false;
	return IsTrue;
}

bool AWaddleDoo::CheckDirect()
{
	float Dis = AMonster::PlayerDistance;
	
	if (("_Left" == AnimDir && Dis >= 0) || ("_Right" == AnimDir && Dis < 0))
	{
		return true;
	}
	return false;
}
