#include "PreCompile.h"
#include "WaddleDoo.h"

#include <EngineCore/SpriteRenderer.h>

AWaddleDoo::AWaddleDoo()
{
	SetMonster("WaddleDoo_Left.png", { 50, 50 });
	SetCollision({ 54, 54 });
	SetAnimation();
	DebugOn();
}

AWaddleDoo::~AWaddleDoo()
{
}

void AWaddleDoo::Pause(float _DeltaTime)
{
}

void AWaddleDoo::Chase(float _DeltaTime)
{
}

void AWaddleDoo::AttackStart(float _DeltaTime)
{
}

void AWaddleDoo::Attack(float _DeltaTime)
{
}

void AWaddleDoo::AttackEnd(float _DeltaTime)
{
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
