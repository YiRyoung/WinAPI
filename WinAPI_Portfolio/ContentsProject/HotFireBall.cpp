#include "PreCompile.h"
#include "HotFireBall.h"

#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/2DCollision.h>

#include "ContentsEnum.h"

AHotFireBall::AHotFireBall()
{
	UpperSpriteRender = CreateDefaultSubObject<USpriteRenderer>();
	UpperSpriteRender->SetSprite("HotHead_Left.png");
	UpperSpriteRender->SetComponentScale({ 50, 51 });
	
	UnderSpriteRender = CreateDefaultSubObject<USpriteRenderer>();
	UnderSpriteRender->SetSprite("HotHead_Right.png");
	UnderSpriteRender->SetComponentScale({ 50, 51 });

	UpperSpriteRender->CreateAnimation("FireBall_Left", "HotHead_Left.png", 6, 7, 0.05f);
	UpperSpriteRender->CreateAnimation("FireBall_Right", "HotHead_Right.png", 6, 7, 0.05f);

	UnderSpriteRender->CreateAnimation("FireBall_Left", "HotHead_Left.png", 6, 7, 0.05f);
	UnderSpriteRender->CreateAnimation("FireBall_Right", "HotHead_Right.png", 6, 7, 0.05f);
}

AHotFireBall::~AHotFireBall()
{
}

void AHotFireBall::BeginPlay()
{
	Super::BeginPlay();
}

void AHotFireBall::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
}

