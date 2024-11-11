#include "PreCompile.h"
#include "Player.h"

#include <EngineCore/EngineAPICore.h>
#include <EngineCore/ImageManager.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/EngineCoreDebug.h>

#include <EnginePlatform/EngineInput.h>


APlayer::APlayer()
{
	{
		SpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
		SpriteRenderer->SetSprite("Kirby_Normal_Right.png");
		SpriteRenderer->SetComponentScale({ 76, 76 });

		// Left
		SpriteRenderer->CreateAnimation("Walk_Left", "Kirby_Normal_Left.png", 2, 5, 0.1f);
		SpriteRenderer->CreateAnimation("Idle_Left", "Kirby_Normal_Left.png", 0, 1, 1.0f);

		// Right
		SpriteRenderer->CreateAnimation("Walk_Right", "Kirby_Normal_Right.png", 2, 5, 0.1f);
		SpriteRenderer->CreateAnimation("Idle_Right", "Kirby_Normal_Right.png", 0, 1, 1.0f);

		SpriteRenderer->ChangeAnimation("Idle_Right");
		SpriteRenderer->SetPivotType(PivotType::BOTTOM);
	}

	DebugOn();
}

APlayer::~APlayer()
{
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

	// FSM
	switch (CurPlayerState)
	{
	case PlayerState::Idle:
		Idle(_DeltaTime);
		break;
	case PlayerState::Move:
		Move(_DeltaTime);
		break;
	default:
		break;
	}

	CameraMove();
}

void APlayer::GetBackImage(std::string_view _ImageName, std::string_view _ColliderName)
{
	BackImage = UImageManager::GetInst().FindImage(_ImageName);
	ColImage = UImageManager::GetInst().FindImage(_ColliderName);
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

void APlayer::PlayerGroundCheck(FVector2D _MovePos)
{
	FVector2D PlayerScale = SpriteRenderer->GetTransform().Scale;

	// Bottom
	FVector2D NextDownPos = GetActorLocation() + _MovePos;
	CheckColor[static_cast<int>(CheckDir::Down)] = ColImage->GetColor(NextDownPos, UColor::MAGENTA);

	// Top
	FVector2D NextUpPos = GetActorLocation() + FVector2D({ 0.0f, PlayerScale.Y * -0.5f }) + _MovePos;
	CheckColor[static_cast<int>(CheckDir::Up)] = ColImage->GetColor(NextUpPos, UColor::MAGENTA);

	// LeftBottom
	FVector2D NextLeftPos = GetActorLocation() + FVector2D({ PlayerScale.X * -0.25f, 0.0f }) + _MovePos;
	CheckColor[static_cast<int>(CheckDir::Left)] = ColImage->GetColor(NextLeftPos, UColor::MAGENTA);

	// Right
	FVector2D NextRightPos = GetActorLocation() + FVector2D({ PlayerScale.X * 0.25f, 0.0f }) + _MovePos;
	CheckColor[static_cast<int>(CheckDir::Right)] = ColImage->GetColor(NextRightPos, UColor::MAGENTA);
}

void APlayer::Gravity(float _DeltaTime)
{
	if (UColor::WHITE == CheckColor[static_cast<int>(CheckDir::Down)])
	{
		AddActorLocation(GravityForce * _DeltaTime);
		GravityForce += FVector2D::DOWN * _DeltaTime * 500.0f;
	}	
	else
	{
		GravityForce = FVector2D::ZERO;
	}
}

void APlayer::ChangeState(PlayerState _CurPlayerState)
{
	CurPlayerState = _CurPlayerState;
}

void APlayer::Idle(float _DeltaTime)
{
	FVector2D Pos = GetActorLocation();

	PlayerGroundCheck(GravityForce * _DeltaTime);
	Gravity(_DeltaTime);

	SpriteRenderer->ChangeAnimation("Idle_Right");

	if (true == UEngineInput::GetInst().IsPress(VK_RIGHT) ||
		true == UEngineInput::GetInst().IsPress(VK_LEFT) ||
		true == UEngineInput::GetInst().IsPress(VK_DOWN) ||
		true == UEngineInput::GetInst().IsPress(VK_UP))
	{
		ChangeState(PlayerState::Move);
		return;
	}
}

void APlayer::Move(float _DeltaTime)
{
	PlayerGroundCheck(GravityForce * _DeltaTime);
	Gravity(_DeltaTime);

	FVector2D Vector = FVector2D::ZERO;

	if (true == UEngineInput::GetInst().IsPress(VK_RIGHT))
	{
		SpriteRenderer->ChangeAnimation("Walk_Right");
		Vector += FVector2D::RIGHT;
	}
	if (true == UEngineInput::GetInst().IsPress(VK_LEFT))
	{
		SpriteRenderer->ChangeAnimation("Walk_Left");
		Vector += FVector2D::LEFT;
	}
	if (true == UEngineInput::GetInst().IsPress(VK_DOWN))
	{
		Vector += FVector2D::DOWN;
	}
	if (true == UEngineInput::GetInst().IsPress(VK_UP))
	{
		Vector += FVector2D::UP;
	}

	if (false == UEngineInput::GetInst().IsPress(VK_RIGHT) &&
		false == UEngineInput::GetInst().IsPress(VK_LEFT) &&
		false == UEngineInput::GetInst().IsPress(VK_DOWN) &&
		false == UEngineInput::GetInst().IsPress(VK_UP))
	{
		ChangeState(PlayerState::Idle);
		return;
	}

	Vector.Normalize();

	PlayerGroundCheck(Vector * _DeltaTime * Speed);

	// White 는 통과
	// Black은 조건부 충돌
	// Magenta는 충돌
	// Yellow는 사다리
	// Red 는 포탈
	// Green은 함정

	//// 공중일 때
	//if (UColor::WHITE == CheckColor[static_cast<int>(CheckDir::Down)])
	//{
	//	// 점프 힘
	//	AddActorLocation(Vector * _DeltaTime * Jump);
	//}
	//else
	//{
	//	AddActorLocation(Vector * _DeltaTime * Speed);
	//}
}