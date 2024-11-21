#include "PreCompile.h"
#include "Player.h"

#include <EngineCore/EngineAPICore.h>
#include <EngineCore/ImageManager.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/EngineCoreDebug.h>
#include <EngineCore/2DCollision.h>

#include <EnginePlatform/EngineInput.h>

#include "ContentsEnum.h"
#include "PlayerState.h"

#include "WindBullet.h"
#include "WaddleDee.h"

APlayer::APlayer()
{

	SetPlayer();
	SetAnimation();
	SetPlayerCollision();

	State = new PlayerState(this);

	DebugOn();
}

APlayer::~APlayer()
{
	delete State;
}

void APlayer::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->SetCameraToMainPawn(false);
}

void APlayer::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	UEngineDebug::CoreOutPutString("FPS : " + std::to_string(1.0f / _DeltaTime));
	UEngineDebug::CoreOutPutString("PlayerPos : " + GetActorLocation().ToString());
	UEngineDebug::CoreOutPutString("StateType : " + std::to_string(static_cast<int>(CurState)));
	UEngineDebug::CoreOutPutString("IsFull : " + std::to_string(IsFull));

	SetAnimDir();
	FSM(_DeltaTime);
	CameraMove();
}

void APlayer::GetBackImage(std::string_view _ImageName, std::string_view _ColImageName)
{
	BackImage = UImageManager::GetInst().FindImage(_ImageName);
	ColImage = UImageManager::GetInst().FindImage(_ColImageName);
}

