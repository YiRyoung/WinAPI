#include "PreCompile.h"
#include "WaddleDee.h"

#include <EngineCore/SpriteRenderer.h>

AWaddleDee::AWaddleDee()
{
	SetMonster("WaddleDee_Left.png", { 50, 50 }, EAbilityType::NORMAL);
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
	SpriteRenderer->CreateAnimation("Walk_Left", "WaddleDee_Left.png", 0, 1, 0.5f);
	SpriteRenderer->CreateAnimation("Walk_Right", "WaddleDee_Right.png", 0, 1, 0.5f);

	// Destory
	SpriteRenderer->CreateAnimation("Destroy", "Destory.png", 4, 6, 0.07f, false);

	// Start Animation
	SpriteRenderer->ChangeAnimation("Walk_Left");
}


void AWaddleDee::Chase(float _DeltaTime)
{
	AMonster::Chase(_DeltaTime);

	ChangeMonsterAnim("Walk");
}
