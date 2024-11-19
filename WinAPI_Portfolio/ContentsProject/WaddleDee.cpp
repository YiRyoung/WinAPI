#include "PreCompile.h"
#include "WaddleDee.h"

#include <EngineCore/ImageManager.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/2DCollision.h>

#include "ContentsEnum.h"

AWaddleDee::AWaddleDee()
{
	SetMonster();
	SetMonsterAnimation();
}

AWaddleDee::~AWaddleDee()
{
}

void AWaddleDee::BeginPlay()
{
	Super::BeginPlay();
}

void AWaddleDee::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	AActor* Result = CollisionComponent->CollisionOnce(ECollisionGroup::PlayerBody);
	if (nullptr != Result)
	{
		Destroy();
	}
}

void AWaddleDee::SetMonster()
{
	SpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
	SpriteRenderer->SetSprite("Waddle Dee_Left.png");
	SpriteRenderer->SetComponentScale({ 50, 50 });

	CollisionComponent = CreateDefaultSubObject<U2DCollision>();
	CollisionComponent->SetComponentLocation({ 0, 0 });
	CollisionComponent->SetComponentScale({ 54, 54 });
	CollisionComponent->SetCollisionGroup(ECollisionGroup::MonsterBody);
	CollisionComponent->SetCollisionType(ECollisionType::CirCle);

	DebugOn();
}

void AWaddleDee::SetMonsterAnimation()
{
	// Idle
	SpriteRenderer->CreateAnimation("Idle_Left", "Waddle Dee_Left.png", 0, 0, 1.0f);
	SpriteRenderer->CreateAnimation("Idle_Right", "Waddle Dee_Right.png", 0, 0, 1.0f);

	// Walk
	SpriteRenderer->CreateAnimation("Walk_Left", "Waddle Dee_Left.png", 0, 1, 0.5f);
	SpriteRenderer->CreateAnimation("Walk_Right", "Waddle Dee_Right.png", 0, 1, 0.5f);

	// Start Animation
	SpriteRenderer->ChangeAnimation("Walk_Left");
}

