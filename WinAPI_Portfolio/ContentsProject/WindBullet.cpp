#include "PreCompile.h"
#include "WindBullet.h"

#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/2DCollision.h>

#include "ContentsEnum.h"
#include "Player.h"

AWindBullet::AWindBullet()
{
	WindBulletRender = CreateDefaultSubObject<USpriteRenderer>();
	WindBulletRender->SetSprite("WindBullet.png");
	WindBulletRender->SetComponentScale({ 51, 48 });

	WindBulletRender->CreateAnimation("WindBullet_Right", "WindBullet.png", 1, 1, 1.0f);
	WindBulletRender->CreateAnimation("WindBullet_Left", "WindBullet.png", 0, 0, 1.0f);
	WindBulletRender->ChangeAnimation("WindBullet_Left");

	WindBulletCollision = CreateDefaultSubObject<U2DCollision>();
	WindBulletCollision->SetComponentLocation(GetActorLocation());
	WindBulletCollision->SetComponentScale({ 48, 48 });
	WindBulletCollision->SetCollisionGroup(ECollisionGroup::PlayerSkill);
	WindBulletCollision->SetCollisionType(ECollisionType::CirCle);

	DebugOn();
}

AWindBullet::~AWindBullet()
{
}

void AWindBullet::BeginPlay()
{
	Super::BeginPlay();

	WindBulletRender->ChangeAnimation("WindBullet" + AnimDir);
}

void AWindBullet::Tick(float _DeltaTime)
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

