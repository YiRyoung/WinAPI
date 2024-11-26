#include "PreCompile.h"
#include "CutterSlider.h"

#include <EngineCore/SpriteRenderer.h>

#include "ContentsEnum.h"

ACutterSlider::ACutterSlider()
{
	CutterSliderRender = CreateDefaultSubObject<USpriteRenderer>();
	CutterSliderRender->SetSprite("CutterSlider.png");
	CutterSliderRender->SetOrder(ERenderOrder::SKILL);
	CutterSliderRender->SetComponentScale({ 60, 60 });

	CutterSliderRender->CreateAnimation("Slider", "CutterSlider.png", 0, 3, 0.01f);
	CutterSliderRender->ChangeAnimation("Slider");


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
}

void ACutterSlider::SetPlayerSkillCollision()
{
}

void ACutterSlider::MonsterSkillCollisionEnter(AActor* _ColActor)
{
}

void ACutterSlider::PlayerSkillCollisionEnter(AActor* _ColActor)
{
}

void ACutterSlider::BeginPlay()
{
	Super::BeginPlay();
}

void ACutterSlider::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	AddActorLocation(Dir * PushForce * _DeltaTime);

	ReversePush += -Dir * 300.0f * _DeltaTime;
	AddActorLocation(ReversePush * _DeltaTime);
}
