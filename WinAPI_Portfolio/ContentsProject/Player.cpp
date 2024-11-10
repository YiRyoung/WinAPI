#include "PreCompile.h"
#include "Player.h"

#include <EngineCore/EngineAPICore.h>
#include <EngineCore/ImageManager.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/EngineCoreDebug.h>

#include <EnginePlatform/EngineInput.h>


APlayer::APlayer()
{
	SetActorLocation({ 100, 100 });

	{
		SpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
		SpriteRenderer->SetSprite("Kirby_Normal_Right.png");
		SpriteRenderer->SetComponentScale({ 96, 96 });

		// 왼쪽
		SpriteRenderer->CreateAnimation("Walk_Left", "Kirby_Normal_Left.png", 2, 5, 0.1f);
		SpriteRenderer->CreateAnimation("Idle_Left", "Kirby_Normal_Left.png", 0, 1, 1.0f);

		// 오른쪽
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

	// 이동
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

void APlayer::ChangeState(PlayerState _CurPlayerState)
{
	CurPlayerState = _CurPlayerState;
}

void APlayer::Idle(float _DeltaTime)
{
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

	// 다음 이동할 위치 및 색상
	// 플레이어의 중심을 기준으로 NextPos가 구해졌으므로 피벗을 변경해주어야 함
	// 현재 Bottom을 기준으로 검사했으므로 나머지 방향 또한 검사를 추가해야함.

	FVector2D PlayerScale = SpriteRenderer->GetTransform().Scale;

	// Bottom
	FVector2D NextDownPos = GetActorLocation() + (FVector2D::DOWN * 2.5f)+ Vector * _DeltaTime * Speed;
	UColor DownColor = ColImage->GetColor(NextDownPos, UColor::MAGENTA);

	// Top
	FVector2D NextUpPos = GetActorLocation() + FVector2D{ 0.0f, -(PlayerScale.Y * 0.5f) } + Vector * _DeltaTime * Speed;
	UColor UpColor = ColImage->GetColor(NextUpPos, UColor::MAGENTA);

	// LeftBottom
	FVector2D NextLeftBottomPos = NextDownPos + FVector2D(-PlayerScale.X * 0.25f, 0.0f) + Vector * _DeltaTime * Speed;
	UColor LeftColor = ColImage->GetColor(NextLeftBottomPos, UColor::MAGENTA);

	// Right
	FVector2D NextRightPos = GetActorLocation() + FVector2D{ (PlayerScale.X * 0.25f), 0.0f } + Vector * _DeltaTime * Speed;
	UColor RightColor = ColImage->GetColor(NextRightPos, UColor::MAGENTA);

	Vector.Normalize();
	if (UColor::MAGENTA != DownColor && UColor::MAGENTA != UpColor && UColor::MAGENTA != LeftColor && UColor::MAGENTA != RightColor)
	{
		AddActorLocation(Vector * _DeltaTime * Speed);


		while (true)
		{
			NextDownPos = GetActorLocation() + (FVector2D::DOWN * 2.5f) + Vector * _DeltaTime * Speed;
			DownColor = ColImage->GetColor(NextDownPos, UColor::MAGENTA);

			NextLeftBottomPos = NextDownPos + FVector2D(-PlayerScale.X * 0.25f, 0.0f) + Vector * _DeltaTime * Speed;
			LeftColor = ColImage->GetColor(NextLeftBottomPos, UColor::MAGENTA);

			NextRightPos = GetActorLocation() + FVector2D{ (PlayerScale.X * 0.25f), 0.0f } + Vector * _DeltaTime * Speed;
			RightColor = ColImage->GetColor(NextRightPos, UColor::MAGENTA);
			
			if (UColor::YELLOW == DownColor || UColor::CYAN == DownColor
				|| UColor::YELLOW == LeftColor || UColor::CYAN == LeftColor 
				|| UColor::YELLOW == RightColor || UColor::CYAN == RightColor)
			{
				AddActorLocation(FVector2D::UP * _DeltaTime * Speed);
			}
			else
			{
				break;
			}
		}
	}
	// WHITE, RED, GREEN, CYAN은 통과
	// MAGENTA는 충돌
	// BLACK은 조건부 통과
}
