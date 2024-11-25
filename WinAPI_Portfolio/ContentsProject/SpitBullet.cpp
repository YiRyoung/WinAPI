#include "PreCompile.h"
#include "SpitBullet.h"

#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/2DCollision.h>

#include "ContentsEnum.h"

ASpitBullet::ASpitBullet()
{
	SpitBulletRender = CreateDefaultSubObject<USpriteRenderer>();
	SpitBulletRender->SetSprite("Kirby_Normal_Right.png", 38);
	SpitBulletRender->SetComponentScale({ 100, 100 });
	SpitBulletRender->SetOrder(ERenderOrder::SKILL);
	SpitBulletRender->CreateAnimation("Spit", "Kirby_Normal_Right.png", 38, 41, 0.05f);
	SpitBulletRender->ChangeAnimation("Spit");

	SpitBulletCollision = CreateDefaultSubObject<U2DCollision>();
	SpitBulletCollision->SetComponentLocation({ 0, 25 });
	SpitBulletCollision->SetComponentScale({ 48, 48 });
	SpitBulletCollision->SetCollisionGroup(ECollisionGroup::PLAYERSKILL);
	SpitBulletCollision->SetCollisionType(ECollisionType::CirCle);

	DebugOn();
}

ASpitBullet::~ASpitBullet()
{
}

void ASpitBullet::BeginPlay(float _DeltaTime)
{
	Super::BeginPlay();
}

void ASpitBullet::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	if (Time <= 1.0f)
	{
		Time += _DeltaTime;
		AddActorLocation(Dir * Speed * _DeltaTime);
	}
	else
	{
		Destroy();
	}
}

