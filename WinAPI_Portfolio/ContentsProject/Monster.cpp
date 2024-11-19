#include "PreCompile.h"
#include "Monster.h"

#include <EngineCore/EngineAPICore.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/2DCollision.h>
#include <EngineCore/EngineCoreDebug.h>

#include "ContentsEnum.h"

AMonster::AMonster()
{
	SetMonsterType();
	SetMonsterAnim();
	DebugOn();
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
}

void AMonster::SetMonsterType()
{
	//UImageManager::GetInst().CuttingSprite("Kirby_Normal_Left.png", { 32, 32 });
	//UImageManager::GetInst().CuttingSprite("Kirby_Normal_Right.png", { 32, 32 });

	SpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
	SpriteRenderer->SetSprite("Kirby_Normal_Left.png");
	SpriteRenderer->SetComponentScale({ 94, 94 });

	U2DCollision* CollisionComponent = CreateDefaultSubObject<U2DCollision>();
	CollisionComponent->SetComponentLocation({ 0, 0 });
	CollisionComponent->SetComponentScale({ 50, 50 });
	CollisionComponent->SetCollisionGroup(ECollisionGroup::MonsterBody);
	CollisionComponent->SetCollisionType(ECollisionType::Rect);
}

void AMonster::SetMonsterAnim()
{
	// Idle
	SpriteRenderer->CreateAnimation("Idle_Left", "Kirby_Normal_Left.png", 0, 1, 1.0f);
	SpriteRenderer->CreateAnimation("Idle_Right", "Kirby_Normal_Right.png", 0, 1, 1.0f);

	// Walk
	SpriteRenderer->CreateAnimation("Walk_Left", "Kirby_Normal_Left.png", 2, 5, 0.1f);
	SpriteRenderer->CreateAnimation("Walk_Right", "Kirby_Normal_Right.png", 2, 5, 0.1f);

	SpriteRenderer->ChangeAnimation("Idle_Left");
}
