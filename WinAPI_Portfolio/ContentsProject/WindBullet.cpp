#include "PreCompile.h"
#include "WindBullet.h"

WindBullet::WindBullet()
{
	SpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
	SpriteRenderer->SetSprite("Kirby_Normal_Right.png");
	SpriteRenderer->SetComponentScale({ 94, 94 });
}

WindBullet::~WindBullet()
{
}

void WindBullet::BeginPlay()
{
}

void WindBullet::Tick(float _DeltaTime)
{
}

