#include "PreCompile.h"
#include "CutterSlider.h"

#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/2DCollision.h>

#include "Player.h"
#include "Monster.h"

#include "ContentsEnum.h"

ACutterSlider::ACutterSlider()
{
	CutterSliderRender = CreateDefaultSubObject<USpriteRenderer>();
	CutterSliderRender->SetSprite("CutterSlider.png");
	CutterSliderRender->SetOrder(ERenderOrder::SKILL);
	CutterSliderRender->SetComponentScale({ 60, 60 });

	CutterSliderRender->CreateAnimation("Slider", "CutterSlider.png", 0, 3, 0.01f);
	CutterSliderRender->ChangeAnimation("Slider");
	DebugOn();
}

ACutterSlider::~ACutterSlider()
{
}

void ACutterSlider::SetDir(const FVector2D& _Dir)
{
	Dir = _Dir;
}

void ACutterSlider::SetMosnterSkillCollision()
{
	CutterSliderCollision = CreateDefaultSubObject<U2DCollision>();
	CutterSliderCollision->SetComponentScale({ 64, 64 });
	CutterSliderCollision->SetCollisionGroup(ECollisionGroup::MONSTERSKILL);
	CutterSliderCollision->SetCollisionType(ECollisionType::CirCle);
}

void ACutterSlider::SetPlayerSkillCollision()
{
	CutterSliderCollision = CreateDefaultSubObject<U2DCollision>();
	CutterSliderCollision->SetComponentScale({ 64, 64 });
	CutterSliderCollision->SetCollisionGroup(ECollisionGroup::PLAYERSKILL);
	CutterSliderCollision->SetCollisionType(ECollisionType::CirCle);
}

void ACutterSlider::BeginPlay()
{
	Super::BeginPlay();
}

void ACutterSlider::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	CurTime += _DeltaTime;

	AddActorLocation(Dir * PushForce * _DeltaTime);

	ReversePush += -Dir * 300.0f * _DeltaTime;
	AddActorLocation(ReversePush * _DeltaTime);

	AActor* PlayerAct = CutterSliderCollision->CollisionOnce(ECollisionGroup::PLAYERBODY);
	if (nullptr != PlayerAct && (static_cast<int>(ECollisionGroup::MONSTERSKILL) == CutterSliderCollision->GetGroup()))
	{
		APlayer* Player = dynamic_cast<APlayer*>(PlayerAct);
		Player->SetCurState(EPlayerState::HURT);
		Destroy();
		return;
	}
	
	AActor* MonsterAct = CutterSliderCollision->CollisionOnce(ECollisionGroup::MONSTERBODY);
	if (nullptr != MonsterAct && (static_cast<int>(ECollisionGroup::PLAYERSKILL) == CutterSliderCollision->GetGroup()))
	{
		AMonster* Monster = dynamic_cast<AMonster*>(MonsterAct);
		Monster->SetMonsterState(EMonsterState::DIE);
		Destroy();
		return;
	}

	if (CurTime >= 2.1f)
	{
		Destroy();
	}
}
