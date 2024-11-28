#include "PreCompile.h"
#include "SpitBullet.h"

#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/2DCollision.h>

#include "Monster.h"
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
	SpitBulletCollision->SetCollisionEnter(std::bind(&ASpitBullet::CollisionEnter, this, std::placeholders::_1));
	DebugOn();
}

ASpitBullet::~ASpitBullet()
{
}

void ASpitBullet::SetDir(std::string _KirbyAnimDir)
{
	AnimDir = _KirbyAnimDir;

	if ("_Left" == AnimDir)
	{
		Dir = FVector2D::LEFT;
		SpawnPos = FVector2D({ -55, -50 });
	}
	else
	{
		Dir = FVector2D::RIGHT;
		SpawnPos = FVector2D({ 55, -50 });
	}
}

void ASpitBullet::CollisionEnter(AActor* _ColActor)
{
	AMonster* Result = dynamic_cast<AMonster*>(_ColActor);
	if (nullptr != Result)
	{
		Result->SetMonsterState(EMonsterState::DIE);
		Destroy();
		return;
	}
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
		Time = 0.0f;
		Destroy();
	}
}

