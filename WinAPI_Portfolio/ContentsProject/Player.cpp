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
		SpriteRenderer->SetComponentScale({ 94, 94 });

		// Idle
		SpriteRenderer->CreateAnimation("Idle_Left", "Kirby_Normal_Left.png", 0, 1, 1.0f);
		SpriteRenderer->CreateAnimation("Idle_Right", "Kirby_Normal_Right.png", 0, 1, 1.0f);

		// Walk
		SpriteRenderer->CreateAnimation("Walk_Left", "Kirby_Normal_Left.png", 2, 5, 0.1f);
		SpriteRenderer->CreateAnimation("Walk_Right", "Kirby_Normal_Right.png", 2, 5, 0.1f);

		// Bend
		SpriteRenderer->CreateAnimation("Bend_Left", "Kirby_Normal_Left.png", 7, 7, 1.0f);
		SpriteRenderer->CreateAnimation("Bend_Right", "Kirby_Normal_Right.png", 7, 7, 1.0f);
		 
		// Fly
		SpriteRenderer->CreateAnimation("Fly_Left", "Kirby_Normal_Left.png", 19, 22, 1.0f, false);
		SpriteRenderer->CreateAnimation("Fly_Right", "Kirby_Normal_Right.png", 19, 22, 1.0f, false);

		SpriteRenderer->ChangeAnimation("Idle_Right");
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
	case PlayerState::Bend:
		Bend(_DeltaTime);
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

	// Down
	FVector2D NextDownPos = GetActorLocation() + _MovePos;
	CheckColor[static_cast<int>(CheckDir::Down)] = ColImage->GetColor(NextDownPos, UColor::MAGENTA);
	
	// Top
	FVector2D NextUpPos = GetActorLocation() + FVector2D({ 0.0f, PlayerScale.Y * -0.5f }) + _MovePos;
	CheckColor[static_cast<int>(CheckDir::Up)] = ColImage->GetColor(NextUpPos, UColor::MAGENTA);
	
	FTransform PlayerTransform1 = GetTransform();
	PlayerTransform1.Location += FVector2D({ 0.0f, PlayerScale.Y * -0.5f }) - GetWorld()->GetCameraPos();
	PlayerTransform1.Scale = { 6,6 };
	UEngineDebug::CoreDebugRender(PlayerTransform1, UEngineDebug::EDebugPosType::Circle);

	// Left
	FVector2D NextLeftPos = GetActorLocation() + FVector2D({ PlayerScale.X * -0.25f, PlayerScale.Y * -0.25f }) + _MovePos;
	CheckColor[static_cast<int>(CheckDir::Left)] = ColImage->GetColor(NextLeftPos, UColor::MAGENTA);

	FTransform PlayerTransform2 = GetTransform();
	PlayerTransform2.Location += FVector2D({ PlayerScale.X * -0.25f, PlayerScale.Y * -0.25f }) - GetWorld()->GetCameraPos();
	PlayerTransform2.Scale = { 6,6 };
	UEngineDebug::CoreDebugRender(PlayerTransform2, UEngineDebug::EDebugPosType::Circle);

	// Right
	FVector2D NextRightPos = GetActorLocation() + FVector2D({ PlayerScale.X * 0.25f, PlayerScale.Y * -0.25f }) + _MovePos;
	CheckColor[static_cast<int>(CheckDir::Right)] = ColImage->GetColor(NextRightPos, UColor::MAGENTA);

	FTransform PlayerTransform3 = GetTransform();
	PlayerTransform3.Location += FVector2D({ PlayerScale.X * 0.25f, PlayerScale.Y * -0.25f }) - GetWorld()->GetCameraPos();
	PlayerTransform3.Scale = { 6,6 };
	UEngineDebug::CoreDebugRender(PlayerTransform3, UEngineDebug::EDebugPosType::Circle);
}

void APlayer::Gravity(float _DeltaTime)
{
	if (UColor::WHITE == CheckColor[static_cast<int>(CheckDir::Down)]
		|| UColor::BLUE == CheckColor[static_cast<int>(CheckDir::Down)])
	{
		AddActorLocation(GravityForce * _DeltaTime);
		GravityForce += FVector2D::DOWN * _DeltaTime * 100.0f;
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
	// 중력가속도를 기준으로 지면 체크
	PlayerGroundCheck(GravityForce * _DeltaTime);
	Gravity(_DeltaTime);

	// 방향 체크 및 애니메이션
	SetAnimDir();
	SpriteRenderer->ChangeAnimation("Idle" + AnimDir);

	// 키 입력 체크
	if (true == UEngineInput::GetInst().IsPress(VK_LEFT))
	{
		IsLeft = true;
		ChangeState(PlayerState::Move);
		return;
	}
	if (true == UEngineInput::GetInst().IsPress(VK_RIGHT))
	{
		IsLeft = false;
		ChangeState(PlayerState::Move);
		return;
	}
	if (true == UEngineInput::GetInst().IsPress(VK_DOWN))
	{
		ChangeState(PlayerState::Bend);
		return;
	}
}

void APlayer::Move(float _DeltaTime)
{
	// 중력가속도를 기준으로 지면 체크
	PlayerGroundCheck(GravityForce * _DeltaTime);
	Gravity(_DeltaTime);

	// 방향 체크 및 애니메이션
	SetAnimDir();
	SpriteRenderer->ChangeAnimation("Walk" + AnimDir);

	// 이동 방향 벡터 설정
	FVector2D Vector = FVector2D::ZERO;

	if (true == UEngineInput::GetInst().IsPress(VK_LEFT))
	{
		Vector += FVector2D::LEFT;
		UColor LeftColor = CheckColor[static_cast<int>(CheckDir::Left)];
		IsMAGENTA = CheckMAGENTA(LeftColor);
	}
	if (true == UEngineInput::GetInst().IsPress(VK_RIGHT))
	{
		Vector += FVector2D::RIGHT;
		UColor RightColor = CheckColor[static_cast<int>(CheckDir::Right)];
		IsMAGENTA = CheckMAGENTA(RightColor);
	}

	// 키 입력 체크
	if (false == UEngineInput::GetInst().IsPress(VK_LEFT)
		&& false == UEngineInput::GetInst().IsPress(VK_RIGHT))
	{
		ChangeState(PlayerState::Idle);
		return;
	}
	if (true == UEngineInput::GetInst().IsPress(VK_DOWN))
	{
		ChangeState(PlayerState::Bend);
		return;
	}
	
	// 이동할 위치 충돌 체크
	if (false == IsMAGENTA)
	{
		AddActorLocation(Vector * _DeltaTime * Speed);
	}
}

void APlayer::Fly(float _DeltaTime)
{
}

void APlayer::Jump(float _DeltaTime)
{
}

void APlayer::Bend(float _DeltaTime)
{
	SetAnimDir();
	SpriteRenderer->ChangeAnimation("Bend" + AnimDir);

	if (false == UEngineInput::GetInst().IsPress(VK_DOWN))
	{
		ChangeState(PlayerState::Idle);
		return;
	}
}
