#include "PreCompile.h"
#include "Monster.h"

#include <EnginePlatform/EngineInput.h>

#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/ImageManager.h>
#include <EngineCore/2DCollision.h>

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
	SetAnimDir();
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
}

void AMonster::SetSkillCollision(FVector2D _CollisionScale, ECollisionType _CollisionType)
{
	SkillCollision = CreateDefaultSubObject<U2DCollision>();
	SkillCollision->SetComponentLocation({ 0, 0 });
	SkillCollision->SetComponentScale(_CollisionScale);
	SkillCollision->SetCollisionGroup(ECollisionGroup::MonsterSkill);
	SkillCollision->SetCollisionType(_CollisionType);
}

void AMonster::SetAnimDir()
{
	if (UEngineInput::GetInst().IsPress(VK_LEFT))
	{
		AnimDir = "_Left";
	}
	else if (UEngineInput::GetInst().IsPress(VK_RIGHT))
	{
		AnimDir = "_Right";
	}
}

void AMonster::ChangeMonsterAnim(std::string _AnimName)
{
	SpriteRenderer->ChangeAnimation(_AnimName + AnimDir);
}

bool AMonster::BottomPixelCheck(UColor _Color)
{
	return false;
}

bool AMonster::LeftPixelCheck(UColor _Color)
{
	return false;
}

bool AMonster::RightPixelCheck(UColor _Color)
{
	return false;
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

