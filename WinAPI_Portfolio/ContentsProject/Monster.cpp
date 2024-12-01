#include "PreCompile.h"
#include "Monster.h"

#include <EnginePlatform/EngineInput.h>

#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/ImageManager.h>
#include <EngineCore/2DCollision.h>
#include <EngineCore/EngineCoreDebug.h>

#include "Player.h"
#include "PlayerState.h"

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
	AnimDir = (PlayerDistanceX <= 0) ? "_Left" : "_Right";	 // 게임 시작 시 방향
}

void AMonster::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	UEngineDebug::CoreOutPutString("State : " + std::to_string(static_cast<int>(GetMonsterState())));
	UEngineDebug::CoreOutPutString("AnimDir : " + AnimDir);
	MonsterFSM(_DeltaTime);
}

void AMonster::GetColImage(std::string _ColImageName)
{
	ColImage = UImageManager::GetInst().FindImage(_ColImageName);
}

void AMonster::SetMonster(std::string _SpriteName, FVector2D _SpriteScale, EAbilityType _AbilityType)
{
	SpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
	SpriteRenderer->SetSprite(_SpriteName);
	SpriteRenderer->SetComponentScale(_SpriteScale);
	MonsterScale = SpriteRenderer->GetTransform().Scale;
	MonsterAbility = _AbilityType;
}

void AMonster::SetCollision(FVector2D _CollisionScale)
{
	CollisionComponent = CreateDefaultSubObject<U2DCollision>();
	CollisionComponent->SetComponentLocation({ 0, 0 });
	CollisionComponent->SetComponentScale(_CollisionScale);
	CollisionComponent->SetCollisionGroup(ECollisionGroup::MONSTERBODY);
	CollisionComponent->SetCollisionType(ECollisionType::CirCle);
}

void AMonster::ChangeMonsterAnim(std::string _AnimName)
{
	SpriteRenderer->ChangeAnimation(_AnimName + AnimDir);
}

bool AMonster::MonsterAnimFinish()
{
	bool IsTrue = (true == SpriteRenderer->IsCurAnimationEnd()) ? true : false;
	return IsTrue;
}

bool AMonster::PixelPointCheck(ECheckDir _Dir, UColor _Color)
{
	FVector2D MonsterScale = SpriteRenderer->GetTransform().Scale;
	FVector2D Point = FVector2D::ZERO;
	FVector2D NextPoint = FVector2D::ZERO;
	switch (_Dir)
	{
	case ECheckDir::UP:
		Point = GetActorLocation() + FVector2D({ 0.0f, MonsterScale.Y * -0.5f });
		NextPoint = Point + FVector2D::UP;
		break;
	case ECheckDir::DOWN:
		 Point = GetActorLocation() + FVector2D({ 0.0f, MonsterScale.Y * 0.5f });
		NextPoint = Point + FVector2D::DOWN;
		break;
	case ECheckDir::LEFT:
		Point = GetActorLocation() + FVector2D({ MonsterScale.X * -0.5f, 0.0f });
		NextPoint = Point + FVector2D::LEFT;
		break;
	case ECheckDir::RIGHT:
		Point = GetActorLocation() + FVector2D({ MonsterScale.X * 0.5f, 0.0f });
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

void AMonster::MonsterFSM(float _DeltaTime)
{
	switch (CurState)
	{
	case EMonsterState::PAUSE:
		Pause(_DeltaTime);
		break;
	case EMonsterState::CHASE:
		Chase(_DeltaTime);
		break;
	case EMonsterState::ATTACKSTART:
		AttackStart(_DeltaTime);
		break;
	case EMonsterState::ATTACK:
		Attack(_DeltaTime);
		break;
	case EMonsterState::ATTACKEND:
		AttackEnd(_DeltaTime);
		break;
	case EMonsterState::INHALE:
		Inhale(_DeltaTime);
		break;
	case EMonsterState::DIE:
		Die(_DeltaTime);
		break;
	}
}

void AMonster::SetMonsterState(EMonsterState _State)
{
	CurState = _State;
}

void AMonster::SetDistance(FVector2D _PlayerLocation)
{
	PlayerDistanceX = GetActorLocation().X - _PlayerLocation.X;
	PlayerDistanceY = GetActorLocation().Y - _PlayerLocation.Y;
}

void AMonster::Gravity(float _DeltaTime)
{
	if (!PixelPointCheck(ECheckDir::DOWN, UColor::MAGENTA)
		&& !PixelPointCheck(ECheckDir::DOWN, UColor::BLACK) && !PixelPointCheck(ECheckDir::DOWN, UColor::YELLOW))
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
	Gravity(_DeltaTime);
	SetMonsterState(EMonsterState::CHASE);
	return;
}

void AMonster::Chase(float _DeltaTime)
{
	Gravity(_DeltaTime);

	if ("_Left" == AnimDir && !PixelPointCheck(ECheckDir::LEFT, UColor::MAGENTA))
	{
		AddActorLocation(FVector2D::LEFT * Speed * _DeltaTime);
	}
	else
	{
		AnimDir = "_Right";
	}

	if ("_Right" == AnimDir && !PixelPointCheck(ECheckDir::RIGHT, UColor::MAGENTA))
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
	SetMonsterState(EMonsterState::ATTACK);
	return;
}

void AMonster::Attack(float _DeltaTime)
{
}

void AMonster::AttackEnd(float _DeltaTime)
{
}

void AMonster::Inhale(float _DeltaTime)
{
	if (PlayerDistanceX >= 0)
	{
		AddActorLocation(FVector2D::LEFT * 100.0f * _DeltaTime);
	}
	else if (PlayerDistanceX < 0)
	{
		AddActorLocation(FVector2D::RIGHT * 100.0f * _DeltaTime);
	}

	AActor* Inhale = CollisionComponent->CollisionOnce(ECollisionGroup::PLAYERBODY);
	APlayer* InhalePlayer = dynamic_cast<APlayer*>(Inhale);
	if (nullptr != InhalePlayer)
	{
		APlayer::Score += 500;
		InhalePlayer->SetIsFull(true);
		InhalePlayer->SkillBoxCollisionSwitch(false);
		InhalePlayer->SetCurState(EPlayerState::IDLE);
		InhalePlayer->State->StopSound();
		InhalePlayer->SetCurMosnterAbility(MonsterAbility);
		SetActive(false);
	}

	AActor* Actor = CollisionComponent->CollisionOnce(ECollisionGroup::INHALEBOX);
	if (nullptr == Actor)
	{
		SetMonsterState(EMonsterState::PAUSE);
		return;
	}
}

void AMonster::Die(float _DeltaTime)
{
	SpriteRenderer->ChangeAnimation("Destroy");
	if (SpriteRenderer->IsCurAnimationEnd())
	{
		SetActive(false);
	}
}

