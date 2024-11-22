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

	UEngineDebug::CoreOutPutString("State : " + std::to_string(static_cast<int>(GetMonsterState())));
	UEngineDebug::CoreOutPutString("Pos : " + std::to_string(abs(Pos.X)));
	
	MonsterFSM(_DeltaTime);
}

void AMonster::GetColImage(std::string _ColImageName)
{
	ColImage = UImageManager::GetInst().FindImage(_ColImageName);
}

void AMonster::SetAnimDir(FVector2D _PlayerLocation)
{	
	SetPos(_PlayerLocation);

	if (Pos.X <= 0)
	{
		AnimDir = "_Left";
	}
	else
	{
		AnimDir = "_Right";
	}
}

void AMonster::SetMonster(std::string _SpriteName, FVector2D _SpriteScale)
{
	SpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
	SpriteRenderer->SetSprite(_SpriteName);
	SpriteRenderer->SetComponentScale(_SpriteScale);
}

void AMonster::SetPos(FVector2D _PlayerLocation)
{
	Pos = GetActorLocation() - _PlayerLocation;
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

void AMonster::SetLeftSkillCollision(FVector2D _CollisionLocation, FVector2D _CollisionScale, ECollisionType _CollisionType)
{
	LeftSkillCollision = CreateDefaultSubObject<U2DCollision>();
	LeftSkillCollision->SetComponentLocation(_CollisionLocation);
	LeftSkillCollision->SetComponentScale(_CollisionScale);
	LeftSkillCollision->SetCollisionGroup(ECollisionGroup::MonsterSkill);
	LeftSkillCollision->SetCollisionType(_CollisionType);
	LeftSkillCollision->SetActive(false);
}

void AMonster::SetRightSkillCollision(FVector2D _CollisionLocation, FVector2D _CollisionScale, ECollisionType _CollisionType)
{
	RightSkillCollision = CreateDefaultSubObject<U2DCollision>();
	RightSkillCollision->SetComponentLocation(_CollisionLocation);
	RightSkillCollision->SetComponentScale(_CollisionScale);
	RightSkillCollision->SetCollisionGroup(ECollisionGroup::MonsterSkill);
	RightSkillCollision->SetCollisionType(_CollisionType);
	RightSkillCollision->SetActive(false);
}

FVector2D AMonster::GetMonsterScale() const
{
	return SpriteRenderer->GetTransform().Scale;
}

void AMonster::ChangeMonsterAnim(std::string _AnimName)
{
	SpriteRenderer->ChangeAnimation(_AnimName + AnimDir);
}

bool AMonster::MonsterAnimFinish()
{
	if (SpriteRenderer->IsCurAnimationEnd() == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool AMonster::PixelLineCheck(CheckDir _Dir, UColor _Color)
{
	FVector2D MonsterScale = SpriteRenderer->GetTransform().Scale;

	FVector2D LeftUpperPoint = GetActorLocation() + FVector2D({ MonsterScale.X * -0.5f, MonsterScale.Y * -0.5f });
	FTransform PlayerTransform0 = GetTransform();
	PlayerTransform0.Location += FVector2D({ MonsterScale.X * -0.5f, MonsterScale.Y * -0.5f }) - GetWorld()->GetCameraPos();
	PlayerTransform0.Scale = { 6,6 };
	UEngineDebug::CoreDebugRender(PlayerTransform0, UEngineDebug::EDebugPosType::Circle);

	FVector2D RightUpperPoint = GetActorLocation() + FVector2D({ MonsterScale.X * 0.5f, MonsterScale.Y * -0.5f });
	FTransform PlayerTransform1 = GetTransform();
	PlayerTransform1.Location += FVector2D({ MonsterScale.X * 0.5f, MonsterScale.Y * -0.5f }) - GetWorld()->GetCameraPos();
	PlayerTransform1.Scale = { 6,6 };
	UEngineDebug::CoreDebugRender(PlayerTransform1, UEngineDebug::EDebugPosType::Circle);

	FVector2D LeftBottomPoint = GetActorLocation() + FVector2D({ MonsterScale.X * -0.5f, MonsterScale.Y * 0.5f });
	FTransform PlayerTransform2 = GetTransform();
	PlayerTransform2.Location += FVector2D({ MonsterScale.X * -0.5f, MonsterScale.Y * 0.5f }) - GetWorld()->GetCameraPos();
	PlayerTransform2.Scale = { 6,6 };
	UEngineDebug::CoreDebugRender(PlayerTransform2, UEngineDebug::EDebugPosType::Circle);

	FVector2D RightBottomPoint = GetActorLocation() + FVector2D({ MonsterScale.X * 0.5f, MonsterScale.Y * 0.5f });
	FTransform PlayerTransform3 = GetTransform();
	PlayerTransform3.Location += FVector2D({ MonsterScale.X * 0.5f, MonsterScale.Y * 0.5f }) - GetWorld()->GetCameraPos();
	PlayerTransform3.Scale = { 6,6 };
	UEngineDebug::CoreDebugRender(PlayerTransform3, UEngineDebug::EDebugPosType::Circle);

	switch (_Dir)
	{
	case CheckDir::UP:
		for (float i = 0.0f; i <= (RightUpperPoint.X - LeftUpperPoint.X); i++)
		{
			FVector2D Point = { (RightUpperPoint.X - i), RightUpperPoint.Y };
			FVector2D PointUp = Point + FVector2D::UP;
			UColor CheckColor = ColImage->GetColor(PointUp, UColor::MAGENTA);
			if (_Color.operator==(CheckColor))
			{
				return true;
			}
		}
		return false;
		break;
	case CheckDir::DOWN:
		for (float i = 0.0f; i <= (RightBottomPoint.X - LeftBottomPoint.X); i++)
		{
			FVector2D Point = { (RightBottomPoint.X - i), RightBottomPoint.Y };
			FVector2D PointDown = Point + FVector2D::DOWN;
			UColor CheckColor = ColImage->GetColor(PointDown, UColor::MAGENTA);
			if (_Color.operator==(CheckColor))
			{
				return true;
			}
		}
		return false;
		break;
	case CheckDir::LEFT:
		for (float i = 0; i <= (LeftBottomPoint.Y - LeftUpperPoint.Y); i++)
		{
			FVector2D Point = { LeftUpperPoint.X,(LeftBottomPoint.Y - i) };
			FVector2D PointLeft = Point + FVector2D::LEFT;
			UColor CheckColor = ColImage->GetColor(PointLeft, UColor::MAGENTA);
			if (_Color.operator==(CheckColor))
			{
				return true;
			}
		}
		return false;
		break;
	case CheckDir::RIGHT:
		for (float i = 0; i <= (RightBottomPoint.Y - RightUpperPoint.Y); i++)
		{
			FVector2D Point = { RightUpperPoint.X,(RightBottomPoint.Y - i) };
			FVector2D PointRight = Point + FVector2D::RIGHT;
			UColor CheckColor = ColImage->GetColor(PointRight, UColor::MAGENTA);
			if (_Color.operator==(CheckColor))
			{
				return true;
			}
		}
		return false;
		break;
	}
	return false;
}

void AMonster::CollisionEnter(AActor* _ColActor)
{
	if (nullptr != _ColActor)
	{
		SetMonsterState(MonsterState::HURT);
		return;
	}
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
	case MonsterState::ATTACKSTART:
		AttackStart(_DeltaTime);
		break;
	case MonsterState::ATTACK:
		Attack(_DeltaTime);
		break;
	case MonsterState::ATTACKEND:
		AttackEnd(_DeltaTime);
		break;
	case MonsterState::HURT:
		Hurt(_DeltaTime);
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
	if (!PixelLineCheck(CheckDir::DOWN, UColor::MAGENTA)
		&& !PixelLineCheck(CheckDir::DOWN, UColor::BLACK) && !PixelLineCheck(CheckDir::DOWN, UColor::YELLOW))
	{
		AddActorLocation(GravityForce * _DeltaTime);
		GravityForce += FVector2D::DOWN * 500.0f * _DeltaTime;
	}
	else
	{
		GravityForce = FVector2D::ZERO;
	}

}

void AMonster::Pause(float _DeltaTime)
{
}

void AMonster::Chase(float _DeltaTime)
{
	Gravity(_DeltaTime);

	if ("_Left" == AnimDir && !PixelLineCheck(CheckDir::LEFT, UColor::MAGENTA))
	{
		AddActorLocation(FVector2D::LEFT * Speed * _DeltaTime);
	}
	else
	{
		AnimDir = "_Right";
	}

	if ("_Right" == AnimDir && !PixelLineCheck(CheckDir::RIGHT, UColor::MAGENTA))
	{
		AddActorLocation(FVector2D::RIGHT * Speed * _DeltaTime);
	}
	else
	{
		AnimDir = "_Left";
	}

}

void AMonster::AttackStart(float _DeltaTime)
{
	SetMonsterState(MonsterState::ATTACK);
	return;
}

void AMonster::Attack(float _DeltaTime)
{
}

void AMonster::AttackEnd(float _DeltaTime)
{
}

void AMonster::Hurt(float _DeltaTime)
{
	AActor* HurtedActor = CollisionComponent->CollisionOnce(ECollisionGroup::PlayerSkill);
	if (nullptr != HurtedActor)
	{
		SetMonsterState(MonsterState::DIED);
		return;
	}
}

void AMonster::Inhale(float _DeltaTime)
{
	FVector2D Vector = FVector2D::ZERO;

	if ("_Right" == KirbyDir)
	{
		Vector = FVector2D::LEFT;
	}
	else
	{
		Vector = FVector2D::RIGHT;
	}
	AddActorLocation(Vector * 100.0f * _DeltaTime);

	AActor* InhaledActor = CollisionComponent->CollisionOnce(ECollisionGroup::InhaleBox);
	if (nullptr != InhaledActor)
	{
		SetMonsterState(MonsterState::INHALE);
		return;
	}


	AActor* Result = CollisionComponent->CollisionOnce(ECollisionGroup::InhaleBox);
	if (Result == nullptr)
	{
		SetMonsterState(MonsterState::PAUSE);
		return;
	}
}

void AMonster::Died(float _DeltaTime)
{
	SpriteRenderer->ChangeAnimation("Destroy");

	if (SpriteRenderer->IsCurAnimationEnd())
	{
		Destroy();
	}
}

