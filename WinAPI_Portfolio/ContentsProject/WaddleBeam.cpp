#include "PreCompile.h"
#include "WaddleBeam.h"

#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/2DCollision.h>

#include "Player.h"
#include "Monster.h"
#include "ContentsEnum.h"

AWaddleBeam::AWaddleBeam()
{
	for (int i = 0; i < 3; i++)
	{
		WaddleBeamRender[i] = CreateDefaultSubObject<USpriteRenderer>();
		WaddleBeamRender[i]->SetSprite("WaddleDoo_Right.png", 4);
		WaddleBeamRender[i]->SetOrder(ERenderOrder::SKILL);
		WaddleBeamRender[i]->SetComponentLocation((Dir + FVector2D{ 0 , -45 * i }));
		WaddleBeamRender[i]->SetComponentScale({ 40, 40 });
	}

	ArrDir[static_cast<int>(Phase::Phase01)] = FVector2D::UP;
	ArrDir[static_cast<int>(Phase::Phase02)] = FVector2D(-0.45f, -1.0f);
	ArrDir[static_cast<int>(Phase::Phase03)] = FVector2D(-1.45f, -1.0f);;
	ArrDir[static_cast<int>(Phase::Phase04)] = FVector2D::LEFT;
	ArrDir[static_cast<int>(Phase::Phase05)] = FVector2D(-1, 1);

	for (size_t i = 0; i < static_cast<int>(Phase::Phase06); i++)
	{
		ArrDir[i].Normalize();
	}
	
	DebugOn();
}

AWaddleBeam::~AWaddleBeam()
{
}

void AWaddleBeam::SetDir(const FVector2D& _Dir)
{
	Dir = _Dir;
}

void AWaddleBeam::SetEffect(FVector2D _Dir)
{
	float StartInter = 65.0f;

	FVector2D RealDir = _Dir;
	if (0 <= Dir.X)
	{

		RealDir.X = -RealDir.X;
	}

	for (int i = 0; i < 3; i++)
	{
		WaddleBeamRender[i]->SetOrder(ERenderOrder::SKILL);
		WaddleBeamRender[i]->SetComponentLocation(RealDir * (StartInter + (50.0f * i)));

		WaddleBeamCollision[i]->SetComponentLocation(RealDir * (StartInter + (50.0f * i)));
	}
}

void AWaddleBeam::Phase01(float _DeltaTime)
{
	SetEffect(ArrDir[static_cast<int>(Phase::Phase01)]);

	Time -= _DeltaTime;

	if (Time <= 0.0f)
	{
		Time = InterTime;
		PhaseValue = Phase::Phase02;
	}
}

void AWaddleBeam::Phase02(float _DeltaTime)
{
	SetEffect(ArrDir[static_cast<int>(Phase::Phase02)]);

	Time -= _DeltaTime;

	if (Time <= 0.0f)
	{
		Time = InterTime;
		PhaseValue = Phase::Phase03;
	}
}

void AWaddleBeam::Phase03(float _DeltaTime)
{
	SetEffect(ArrDir[static_cast<int>(Phase::Phase03)]);

	Time -= _DeltaTime;

	if (Time <= 0.0f)
	{
		Time = InterTime;
		PhaseValue = Phase::Phase04;
	}
}

void AWaddleBeam::Phase04(float _DeltaTime)
{
	SetEffect(ArrDir[static_cast<int>(Phase::Phase04)]);

	Time -= _DeltaTime;

	if (Time <= 0.0f)
	{
		Time = InterTime;
		PhaseValue = Phase::Phase05;
	}
}

void AWaddleBeam::Phase05(float _DeltaTime)
{
	SetEffect(ArrDir[static_cast<int>(Phase::Phase05)]);

	Time -= _DeltaTime;

	if (Time <= 0.0f)
	{
		Time = InterTime;
		PhaseValue = Phase::Phase06;
	}
}

void AWaddleBeam::Phase06(float _DeltaTime)
{
	Destroy();
}

void AWaddleBeam::SetMosnterSkillCollision()
{
	for (int i = 0; i < 3; i++)
	{
		WaddleBeamCollision[i] = CreateDefaultSubObject<U2DCollision>();
		WaddleBeamCollision[i]->SetComponentLocation((Dir + FVector2D{ 0 , -45 * i }));
		WaddleBeamCollision[i]->SetComponentScale({ 38, 38 });
		WaddleBeamCollision[i]->SetCollisionGroup(ECollisionGroup::MONSTERSKILL);
		WaddleBeamCollision[i]->SetCollisionType(ECollisionType::CirCle);

		GetWorld()->CollisionGroupLink(ECollisionGroup::MONSTERSKILL, ECollisionGroup::PLAYERBODY);
		WaddleBeamCollision[i]->SetCollisionEnter(std::bind(&AWaddleBeam::MonsterSkillCollisionEnter, this, std::placeholders::_1));
	}
}

void AWaddleBeam::SetPlayerSkillCollision()
{
	for (int i = 0; i < 3; i++)
	{
		WaddleBeamCollision[i] = CreateDefaultSubObject<U2DCollision>();
		WaddleBeamCollision[i]->SetComponentLocation((Dir + FVector2D{ 0 , -45 * i }));
		WaddleBeamCollision[i]->SetComponentScale({ 38, 38 });
		WaddleBeamCollision[i]->SetCollisionGroup(ECollisionGroup::PLAYERSKILL);
		WaddleBeamCollision[i]->SetCollisionType(ECollisionType::CirCle);

		GetWorld()->CollisionGroupLink(ECollisionGroup::PLAYERSKILL, ECollisionGroup::MONSTERBODY);
		WaddleBeamCollision[i]->SetCollisionEnter(std::bind(&AWaddleBeam::PlayerSkillCollisionEnter, this, std::placeholders::_1));
	}
}

void AWaddleBeam::MonsterSkillCollisionEnter(AActor* _ColActor)
{
	APlayer* Player = dynamic_cast<APlayer*>(_ColActor);
	if (nullptr != Player)
	{
		Player->SetCurState(EPlayerState::HURT);
		return;
	}
}

void AWaddleBeam::PlayerSkillCollisionEnter(AActor* _ColActor)
{
	AMonster* Monster = dynamic_cast<AMonster*>(_ColActor);
	if (nullptr != Monster)
	{
		Monster->SetMonsterState(EMonsterState::DIE);
		return;
	}
}

void AWaddleBeam::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	switch (PhaseValue)
	{
	case Phase::Phase01:
		Phase01(_DeltaTime);
		break;
	case Phase::Phase02:
		Phase02(_DeltaTime);
		break;
	case Phase::Phase03:
		Phase03(_DeltaTime);
		break;
	case Phase::Phase04:
		Phase04(_DeltaTime);
		break;
	case Phase::Phase05:
		Phase05(_DeltaTime);
		break;
	case Phase::Phase06:
		Phase06(_DeltaTime);
		break;
	default:
		break;
	}
}
