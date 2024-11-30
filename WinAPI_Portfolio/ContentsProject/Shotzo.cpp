#include "PreCompile.h"
#include "Shotzo.h"

#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/EngineCoreDebug.h>

#include "Player.h"
#include "ShotBullet.h"

AShotzo::AShotzo()
{
	SetMonster("Shotzo.png", { 51, 51 }, EAbilityType::NORMAL);
	SetCollision({ 54, 54 });
	CollisionComponent->SetCollisionGroup(ECollisionGroup::OBSTACLE);
	GetWorld()->CollisionGroupLink(ECollisionGroup::OBSTACLE, ECollisionGroup::PLAYERBODY);
	CollisionComponent->SetCollisionEnter(std::bind(&AShotzo::CollisionEnter, this, std::placeholders::_1));
	DebugOn();
}

AShotzo::~AShotzo()
{
}

void AShotzo::BeginPlay()
{
	AMonster::BeginPlay();
}

void AShotzo::Tick(float _DeltaTime)
{
	AMonster::Tick(_DeltaTime);
	CurTime += _DeltaTime;
	PauseTime += _DeltaTime;

	UEngineDebug::CoreOutPutString("ShotDir : " + std::to_string(static_cast<int>(ShotDir)));
	UEngineDebug::CoreOutPutString("PlayerDirectionX : " + std::to_string(AMonster::PlayerDistanceX));
	UEngineDebug::CoreOutPutString("PlayerDirectionY : " + std::to_string(AMonster::PlayerDistanceY));

	if (!IsAttack && CurTime >= 3.0f)
	{
		IsAttack = true;
	}
}

bool AShotzo::CheckDistance()
{
	float DisX = AMonster::PlayerDistanceX;
	bool IsTrue = (abs(DisX) <= 250.0f) ? true : false;
	return IsTrue;
}

void AShotzo::CheckDirect()
{
	float DistanceX = AMonster::PlayerDistanceX;
	float DistanceY = AMonster::PlayerDistanceY;

	if ((DistanceX > 0) && (DistanceY >= -24) && (DistanceY <= 80))
	{
		ShotDir = EShotDirect::LEFT;
	}
	if ((DistanceX > 0) && (abs(DistanceX < 180)) && (DistanceY > 0))
	{
		ShotDir = EShotDirect::LEFTUPPER;
	}
	if (DistanceY >= 110 )
	{
		ShotDir = EShotDirect::UP;
	}
	if ((DistanceX < 0) && (abs(DistanceX < 180)) && (DistanceY >= 80))
	{
		ShotDir = EShotDirect::RIGHTUPPER;
	}
	if ((DistanceX < 0) && (DistanceY >= -24) && (DistanceY <= 80))
	{
		ShotDir = EShotDirect::RIGHT;
	}
	if ((DistanceX > 0) && (DistanceY <= -24))
	{
		ShotDir = EShotDirect::LEFT;
	}
	if ((DistanceX < 0) && (DistanceY <= -24))
	{
		ShotDir = EShotDirect::RIGHT;
	}
}

void AShotzo::Pause(float _DeltaTime)
{
	if (PauseTime >= 0.2f)
	{
		PauseTime = 0.0f;
		CheckDirect();
		SetMonsterState(EMonsterState::CHASE);
		return;
	}
}

void AShotzo::Chase(float _DetlaTime)
{
	switch (ShotDir)
	{
	case EShotDirect::LEFT:
		SpriteRenderer->SetSprite("Shotzo.png", 0);
		break;
	case EShotDirect::LEFTUPPER:
		SpriteRenderer->SetSprite("Shotzo.png", 1);
		break;
	case EShotDirect::UP:
		SpriteRenderer->SetSprite("Shotzo.png", 2);
		break;
	case EShotDirect::RIGHTUPPER:
		SpriteRenderer->SetSprite("Shotzo.png", 3);
		break;
	case EShotDirect::RIGHT:
		SpriteRenderer->SetSprite("Shotzo.png", 4);
		break;
	default:
		break;
	}
	
	if ( IsAttack && CheckDistance() && AMonster::PlayerDistanceY > -24)
	{
		CurTime = 0.0f;
		SetMonsterState(EMonsterState::ATTACKSTART);
		return;
	}
}

void AShotzo::AttackStart(float _DetlaTime)
{
	if (true == BGMPlayer.IsPlaying())
	{
		BGMPlayer.Stop();
	}
	BGMPlayer = UEngineSound::Play("Shot.wav");
	
	IsAttack = false;
	SetMonsterState(EMonsterState::ATTACK);
	return;
}

void AShotzo::Attack(float _DetlaTime)
{
	NewBullet = GetWorld()->SpawnActor<AShotBullet>();
	NewBullet->SetDirection(ShotDir);
	NewBullet->SetActorLocation(GetActorLocation() + NewBullet->GetBulletDirection() * 10);

	PauseTime = 0.0f;
	SetMonsterState(EMonsterState::PAUSE);
	return;
}

void AShotzo::CollisionEnter(AActor* _ColActor)
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
		return;
	}
}