bool APlayer::UpperPointCheck(UColor _Color)
{
	FVector2D PlayerScale = SpriteRenderer->GetTransform().Scale;

	FVector2D UpperPoint = GetActorLocation() + FVector2D({ 0.0f, PlayerScale.Y * -0.15f });
	FTransform UpperTransform = GetTransform();
	UpperTransform.Location += FVector2D({ 0.0f, PlayerScale.Y * -0.15f }) - GetWorld()->GetCameraPos();
	UpperTransform.Scale = { 6,6 };
	UEngineDebug::CoreDebugRender(UpperTransform, UEngineDebug::EDebugPosType::Circle);

	FVector2D NextUpperPoint = UpperPoint + FVector2D::UP;
	UColor UpperColor = ColImage->GetColor(NextUpperPoint, UColor::MAGENTA);
	if (_Color.operator==(UpperColor))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool APlayer::BottomPointCheck(UColor _Color)
{
	FVector2D PlayerScale = SpriteRenderer->GetTransform().Scale;

	FVector2D BottomPoint = GetActorLocation() + FVector2D({ 0.0f, PlayerScale.Y * 0.3f });
	FTransform BottomTransform = GetTransform();
	BottomTransform.Location += FVector2D({ 0.0f, PlayerScale.Y * 0.3f }) - GetWorld()->GetCameraPos();
	BottomTransform.Scale = { 6,6 };
	UEngineDebug::CoreDebugRender(BottomTransform, UEngineDebug::EDebugPosType::Circle);

	FVector2D NextBottomPoint = BottomPoint + FVector2D::DOWN;
	UColor BottomColor = ColImage->GetColor(NextBottomPoint, UColor::MAGENTA);
	if (_Color.operator==(BottomColor))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool APlayer::PixelLineColor(CheckDir _Dir, UColor _Color)
{
	FVector2D PlayerScale = SpriteRenderer->GetTransform().Scale;

	FVector2D LeftUpperPoint = GetActorLocation() + FVector2D({ PlayerScale.X * -0.25f, PlayerScale.Y * -0.15f });
	FTransform PlayerTransform0 = GetTransform();
	PlayerTransform0.Location += FVector2D({ PlayerScale.X * -0.25f, PlayerScale.Y * -0.15f }) - GetWorld()->GetCameraPos();
	PlayerTransform0.Scale = { 6,6 };
	UEngineDebug::CoreDebugRender(PlayerTransform0, UEngineDebug::EDebugPosType::Circle);

	FVector2D RightUpperPoint = GetActorLocation() + FVector2D({ PlayerScale.X * 0.25f, PlayerScale.Y * -0.15f });
	FTransform PlayerTransform1 = GetTransform();
	PlayerTransform1.Location += FVector2D({ PlayerScale.X * 0.25f, PlayerScale.Y * -0.15f }) - GetWorld()->GetCameraPos();
	PlayerTransform1.Scale = { 6,6 };
	UEngineDebug::CoreDebugRender(PlayerTransform1, UEngineDebug::EDebugPosType::Circle);

	FVector2D LeftBottomPoint = GetActorLocation() + FVector2D({ PlayerScale.X * -0.25f, PlayerScale.Y * 0.3f });
	FTransform PlayerTransform2 = GetTransform();
	PlayerTransform2.Location += FVector2D({ PlayerScale.X * -0.25f, PlayerScale.Y * 0.3f }) - GetWorld()->GetCameraPos();
	PlayerTransform2.Scale = { 6,6 };
	UEngineDebug::CoreDebugRender(PlayerTransform2, UEngineDebug::EDebugPosType::Circle);

	FVector2D RightBottomPoint = GetActorLocation() + FVector2D({ PlayerScale.X * 0.25f, PlayerScale.Y * 0.3f });
	FTransform PlayerTransform3 = GetTransform();
	PlayerTransform3.Location += FVector2D({ PlayerScale.X * 0.25f, PlayerScale.Y * 0.3f }) - GetWorld()->GetCameraPos();
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

void APlayer::CollisionEnter(AActor* _ColActor)
{
	if (CurState == EStateType::INHALESTART)
	{
		IsFull = true;
		_ColActor->Destroy();
	}
	else
	{
		dynamic_cast<AMonster*>(_ColActor)->SetMonsterState(MonsterState::DIED);
	}
}

void APlayer::CollisionStay(AActor* _ColActor)
{
	if (CurState == EStateType::INHALESTART)
	{
		dynamic_cast<AMonster*>(_ColActor)->SetKirbyDir(AnimDir);
		dynamic_cast<AMonster*>(_ColActor)->SetMonsterState(MonsterState::INHALE);
	}
}

void APlayer::SetPlayer()
{
	SpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
	SpriteRenderer->SetSprite("Kirby_Normal_Right.png");
	SpriteRenderer->SetComponentScale({ 94, 94 });
	SpriteRenderer->SetPivotType(PivotType::Bot);
	CurState = EStateType::IDLE;
}

void APlayer::SetAnimation()
{
	// Idle
	SpriteRenderer->CreateAnimation("Idle_Left", "Kirby_Normal_Left.png", 0, 1, 1.0f);
	SpriteRenderer->CreateAnimation("Idle_Right", "Kirby_Normal_Right.png", 0, 1, 1.0f);

	// Walk
	SpriteRenderer->CreateAnimation("Walk_Left", "Kirby_Normal_Left.png", 2, 5, 0.1f);
	SpriteRenderer->CreateAnimation("Walk_Right", "Kirby_Normal_Right.png", 2, 5, 0.1f);

	// Dash
	SpriteRenderer->CreateAnimation("Dash_Left", "Kirby_Normal_Left.png", 2, 5, 0.07f);
	SpriteRenderer->CreateAnimation("Dash_Right", "Kirby_Normal_Right.png", 2, 5, 0.07f);

	// Bend
	SpriteRenderer->CreateAnimation("Bend_Left", "Kirby_Normal_Left.png", 7, 7, 1.0f);
	SpriteRenderer->CreateAnimation("Bend_Right", "Kirby_Normal_Right.png", 7, 7, 1.0f);

	// Slide
	SpriteRenderer->CreateAnimation("Slide_Left", "Kirby_Normal_Left.png", 8, 8, 1.0f);
	SpriteRenderer->CreateAnimation("Slide_Right", "Kirby_Normal_Right.png", 8, 8, 1.0f);

	// FlyStart
	SpriteRenderer->CreateAnimation("FlyStart_Left", "Kirby_Normal_Left.png", 17, 21, 0.1f, false);
	SpriteRenderer->CreateAnimation("FlyStart_Right", "Kirby_Normal_Right.png", 17, 21, 0.1f, false);

	// Flying
	SpriteRenderer->CreateAnimation("Flying_Left", "Kirby_Normal_Left.png", 21, 22, 0.5f);
	SpriteRenderer->CreateAnimation("Flying_Right", "Kirby_Normal_Right.png", 21, 22, 0.5f);

	// FlyEnd
	SpriteRenderer->CreateAnimation("FlyEnd_Left", "Kirby_Normal_Left.png", 23, 26, 0.07f, false);
	SpriteRenderer->CreateAnimation("FlyEnd_Right", "Kirby_Normal_Right.png", 23, 26, 0.07f, false);

	// JumpStart
	SpriteRenderer->CreateAnimation("JumpStart_Left", "Kirby_Normal_Left.png", 11, 11, 1.0f);
	SpriteRenderer->CreateAnimation("JumpStart_Right", "Kirby_Normal_Right.png", 11, 11, 1.0f);

	// JumpEnd
	SpriteRenderer->CreateAnimation("JumpEnd_Left", "Kirby_Normal_Left.png", 11, 15, 0.07f, false);
	SpriteRenderer->CreateAnimation("JumpEnd_Right", "Kirby_Normal_Right.png", 11, 15, 0.07f, false);

	// Climb
	SpriteRenderer->CreateAnimation("ClimbUp_Left", "Kirby_Normal_Left.png", 68, 70, 0.2f);
	SpriteRenderer->CreateAnimation("ClimbUp_Right", "Kirby_Normal_Left.png", 68, 70, 0.2f);
	SpriteRenderer->CreateAnimation("ClimbDown_Left", "Kirby_Normal_Left.png", 62, 62, 1.0f);
	SpriteRenderer->CreateAnimation("ClimbDown_Right", "Kirby_Normal_Left.png", 62, 62, 1.0f);

	// Falling
	SpriteRenderer->CreateAnimation("Falling_Left", "Kirby_Normal_Left.png", 15, 15, 1.0f);
	SpriteRenderer->CreateAnimation("Falling_Right", "Kirby_Normal_Right.png", 15, 15, 1.0f);

	// FullIdle
	SpriteRenderer->CreateAnimation("IdleFull_Left", "Kirby_Normal_Left.png", 30, 30, 1.0f);
	SpriteRenderer->CreateAnimation("IdleFull_Right", "Kirby_Normal_Right.png", 30, 30, 1.0f);

	// FullWalk
	SpriteRenderer->CreateAnimation("WalkFull_Left", "Kirby_Normal_Left.png", 30, 32, 0.1f);
	SpriteRenderer->CreateAnimation("WalkFull_Right", "Kirby_Normal_Right.png", 30, 32, 0.1f);

	// FullJumpStart
	SpriteRenderer->CreateAnimation("JumpStartFull_Left", "Kirby_Normal_Left.png", { 31, 32, 31 }, 0.3f, false);
	SpriteRenderer->CreateAnimation("JumpStartFull_Right", "Kirby_Normal_Right.png", {31, 32, 31}, 0.3f, false);

	//FullJumpEnd
	SpriteRenderer->CreateAnimation("JumpEndFull_Left", "Kirby_Normal_Left.png", 31, 31, 0.1f);
	SpriteRenderer->CreateAnimation("JumpEndFull_Right", "Kirby_Normal_Right.png", 31, 31, 0.1f);

	// Attack_Inhale
	SpriteRenderer->CreateAnimation("InhaleStart_Left", "Kirby_Normal_Left.png", 17, 19, 0.05f, false);
	SpriteRenderer->CreateAnimation("InhaleStart_Right", "Kirby_Normal_Right.png", 17, 19, 0.05f, false);
	SpriteRenderer->CreateAnimation("InhaleEnd_Left", "Kirby_Normal_Left.png", 24, 26, 0.05f, false);
	SpriteRenderer->CreateAnimation("InhaleEnd_Right", "Kirby_Normal_Right.png", 24, 26, 0.05f, false);

	// Start Animation
	SpriteRenderer->ChangeAnimation("Idle_Right");
}

void APlayer::SetAnimDir()
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

void APlayer::SetPlayerCollision()
{
	FVector2D PlayerScale = SpriteRenderer->GetTransform().Scale;

	// PlayerBody Collision
	CollisionComponent = CreateDefaultSubObject<U2DCollision>();
	CollisionComponent->SetComponentLocation({ 3, -10 });
	CollisionComponent->SetComponentScale({ 64, 64 });
	CollisionComponent->SetCollisionGroup(ECollisionGroup::PlayerBody);
	CollisionComponent->SetCollisionType(ECollisionType::CirCle);
	CollisionComponent->SetCollisionEnter(std::bind(&APlayer::CollisionEnter, this, std::placeholders::_1));

	// Slide Left Collision
	SlideLeftCollision = CreateDefaultSubObject<U2DCollision>();
	SlideLeftCollision->SetComponentLocation({ PlayerScale.X * -0.35f, PlayerScale.Y * 0.2f });
	SlideLeftCollision->SetComponentScale({ 20, 50 });
	SlideLeftCollision->SetCollisionGroup(ECollisionGroup::PlayerSkill);
	SlideLeftCollision->SetCollisionType(ECollisionType::Rect);
	SlideLeftCollision->SetCollisionEnter(std::bind(&APlayer::CollisionEnter, this, std::placeholders::_1));
	SlideLeftCollision->SetActive(false);

	// Slide Right Collision
	SlideRightCollision = CreateDefaultSubObject<U2DCollision>();
	SlideRightCollision->SetComponentLocation({ PlayerScale.X * 0.35f, PlayerScale.Y * 0.2f });
	SlideRightCollision->SetComponentScale({ 20, 50 });
	SlideRightCollision->SetCollisionGroup(ECollisionGroup::PlayerSkill);
	SlideRightCollision->SetCollisionType(ECollisionType::Rect);
	SlideRightCollision->SetCollisionEnter(std::bind(&APlayer::CollisionEnter, this, std::placeholders::_1));
	SlideRightCollision->SetActive(false);
		
	// Inhale_Left Collision
	InhaleLeftCollision = CreateDefaultSubObject<U2DCollision>();
	InhaleLeftCollision->SetComponentLocation({ PlayerScale.X * -1.1f, 8.0f});
	InhaleLeftCollision->SetComponentScale({ 160, 50 });
	InhaleLeftCollision->SetCollisionGroup(ECollisionGroup::PlayerSkill);
	InhaleLeftCollision->SetCollisionType(ECollisionType::Rect);
	InhaleLeftCollision->SetCollisionStay(std::bind(&APlayer::CollisionStay, this, std::placeholders::_1));
	InhaleLeftCollision->SetActive(false);
	
	// Inhale_Right Collision
	InhaleRightCollision = CreateDefaultSubObject<U2DCollision>();
	InhaleRightCollision->SetComponentLocation({ PlayerScale.X * 1.1f, 8.0f });
	InhaleRightCollision->SetComponentScale({ 160, 50 });
	InhaleRightCollision->SetCollisionGroup(ECollisionGroup::PlayerSkill);
	InhaleRightCollision->SetCollisionType(ECollisionType::Rect);
	InhaleRightCollision->SetCollisionStay(std::bind(&APlayer::CollisionStay, this, std::placeholders::_1));
	InhaleRightCollision->SetActive(false);
	
	GetWorld()->CollisionGroupLink(ECollisionGroup::PlayerBody, ECollisionGroup::MonsterBody);
	GetWorld()->CollisionGroupLink(ECollisionGroup::PlayerSkill, ECollisionGroup::MonsterBody);
}

void APlayer::CameraMove()
{
	// 카메라 이동
	FVector2D Size = UEngineAPICore::GetCore()->GetMainWindow().GetWindowSize();
	FVector2D CurPos = GetActorLocation() - Size.Half();
	FVector2D CurPosRB = CurPos + Size;
	FVector2D BackImageSize = BackImage->GetImageScale();
	BackImageSize += FVector2D(0, 198);

	// 카메라 제한
	if (0.0f >= CurPos.X)
	{
		CurPos = { 0.0f, CurPos.Y };
	}

	if (0.0f >= CurPos.Y)
	{
		CurPos = { CurPos.X, 0.0f };
	}

	if (BackImageSize.X <= CurPosRB.X)
	{
		CurPosRB = { BackImageSize.X, CurPosRB.Y };
		FVector2D TempPos = CurPosRB - Size;
		CurPos = { TempPos.X ,CurPos.Y };
	}

	if (BackImageSize.Y <= CurPosRB.Y)
	{
		CurPosRB = { CurPosRB.X, BackImageSize.Y };
		FVector2D TempPos = CurPosRB - Size;
		CurPos = { CurPos.X, TempPos.Y };
	}

	GetWorld()->SetCameraPos(CurPos);
}

void APlayer::FSM(float _DeltaTime)
{
	switch (CurState)
	{
	case EStateType::IDLE:
		State->Idle(_DeltaTime);
		break;
	case EStateType::WALK:
		State->Walk(_DeltaTime);
		break;
	case EStateType::DASH:
		State->Dash(_DeltaTime);
		break;
	case EStateType::FLYSTART:
		State->FlyStart(_DeltaTime);
		break;
	case EStateType::FLYING:
		State->Flying(_DeltaTime);
		break;
	case EStateType::FLYEND:
		State->FlyEnd(_DeltaTime);
		break;
	case EStateType::JUMP:
		State->Jump(_DeltaTime);
		break;
	case EStateType::BEND:
		State->Bend(_DeltaTime);
		break;
	case EStateType::SLIDE:
		State->Slide(_DeltaTime);
		break;
	case EStateType::CLIMB:
		State->Climb(_DeltaTime);
		break;
	case EStateType::FALLING:
		State->Falling(_DeltaTime);
		break;
	case EStateType::INHALESTART:
		State->InhaleStart(_DeltaTime);
		break;
	case EStateType::INHALEEND:
		State->InhaleEnd(_DeltaTime);
		break;
	case EStateType::HURT:
		State->Hurt(_DeltaTime);
		break;
	default:
		break;
	}
}
