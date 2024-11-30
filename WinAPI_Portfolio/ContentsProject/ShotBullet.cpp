#include "PreCompile.h"
#include "ShotBullet.h"

#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/2DCollision.h>

#include "Player.h"
#include "ContentsEnum.h"

AShotBullet::AShotBullet()
{
	ShotBulletRender = CreateDefaultSubObject<USpriteRenderer>();
	ShotBulletRender->SetSprite("Shotzo.png", 5);
	ShotBulletRender->SetComponentScale({ 51, 51 });
	ShotBulletRender->SetOrder(ERenderOrder::SKILL);

	ShotBulletCollision = CreateDefaultSubObject<U2DCollision>();
	ShotBulletCollision->SetComponentLocation({ 0, 0 });
	ShotBulletCollision->SetComponentScale({ 34, 34 });
	ShotBulletCollision->SetCollisionGroup(ECollisionGroup::MONSTERSKILL);
	ShotBulletCollision->SetCollisionType(ECollisionType::CirCle);
	GetWorld()->CollisionGroupLink(ECollisionGroup::MONSTERSKILL, ECollisionGroup::PLAYERBODY);
	ShotBulletCollision->SetCollisionEnter(std::bind(&AShotBullet::CollisionEnter, this, std::placeholders::_1));
	DebugOn();
}

AShotBullet::~AShotBullet()
{
}

void AShotBullet::SetDirection(EShotDirect _CurDirect)
{
	FVector2D Vector = FVector2D::ZERO;

	switch (_CurDirect)
	{
	case EShotDirect::LEFT:
		Vector += FVector2D::LEFT;
		break;
	case EShotDirect::LEFTUPPER:
		Vector += FVector2D::LEFT + FVector2D::UP;
		break;
	case EShotDirect::UP:
		Vector += FVector2D::UP;
		break;
	case EShotDirect::RIGHTUPPER:
		Vector += FVector2D::RIGHT + FVector2D::UP;
		break;
	case EShotDirect::RIGHT:
		Vector += FVector2D::RIGHT;
		break;
	default:
		break;
	}

	Direction = Vector;
}

void AShotBullet::CollisionEnter(AActor* _ColActor)
{
	APlayer* Player = dynamic_cast<APlayer*>(_ColActor);
	if (nullptr != Player)
	{
		if (APlayer::PlayerHp > 0 && true == Player->GetCanHurt())
		{
			Player->SetCanHurt(false);
			APlayer::PlayerHp--;
		}
		Player->SetCurState(EPlayerState::HURTSTART);
		SetActive(false);
		return;
	}
}

void AShotBullet::BeginPlay()
{
	Super::BeginPlay();
}

void AShotBullet::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	Time += _DeltaTime;

	if (Time <= 2.0f)
	{
		AddActorLocation(Direction * Speed * _DeltaTime);
	}
	else
	{
		SetActive(false);
	}
}
