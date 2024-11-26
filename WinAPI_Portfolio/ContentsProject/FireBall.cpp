#include "PreCompile.h"
#include "FireBall.h"

#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/2DCollision.h>

#include "ContentsEnum.h"

AFireBall::AFireBall()
{
	FireballRender = CreateDefaultSubObject<USpriteRenderer>();
	FireballRender->SetSprite("HotHeadFire_Left.png");
	FireballRender->SetOrder(ERenderOrder::SKILL);
	FireballRender->SetComponentScale({ 60, 60 });

	FireballRender->CreateAnimation("Fireball_Left", "HotHeadFire_Left.png", 0, 1, 0.01f);
	FireballRender->CreateAnimation("Fireball_Right", "HotHeadFire_Right.png", 0, 1, 0.01f);
	FireballRender->ChangeAnimation("Fireball_Left");
	DebugOn();
}

AFireBall::~AFireBall()
{
}

void AFireBall::SetDir(const FVector2D& _Dir)
{
	Dir = _Dir;
}

void AFireBall::BeginPlay()
{
	Super::BeginPlay();
}

void AFireBall::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
}

