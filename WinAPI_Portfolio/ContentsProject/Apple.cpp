#include "PreCompile.h"
#include "Apple.h"

#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/ImageManager.h>
#include <EngineCore/2DCollision.h>
#include <EngineCore/EngineCoreDebug.h>

#include "Player.h"
#include "PlayerState.h"

AApple::AApple()
{
	SpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
	SpriteRenderer->SetSprite("WhispySkill.png");
	SpriteRenderer->SetComponentScale({ 50, 50 });
	SpriteRenderer->CreateAnimation("Apple", "WhispySkill.png", 0, 3, 0.2f);
	SpriteRenderer->ChangeAnimation("Apple");


	CollisionComponent = CreateDefaultSubObject<U2DCollision>();
	CollisionComponent->SetComponentLocation({ 0, 0 });
	CollisionComponent->SetComponentScale({ 50, 50 });
	CollisionComponent->SetCollisionGroup(ECollisionGroup::BLOCK);
	CollisionComponent->SetCollisionType(ECollisionType::CirCle);

	DebugOn();
}

AApple::~AApple()
{
}

void AApple::SetColImage(UEngineWinImage* _ColImage)
{
	ColImage = _ColImage;
}

void AApple::SetPlayerLocation(FVector2D _PlayerPos)
{
	PlayerPos = _PlayerPos;
}

void AApple::BeginPlay()
{
	Super::BeginPlay();
	CurTime = 0.0f;
}

void AApple::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	//UEngineDebug::CoreOutPutString("AppleState : " + std::to_string(static_cast<int>(CurState)));
	UEngineDebug::CoreOutPutString("ApplePosX: " + std::to_string(GetActorLocation().X));
	//UEngineDebug::CoreOutPutString("Distance : " + std::to_string(GetActorLocation().X - PlayerPos.X));

	CurTime += _DeltaTime;

	AppleState(_DeltaTime);

	AActor* BossActor = CollisionComponent->CollisionOnce(ECollisionGroup::BOSS);
	if (nullptr != BossActor)
	{
		SetActive(false);
	}

	AActor* Inhale = CollisionComponent->CollisionOnce(ECollisionGroup::PLAYERBODY);
	APlayer* InhalePlayer = dynamic_cast<APlayer*>(Inhale);
	if (nullptr != InhalePlayer && EPlayerState::INHALE == InhalePlayer->GetCurState())
	{
		InhalePlayer->SetIsFull(true);
		InhalePlayer->SkillBoxCollisionSwitch(false);
		InhalePlayer->SetCurState(EPlayerState::IDLE);
		InhalePlayer->State->StopSound();
		APlayer::Score += 150;
		SetActive(false);
	}
	else if (nullptr != InhalePlayer && EPlayerState::INHALE != InhalePlayer->GetCurState())
	{
		if (APlayer::PlayerHp > 0 && true == InhalePlayer->GetCanHurt())
		{
			InhalePlayer->SetCanHurt(false);
			APlayer::PlayerHp--;
		}
		InhalePlayer->SetCurState(EPlayerState::HURTSTART);
		return;
	}

	AActor* Actor = CollisionComponent->CollisionOnce(ECollisionGroup::INHALEBOX);
	if (nullptr != Actor)
	{
		CurState = EAppleState::INHALE;
		return;
	}

	if (CurTime >= 7.0f)
	{
		SetActive(false);
	}
}

bool AApple::PixelPointCheck(ECheckDir _Dir, UColor _Color)
{
	FVector2D Scale = SpriteRenderer->GetTransform().Scale;
	FVector2D Point = FVector2D::ZERO;
	FVector2D NextPoint = FVector2D::ZERO;
	switch (_Dir)
	{
	case ECheckDir::UP:
		Point = GetActorLocation() + FVector2D({ 0.0f, Scale.Y * -0.5f });
		NextPoint = Point + FVector2D::UP;
		break;
	case ECheckDir::DOWN:
		Point = GetActorLocation() + FVector2D({ 0.0f, Scale.Y * 0.5f });
		NextPoint = Point + FVector2D::DOWN;
		break;
	case ECheckDir::LEFT:
		Point = GetActorLocation() + FVector2D({ Scale.X * -0.5f, 0.0f });
		NextPoint = Point + FVector2D::LEFT;
		break;
	case ECheckDir::RIGHT:
		Point = GetActorLocation() + FVector2D({ Scale.X * 0.5f, 0.0f });
		NextPoint = Point + FVector2D::RIGHT;
		break;
	}

	UColor CheckColor = ColImage->GetColor(NextPoint, UColor::MAGENTA);
	if (_Color.operator==(CheckColor))
	{
		return true;
	}
	return false;
}

void AApple::Gravity(float _DeltaTime)
{
	if (!PixelPointCheck(ECheckDir::DOWN, UColor::MAGENTA))
	{
		AddActorLocation(GravityForce * _DeltaTime);
		GravityForce += FVector2D::DOWN * 500.0f * _DeltaTime;
	}
	else
	{
		GravityForce = FVector2D::ZERO;
	}
}

void AApple::AppleState(float _DeltaTime)
{
	switch (CurState)
	{
	case EAppleState::IDLE:
		Idle(_DeltaTime);
		break;
	case EAppleState::CHASE:
		Chase(_DeltaTime);
		break;
	case EAppleState::INHALE:
		Inhale(_DeltaTime);
		break;
	}
}

void AApple::Idle(float _DeltaTime)
{
	Gravity(_DeltaTime);

	if ((GetActorLocation().X) - (PlayerPos.X) < 0)
	{
		AppleDir = FVector2D::RIGHT;
	}
	if ((GetActorLocation().X - PlayerPos.X) > 0)
	{
		AppleDir = FVector2D::LEFT;
	}

	if (PixelPointCheck(ECheckDir::DOWN, UColor::MAGENTA))
	{
		CurState = EAppleState::CHASE;
		return;
	}
}

void AApple::Chase(float _DeltaTime)
{
	Gravity(_DeltaTime);

	AddActorLocation((AppleDir + FVector2D::UP) * 100.0f * _DeltaTime);
}

void AApple::Inhale(float _DeltaTime)
{
	if ((GetActorLocation().X - PlayerPos.X) > 0)
	{
		AddActorLocation(FVector2D::LEFT * 230.0f * _DeltaTime);
	}
	else if ((GetActorLocation().X - PlayerPos.X) < 0)
	{
		AddActorLocation(FVector2D::RIGHT * 230.0f * _DeltaTime);
	}

	AActor* Actor = CollisionComponent->CollisionOnce(ECollisionGroup::INHALEBOX);
	if (nullptr == Actor)
	{
		CurState = EAppleState::CHASE;
		return;
	}
}


