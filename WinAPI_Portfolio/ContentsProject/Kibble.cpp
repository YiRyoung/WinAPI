#include "PreCompile.h"
#include "Kibble.h"

#include <EngineCore/SpriteRenderer.h>

#include "CutterSlider.h"

AKibble::AKibble()
{
	SetMonster("Kibble_Right.png", { 50, 51 }, EAbilityType::CUTTER);
	SetCollision({ 54, 54 });
	SetAnimation();
	DebugOn();
}

AKibble::~AKibble()
{
}

void AKibble::BeginPlay()
{
	AMonster::BeginPlay();
}

void AKibble::Tick(float _DeltaTime)
{
	AMonster::Tick(_DeltaTime);
	CurTime += _DeltaTime;
}

void AKibble::SetAnimation()
{
	// Idle
	SpriteRenderer->CreateAnimation("Idle_Left", "Kibble_Left.png", 0, 1, 0.5f);
	SpriteRenderer->CreateAnimation("Idle_Right", "Kibble_Right.png", {3, 2}, 0.5f);

	// Attack
	SpriteRenderer->CreateAnimation("Attack_Left", "Kibble_Left.png", 2, 3, 0.5f);
	SpriteRenderer->CreateAnimation("Attack_Right", "Kibble_Right.png", {1, 0}, 0.5f);

	// Destroy
	SpriteRenderer->CreateAnimation("Destroy", "Destory.png", 4, 6, 0.07f, false);

	// Start Animation
	SpriteRenderer->ChangeAnimation("Idle_Left");
}

bool AKibble::CheckDistance()
{
	float Dis = AMonster::PlayerDistance;
	bool IsTrue = (abs(Dis) <= 200.0f) ? true : false;
	return IsTrue;
}

bool AKibble::CheckDirect()
{
	float Dis = AMonster::PlayerDistance;

	if (("_Left" == AnimDir && Dis >= 0) || ("_Right" == AnimDir && Dis < 0))
	{
		return true;
	}
	return false;
}

void AKibble::LookPlayer()
{
	if (AMonster::PlayerDistance >= 0)
	{
		AnimDir = "_Left";
	}
	else
	{
		AnimDir = "_Right";
	}
}

void AKibble::Pause(float _DeltaTime)
{
	Gravity(_DeltaTime);
	ChangeMonsterAnim("Idle");

	if (CurTime >= 5.0f)
	{
		CurTime = 0.0f;
		LookPlayer();
		SetMonsterState(EMonsterState::ATTACKSTART);
		return;
	}
}

void AKibble::Chase(float _DeltaTime)
{
}

void AKibble::AttackStart(float _DeltaTime)
{
	ChangeMonsterAnim("Attack");

	if (CurTime >= 1.0f)
	{
		CurTime = 0.0f;
		ChangeMonsterAnim("Idle");
		SetMonsterState(EMonsterState::ATTACK);
		return;
	}
}

void AKibble::Attack(float _DeltaTime)
{
	NewSlider = GetWorld()->SpawnActor<ACutterSlider>();
	FVector2D Vector = ("_Left" == AnimDir) ? FVector2D::LEFT : FVector2D::RIGHT;
	NewSlider->SetActorLocation(GetActorLocation() + (Vector * 10.0f) + FVector2D({0.0f, -10.0f}));
	NewSlider->SetMosnterSkillCollision();
	NewSlider->SetDir(Vector);

	CurTime = 0.0f;
	SetMonsterState(EMonsterState::ATTACKEND);
	return;
}

void AKibble::AttackEnd(float _DeltaTime)
{
	if (CurTime >= 2.5f)
	{
		CurTime = 0.0f;
		SetMonsterState(EMonsterState::PAUSE);
		return;
	}
}
