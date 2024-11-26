#include "PreCompile.h"
#include "HotHead.h"

AHotHead::AHotHead()
{
	SetMonster("HotHead_Left.png", { 50, 51 }, EAbilityType::FIRE);
	//SetCollision({ 54, 54 });
	SetAnimation();
	DebugOn();
}

AHotHead::~AHotHead()
{
}

void AHotHead::BeginPlay()
{
	AMonster::BeginPlay();
}

void AHotHead::Tick(float _DeltaTime)
{
	AMonster::Tick(_DeltaTime);
}

void AHotHead::SetAnimation()
{
}

bool AHotHead::CheckDistance()
{
	return false;
}

bool AHotHead::CheckDirect()
{
	return false;
}

void AHotHead::LookPlayer()
{
}

void AHotHead::Chase(float _DeltaTime)
{
}

void AHotHead::AttackStart(float _DeltaTime)
{
}

void AHotHead::Attack(float _DeltaTime)
{
}

void AHotHead::AttackEnd(float _DeltaTime)
{
}
