#include "PreCompile.h"
#include "SpitBullet.h"

#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/2DCollision.h>

#include "ContentsEnum.h"

ASpitBullet::ASpitBullet()
{

	SpitBulletRender = CreateDefaultSubObject<USpriteRenderer>();
	SpitBulletRender->SetSprite("SpitBullet.png");
	SpitBulletRender->SetComponentScale({ 50, 50 });

	SpitBulletRender->CreateAnimation("SpitBullet", "SpitBullet.png", 0, 3, 0.05f);
	SpitBulletRender->ChangeAnimation("SpitBullet");

	SpitBulletCollision = CreateDefaultSubObject<U2DCollision>();
	SpitBulletCollision->SetComponentLocation(GetActorLocation());
	SpitBulletCollision->SetComponentScale({ 52, 52 });
	SpitBulletCollision->SetCollisionGroup(ECollisionGroup::PlayerSkill);
	SpitBulletCollision->SetCollisionType(ECollisionType::CirCle);

	DebugOn();
}

ASpitBullet::~ASpitBullet()
{
}

void ASpitBullet::BeginPlay()
{
	Super::BeginPlay();

	SpitBulletRender->ChangeAnimation("SpitBullet");
}

void ASpitBullet::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	if (Time <= 0.7f)
	{
		Time += _DeltaTime;
		AddActorLocation(Dir * Speed * _DeltaTime);
	}
	else
	{
		Destroy();
	}
}

