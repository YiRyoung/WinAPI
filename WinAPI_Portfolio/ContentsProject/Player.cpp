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
#include "PlayerAbility.h"

#include "WaddleDee.h"

APlayer::APlayer()
{

	SetPlayer();
	SetAnimation();
	SetPlayerCollision();

	State = new PlayerState(this);
	Ability = new PlayerAbility(this);

	DebugOn();
}

APlayer::~APlayer()
{
	delete State;
	delete Ability;
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
}

void APlayer::CollisionEnter(AActor* _ColActor)
{
	if (_ColActor != nullptr)
	{
		dynamic_cast<AWaddleDee*>(_ColActor)->SetDestory();
	}
}

void APlayer::CollisionStay(AActor* _ColActor)
{
	if (_ColActor != nullptr && InhaleLeftCollision->CollisionOnce(ECollisionGroup::MonsterBody))
	{
		dynamic_cast<AWaddleDee*>(_ColActor)->AddActorLocation(FVector2D::LEFT * 10.0f);
	}
}

void APlayer::SetPlayer()
{
	SpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
	SpriteRenderer->SetSprite("Kirby_Normal_Right.png");
	SpriteRenderer->SetComponentScale({ 94, 94 });
	SpriteRenderer->SetPivotType(PivotType::Bot);
	CurState = StateType::IDLE;
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

	// Attack_InhaleStart
	SpriteRenderer->CreateAnimation("InhaleStart_Left", "Kirby_Normal_Left.png", 17, 19, 0.05f, false);
	SpriteRenderer->CreateAnimation("InhaleStart_Right", "Kirby_Normal_Right.png", 17, 19, 0.05f, false);

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
	GetWorld()->CollisionGroupLink(ECollisionGroup::PlayerBody, ECollisionGroup::MonsterBody);
	CollisionComponent->SetCollisionEnter(std::bind(&APlayer::CollisionEnter, this, std::placeholders::_1));

	// Inhale_Left Collision
	InhaleLeftCollision = CreateDefaultSubObject<U2DCollision>();
	InhaleLeftCollision->SetComponentLocation({ PlayerScale.X * -1.1f, 8.0f});
	InhaleLeftCollision->SetComponentScale({ 160, 50 });
	InhaleLeftCollision->SetCollisionGroup(ECollisionGroup::PlayerSkill);
	InhaleLeftCollision->SetCollisionType(ECollisionType::Rect);
	GetWorld()->CollisionGroupLink(ECollisionGroup::PlayerSkill, ECollisionGroup::MonsterBody);
	InhaleLeftCollision->SetCollisionStay(std::bind(&APlayer::CollisionStay, this, std::placeholders::_1));
	InhaleLeftCollision->SetActive(false);
	
	// Inhale_Right Collision
	InhaleRightCollision = CreateDefaultSubObject<U2DCollision>();
	InhaleRightCollision->SetComponentLocation({ PlayerScale.X * 1.1f, 8.0f });
	InhaleRightCollision->SetComponentScale({ 160, 50 });
	InhaleRightCollision->SetCollisionGroup(ECollisionGroup::PlayerSkill);
	InhaleRightCollision->SetCollisionType(ECollisionType::Rect);
	GetWorld()->CollisionGroupLink(ECollisionGroup::PlayerSkill, ECollisionGroup::MonsterBody);
	InhaleRightCollision->SetCollisionStay(std::bind(&APlayer::CollisionStay, this, std::placeholders::_1));
	InhaleRightCollision->SetActive(false);

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
	case StateType::IDLE:
		State->Idle(_DeltaTime);
		break;
	case StateType::WALK:
		State->Walk(_DeltaTime);
		break;
	case StateType::DASH:
		State->Dash(_DeltaTime);
		break;
	case StateType::FLYSTART:
		State->FlyStart(_DeltaTime);
		break;
	case StateType::FLYING:
		State->Flying(_DeltaTime);
		break;
	case StateType::FLYEND:
		State->FlyEnd(_DeltaTime);
		break;
	case StateType::JUMP:
		State->Jump(_DeltaTime);
		break;
	case StateType::BEND:
		State->Bend(_DeltaTime);
		break;
	case StateType::SLIDE:
		State->Slide(_DeltaTime);
		break;
	case StateType::CLIMB:
		State->Climb(_DeltaTime);
		break;
	case StateType::FALLING:
		State->Falling(_DeltaTime);
		break;
	case StateType::HURT:
		State->Hurt(_DeltaTime);
		break;
	case StateType::ATTACK:
		Ability->Attack(_DeltaTime);
		break;
	default:
		break;
	}
}
