#include "PreCompile.h"
#include "WaddleDee.h"

#include <EngineCore/SpriteRenderer.h>

AWaddleDee::AWaddleDee()
{
	SetMonster("Waddle Dee_Left.png", { 50, 50 });
	SetCollision({ 54, 54 });
	SetAnimation();
	DebugOn();
}

AWaddleDee::~AWaddleDee()
{
}

void AWaddleDee::SetAnimation()
{
	// Walk
	SpriteRenderer->CreateAnimation("Walk_Left", "Waddle Dee_Left.png", 0, 1, 0.5f);
	SpriteRenderer->CreateAnimation("Walk_Right", "Waddle Dee_Right.png", 0, 1, 0.5f);

	// Destory
	SpriteRenderer->CreateAnimation("Destroy", "Destory.png", 4, 6, 0.07f, false);

	// Start Animation
	SpriteRenderer->ChangeAnimation("Walk_Left");
}

void AWaddleDee::Pause(float _DeltaTime)
{
	AMonster::Pause(_DeltaTime);

	SetMonsterState(MonsterState::CHASE);
	return;
}

void AWaddleDee::Chase(float _DeltaTime)
{
	AMonster::Chase(_DeltaTime);

	ChangeMonsterAnim("Walk");
}

void AWaddleDee::Died(float _DeltaTime)
{
	AMonster::Died(_DeltaTime);

	SpriteRenderer->ChangeAnimation("Destroy");

	if (SpriteRenderer->IsCurAnimationEnd())
	{
		Destroy();
	}
}
