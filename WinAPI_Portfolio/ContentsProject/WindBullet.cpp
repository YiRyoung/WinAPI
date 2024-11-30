#include "PreCompile.h"
#include "WindBullet.h"

#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/2DCollision.h>

#include "Monster.h"
#include "ContentsEnum.h"

AWindBullet::AWindBullet()
{
	WindBulletRender = CreateDefaultSubObject<USpriteRenderer>();
	WindBulletRender->SetSprite("Kirby_Normal_Right.png", 27);
	WindBulletRender->SetComponentScale({ 100, 100 });
	WindBulletRender->SetOrder(ERenderOrder::SKILL);

	WindBulletCollision = CreateDefaultSubObject<U2DCollision>();
	WindBulletCollision->SetComponentLocation({0, 30});
	WindBulletCollision->SetComponentScale({ 48, 48 });
	WindBulletCollision->SetCollisionGroup(ECollisionGroup::PLAYERSKILL);
	WindBulletCollision->SetCollisionType(ECollisionType::CirCle);
	WindBulletCollision->SetCollisionEnter(std::bind(&AWindBullet::CollisionEnter, this, std::placeholders::_1));
	DebugOn();
}

AWindBullet::~AWindBullet()
{
}

void AWindBullet::SetDir(std::string _KirbyAnimDir)
{
	AnimDir = _KirbyAnimDir;

	if ("_Left" == AnimDir)
	{
		Dir = FVector2D::LEFT;
		SpawnPos = FVector2D({ -55, -45 });
	}
	else
	{
		Dir = FVector2D::RIGHT;
		SpawnPos = FVector2D({ 55, -45 });
	}
}

void AWindBullet::CollisionEnter(AActor* _ColActor)
{
	AMonster* Result = dynamic_cast<AMonster*>(_ColActor);
	if (nullptr != Result)
	{
		Result->SetMonsterState(EMonsterState::DIE);
		SetActive(false);
		return;
	}
}

void AWindBullet::BeginPlay()
{
	Super::BeginPlay();

	if ("_Left" == AnimDir)
	{
		WindBulletRender->SetSprite("Kirby_Normal_Left.png", 27);
	}
	else
	{
		WindBulletRender->SetSprite("Kirby_Normal_Right.png", 27);
	}
}

void AWindBullet::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	if (Time <= 0.4f)
	{
		Time += _DeltaTime;
		AddActorLocation(Dir * Speed * _DeltaTime);
	}
	else
	{
		Time = 0.0f;
		SetActive(false);
	}
}

