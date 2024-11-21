#include "PreCompile.h"
#include "Monster.h"

#include <EnginePlatform/EngineInput.h>

#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/ImageManager.h>
#include <EngineCore/2DCollision.h>
#include <EngineCore/EngineCoreDebug.h>

#include "ContentsEnum.h"

AMonster::AMonster()
{
	
}

AMonster::~AMonster()
{
}

void AMonster::BeginPlay()
{
	Super::BeginPlay();
}

void AMonster::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	MonsterFSM(_DeltaTime);
}

void AMonster::GetColImage(std::string _ColImageName)
{
	ColImage = UImageManager::GetInst().FindImage(_ColImageName);
}

void AMonster::SetMonster(std::string _SpriteName, FVector2D _SpriteScale)
{
	SpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
	SpriteRenderer->SetSprite(_SpriteName);
	SpriteRenderer->SetComponentScale(_SpriteScale);
}

void AMonster::SetCollision(FVector2D _CollisionScale)
{
	CollisionComponent = CreateDefaultSubObject<U2DCollision>();
	CollisionComponent->SetComponentLocation({ 0, 0 });
	CollisionComponent->SetComponentScale(_CollisionScale);
	CollisionComponent->SetCollisionGroup(ECollisionGroup::MonsterBody);
	CollisionComponent->SetCollisionType(ECollisionType::CirCle);
	CollisionComponent->SetCollisionEnter(std::bind(&AMonster::CollisionEnter, this, std::placeholders::_1));

	GetWorld()->CollisionGroupLink(ECollisionGroup::MonsterBody, ECollisionGroup::PlayerSkill);
}

void AMonster::SetSkillCollision(FVector2D _CollisionScale, ECollisionType _CollisionType)
{
	SkillCollision = CreateDefaultSubObject<U2DCollision>();
	SkillCollision->SetComponentLocation({ 0, 0 });
	SkillCollision->SetComponentScale(_CollisionScale);
	SkillCollision->SetCollisionGroup(ECollisionGroup::MonsterSkill);
	SkillCollision->SetCollisionType(_CollisionType);
}

FVector2D AMonster::GetMonsterScale() const
{
	return SpriteRenderer->GetTransform().Scale;
}

void AMonster::ChangeMonsterAnim(std::string _AnimName)
{
	SpriteRenderer->ChangeAnimation(_AnimName + AnimDir);
}

bool AMonster::BottomPixelCheck(UColor _Color)
{
	FVector2D Scale = GetMonsterScale();
	FVector2D LeftPoint = GetActorLocation() + FVector2D({ Scale.X * -0.5f, 0.0f });
	
	FTransform LeftTransform = GetTransform();
	LeftTransform.Location += FVector2D({ Scale.X * -0.5f, 0.0f }) - GetWorld()->GetCameraPos();
	LeftTransform.Scale = { 6, 6 };
	UEngineDebug::CoreDebugRender(LeftTransform, UEngineDebug::EDebugPosType::Circle);

	FVector2D NextLeftPoint = LeftPoint + FVector2D::LEFT;
	UColor LeftColor = ColImage->GetColor(NextLeftPoint, UColor::MAGENTA);
	if (_Color.operator==(LeftColor))
	{
		return true;
	}
	else
	{
		return false;
	}

}

bool AMonster::LeftPixelCheck(UColor _Color)
{
	FVector2D Scale = GetMonsterScale();
	FVector2D LeftPoint = GetActorLocation() + FVector2D({ Scale.X * -0.5f, 0.0f });

	FTransform LeftTransform = GetTransform();
	LeftTransform.Location += FVector2D({ Scale.X * -0.5f, 0.0f }) - GetWorld()->GetCameraPos();
	LeftTransform.Scale = { 6,6 };
	UEngineDebug::CoreDebugRender(LeftTransform, UEngineDebug::EDebugPosType::Circle);

	FVector2D NextLeftPoint = LeftPoint + FVector2D::LEFT;
	UColor LeftColor = ColImage->GetColor(NextLeftPoint, UColor::MAGENTA);
	if (_Color.operator==(LeftColor))
	{
		return true;
	}
	else
	{
		return false;
	}

}

bool AMonster::RightPixelCheck(UColor _Color)
{
	FVector2D Scale = GetMonsterScale();
	FVector2D RightPoint = GetActorLocation() + FVector2D({ Scale.X * 0.5f, 0.0f });

	FTransform RightTransform = GetTransform();
	RightTransform.Location += FVector2D({ Scale.X * 0.5f, 0.0f }) - GetWorld()->GetCameraPos();
	RightTransform.Scale = { 6,6 };
	UEngineDebug::CoreDebugRender(RightTransform, UEngineDebug::EDebugPosType::Circle);

	FVector2D NextRightPoint = RightPoint + FVector2D::RIGHT;
	UColor RightColor = ColImage->GetColor(NextRightPoint, UColor::MAGENTA);
	if (_Color.operator==(RightColor))
	{
		return true;
	}
	else
	{
		return false;
	}

}

void AMonster::CollisionEnter(AActor* _ColActor)
{
	SetMonsterState(MonsterState::DIED);
	return;
}

void AMonster::MonsterFSM(float _DeltaTime)
{
	switch (CurState)
	{
	case MonsterState::PAUSE:
		Pause(_DeltaTime);
		break;
	case MonsterState::CHASE:
		Chase(_DeltaTime);
		break;
	case MonsterState::ATTACK:
		Attack(_DeltaTime);
		break;
	case MonsterState::INHALE:
		Inhale(_DeltaTime);
		break;
	case MonsterState::DIED:
		Died(_DeltaTime);
		break;
	}
}

void AMonster::Gravity(float _DeltaTime)
{

}

void AMonster::Pause(float _DeltaTime)
{
}

void AMonster::Chase(float _DeltaTime)
{
}

void AMonster::Attack(float _DeltaTime)
{
}

void AMonster::Inhale(float _DeltaTime)
{
	
}

void AMonster::Died(float _DeltaTime)
{
}

