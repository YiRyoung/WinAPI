#include "PreCompile.h"
#include "WaddleDee.h"

#include <EnginePlatform/EngineInput.h>

#include <EngineCore/ImageManager.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/2DCollision.h>
#include <EngineCore/EngineCoreDebug.h>

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

	MonsterFSM(_DeltaTime);
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

	// Destory
	SpriteRenderer->CreateAnimation("Destroy", "Destory.png", 4, 6, 0.07f, false);

	// Start Animation
	SpriteRenderer->ChangeAnimation("Walk_Left");
}

void AWaddleDee::SetAnimDir()
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

bool AWaddleDee::BottomPointCheck(UColor _Color)
{
	FVector2D MonsterScale = SpriteRenderer->GetTransform().Scale;
	FVector2D DownPoint = GetActorLocation() + FVector2D({ 0.0f, MonsterScale.Y * 0.5f });
	FTransform DownTransform = GetTransform();
	DownTransform.Location += FVector2D({ 0.0f, MonsterScale.Y * 0.5f }) - GetWorld()->GetCameraPos();
	DownTransform.Scale = { 6,6 };
	UEngineDebug::CoreDebugRender(DownTransform, UEngineDebug::EDebugPosType::Circle);

	FVector2D NextDownPoint = DownPoint + FVector2D::DOWN;
	UColor DownColor = ColImage->GetColor(NextDownPoint, UColor::MAGENTA);
	if (_Color.operator==(DownColor))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool AWaddleDee::LeftPointCheck(UColor _Color)
{
	FVector2D MonsterScale = SpriteRenderer->GetTransform().Scale;
	FVector2D LeftPoint = GetActorLocation() + FVector2D({ MonsterScale.X * -0.5f, 0.0f });
	FTransform LeftTransform = GetTransform();
	LeftTransform.Location += FVector2D({ MonsterScale.X * -0.5f, 0.0f }) - GetWorld()->GetCameraPos();
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

bool AWaddleDee::RightPointCheck(UColor _Color)
{
	FVector2D MonsterScale = SpriteRenderer->GetTransform().Scale;

	FVector2D RightPoint = GetActorLocation() + FVector2D({ MonsterScale.X * 0.5f, 0.0f });
	FTransform RightTransform = GetTransform();
	RightTransform.Location += FVector2D({ MonsterScale.X * 0.5f, 0.0f }) - GetWorld()->GetCameraPos();
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

bool AWaddleDee::PointCheck(CheckDir _Dir, UColor _Color)
{
	switch (_Dir)
	{
	case CheckDir::LEFT:
		if (true == LeftPointCheck(_Color))
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case CheckDir::RIGHT:
		if (true == RightPointCheck(_Color))
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case CheckDir::DOWN:
		if (true == BottomPointCheck(_Color))
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	default:
		break;
	}

}

void AWaddleDee::MonsterFSM(float _DeltaTime)
{
	switch (CurState)
	{
	case MonsterState::PAUSE:
		break;
	case MonsterState::MOVE:
		Move(_DeltaTime);
		break;
	case MonsterState::ATTACK:
		break;
	case MonsterState::DIED:
		Died(_DeltaTime);
		break;
	default:
		break;
	}
}

void AWaddleDee::Gravity(float _DeltaTime)
{
	if (!PointCheck(CheckDir::DOWN, UColor::MAGENTA)
		&& !PointCheck(CheckDir::DOWN, UColor::BLACK) && !PointCheck(CheckDir::DOWN, UColor::YELLOW))
	{
		AddActorLocation(GravityForce * _DeltaTime);
		GravityForce += FVector2D::DOWN * 500.0f * _DeltaTime;
	}
	else
	{
		GravityForce = FVector2D::ZERO;
	}
}

void AWaddleDee::Move(float _DeltaTime)
{
	// 앞이 벽이라면 방향 바꿔서 이동
	Gravity(_DeltaTime);

	SpriteRenderer->ChangeAnimation("Walk" + AnimDir);

	if ("_Left" == AnimDir)
	{
		if (PointCheck(CheckDir::LEFT, UColor::MAGENTA))
		{
			AddActorLocation(FVector2D::LEFT * 300.0f * _DeltaTime);
		}
	}
}

void AWaddleDee::Died(float _DeltaTime)
{
	SpriteRenderer->ChangeAnimation("Destroy");
	if (SpriteRenderer->IsCurAnimationEnd())
	{
		Destroy();
	}
}

void AWaddleDee::SetDestory()
{
	CurState = MonsterState::DIED;
	return;
}