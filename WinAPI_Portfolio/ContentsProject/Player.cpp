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

		SetAnim();

		SpriteRenderer->ChangeAnimation("Idle_Right");
		SpriteRenderer->SetPivotType(PivotType::Bot);
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

	//BGM = UEngineSound::Play("Vegetable Valley 2.mp3");
}

void APlayer::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	UEngineDebug::CoreOutPutString("FPS : " + std::to_string(1.0f / _DeltaTime));
	UEngineDebug::CoreOutPutString("PlayerPos : " + GetActorLocation().ToString());
	UEngineDebug::CoreOutPutString("DirAcc : " + DirForce.ToString());

	// 좌우 방향 체크
	SetAnimDir();

	// PlayerState FSM
	switch (CurPlayerState)
	{
	case PlayerState::Idle:
		Idle(_DeltaTime);
		break;
	case PlayerState::Move:
		Move(_DeltaTime);
		break;
	case PlayerState::Dash:
		Dash(_DeltaTime);
		break;
	case PlayerState::Bend:
		Bend(_DeltaTime);
		break;
	case PlayerState::Slide:
		Slide(_DeltaTime);
		break;
	case PlayerState::Jump:
		Jump(_DeltaTime);
		break;
	case PlayerState::FlyStart:
		FlyStart(_DeltaTime);
		break;
	case PlayerState::Flying:
		Flying(_DeltaTime);
		break;
	case PlayerState::FlyEnd:
		FlyEnd(_DeltaTime);
		break;
	case PlayerState::Climb:
		Climb(_DeltaTime);
		break;
	case PlayerState::Fall:
		Fall(_DeltaTime);
		break;
	default:
		break;
	}

	CameraMove();
}

void APlayer::SetAnim()
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
	SpriteRenderer->CreateAnimation("FlyStart_Left", "Kirby_Normal_Left.png", 17, 21, 0.07f, false);
	SpriteRenderer->CreateAnimation("FlyStart_Right", "Kirby_Normal_Right.png", 17, 21, 0.07f, false);

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
	SpriteRenderer->CreateAnimation("ClimbUp", "Kirby_Normal_Left.png", 68, 70, 0.2f);
	SpriteRenderer->CreateAnimation("ClimbDown", "Kirby_Normal_Left.png", 62, 62, 1.0f);

	// Fall
	SpriteRenderer->CreateAnimation("Falling_Left", "Kirby_Normal_Left.png", 15, 15, 1.0f);
	SpriteRenderer->CreateAnimation("Falling_Right", "Kirby_Normal_Right.png", 15, 15, 1.0f);

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
	FVector2D NextDownPos = GetActorLocation() + FVector2D({ 0.0f, PlayerScale.Y * 0.35f }) + _MovePos;
	CheckColor[static_cast<int>(CheckDir::Down)] = ColImage->GetColor(NextDownPos, UColor::MAGENTA);
	
	FTransform PlayerTransform = GetTransform();
	PlayerTransform.Location += FVector2D({ 0.0f, PlayerScale.Y * 0.35f }) - GetWorld()->GetCameraPos();
	PlayerTransform.Scale = { 6,6 };
	UEngineDebug::CoreDebugRender(PlayerTransform, UEngineDebug::EDebugPosType::Circle);
	
	// Top
	FVector2D NextUpPos = GetActorLocation() + FVector2D({ 0.0f, PlayerScale.Y * -0.2f }) + _MovePos;
	CheckColor[static_cast<int>(CheckDir::Up)] = ColImage->GetColor(NextUpPos, UColor::MAGENTA);
	
	FTransform PlayerTransform1 = GetTransform();
	PlayerTransform1.Location += FVector2D({ 0.0f, PlayerScale.Y * -0.2f }) - GetWorld()->GetCameraPos();
	PlayerTransform1.Scale = { 6,6 };
	UEngineDebug::CoreDebugRender(PlayerTransform1, UEngineDebug::EDebugPosType::Circle);

	// LeftDowm
	FVector2D NextLeftDownPos = GetActorLocation() + FVector2D({ PlayerScale.X * -0.25f,  PlayerScale.Y * 0.35f }) + _MovePos;
	CheckColor[static_cast<int>(CheckDir::LeftDown)] = ColImage->GetColor(NextLeftDownPos, UColor::MAGENTA);

	FTransform PlayerTransform2 = GetTransform();
	PlayerTransform2.Location += FVector2D({ PlayerScale.X * -0.25f, PlayerScale.Y * 0.35f }) - GetWorld()->GetCameraPos();
	PlayerTransform2.Scale = { 6,6 };
	UEngineDebug::CoreDebugRender(PlayerTransform2, UEngineDebug::EDebugPosType::Circle);

	// RightDown
	FVector2D NextRightDownPos = GetActorLocation() + FVector2D({ PlayerScale.X * 0.25f,  PlayerScale.Y * 0.35f }) + _MovePos;
	CheckColor[static_cast<int>(CheckDir::RightDown)] = ColImage->GetColor(NextRightDownPos, UColor::MAGENTA);

	FTransform PlayerTransform3 = GetTransform();
	PlayerTransform3.Location += FVector2D({ PlayerScale.X * 0.25f,  PlayerScale.Y * 0.35f }) - GetWorld()->GetCameraPos();
	PlayerTransform3.Scale = { 6,6 };
	UEngineDebug::CoreDebugRender(PlayerTransform3, UEngineDebug::EDebugPosType::Circle);

	// LeftUp
	FVector2D NextLeftUpPos = GetActorLocation() + FVector2D({ PlayerScale.X * -0.25f,  PlayerScale.Y * -0.2f }) + _MovePos;
	CheckColor[static_cast<int>(CheckDir::LeftUp)] = ColImage->GetColor(NextLeftUpPos, UColor::MAGENTA);

	FTransform PlayerTransform4 = GetTransform();
	PlayerTransform4.Location += FVector2D({ PlayerScale.X * -0.25f, PlayerScale.Y * -0.2f }) - GetWorld()->GetCameraPos();
	PlayerTransform4.Scale = { 6,6 };
	UEngineDebug::CoreDebugRender(PlayerTransform4, UEngineDebug::EDebugPosType::Circle);

	// RightUp
	FVector2D NextRightUpPos = GetActorLocation() + FVector2D({ PlayerScale.X * 0.25f,  PlayerScale.Y * -0.2f }) + _MovePos;
	CheckColor[static_cast<int>(CheckDir::RightUp)] = ColImage->GetColor(NextRightUpPos, UColor::MAGENTA);

	FTransform PlayerTransform5 = GetTransform();
	PlayerTransform5.Location += FVector2D({ PlayerScale.X * 0.25f,  PlayerScale.Y * -0.2f }) - GetWorld()->GetCameraPos();
	PlayerTransform5.Scale = { 6,6 };
	UEngineDebug::CoreDebugRender(PlayerTransform5, UEngineDebug::EDebugPosType::Circle);

	// Left
	FVector2D NextLeftPos = GetActorLocation() + FVector2D({ PlayerScale.X * -0.25f,  PlayerScale.Y * 0.1f }) + _MovePos;
	CheckColor[static_cast<int>(CheckDir::LeftDown)] = ColImage->GetColor(NextLeftPos, UColor::MAGENTA);

	FTransform PlayerTransform6 = GetTransform();
	PlayerTransform6.Location += FVector2D({ PlayerScale.X * -0.25f,  PlayerScale.Y * 0.1f }) - GetWorld()->GetCameraPos();
	PlayerTransform6.Scale = { 6,6 };
	UEngineDebug::CoreDebugRender(PlayerTransform6, UEngineDebug::EDebugPosType::Circle);

	// Right
	FVector2D NextRightPos = GetActorLocation() + FVector2D({ PlayerScale.X * 0.25f,  PlayerScale.Y * 0.1f }) + _MovePos;
	CheckColor[static_cast<int>(CheckDir::RightDown)] = ColImage->GetColor(NextRightPos, UColor::MAGENTA);

	FTransform PlayerTransform7 = GetTransform();
	PlayerTransform7.Location += FVector2D({ PlayerScale.X * 0.25f,  PlayerScale.Y * 0.1f }) - GetWorld()->GetCameraPos();
	PlayerTransform7.Scale = { 6,6 };
	UEngineDebug::CoreDebugRender(PlayerTransform7, UEngineDebug::EDebugPosType::Circle);
}

void APlayer::Gravity(float _DeltaTime)
{
	UColor DownColor = CheckColor[static_cast<int>(CheckDir::Down)];

	if (UColor::WHITE == DownColor || UColor::RED == DownColor)
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
	SpriteRenderer->ChangeAnimation("Idle" + AnimDir);
	
	// 중력가속도를 기준으로 지면 체크
	PlayerGroundCheck(GravityForce * _DeltaTime);
	Gravity(_DeltaTime);

	// 키 입력 체크
	PlayerGroundCheck(FVector2D::ZERO * _DeltaTime);
	UColor DownColor = CheckColor[static_cast<int>(CheckDir::Down)];
	UColor UpColor = CheckColor[static_cast<int>(CheckDir::Up)];

	// Move & Dash
	if (IsDoubleKey(VK_LEFT, 0.2f) || IsDoubleKey(VK_RIGHT, 0.2f))
	{
		ChangeState(PlayerState::Dash);
		return;
	}
	else if (IsPressKey(VK_LEFT) || IsPressKey(VK_RIGHT))
	{
		ChangeState(PlayerState::Move);
		return;
	}

	// Jump
	if (IsPressKey('Z') && !CheckState(PlayerState::Bend))
	{
		ChangeState(PlayerState::Jump);
		return;
	}

	// Fly
	if (IsPressKey(VK_UP) && !CheckYELLOW(UpColor))
	{
		ChangeState(PlayerState::FlyStart);
		return;
	}

	// Bend
	if (IsPressKey(VK_DOWN) && !CheckYELLOW(DownColor))
	{
		ChangeState(PlayerState::Bend);
		return;
	}

	// Climb
	if ((IsPressKey(VK_UP) && CheckYELLOW(UpColor)) 
		|| (IsPressKey(VK_DOWN) && CheckYELLOW(DownColor)))
	{
		ChangeState(PlayerState::Climb);
		return;
	}

	// 가속도 감속
	PlayerGroundCheck(DirForce * _DeltaTime);
	UColor LeftColor = CheckColor[static_cast<int>(CheckDir::LeftDown)];
	UColor RightColor = CheckColor[static_cast<int>(CheckDir::RightDown)];

	if (!CheckMAGENTA(LeftColor) && !CheckMAGENTA(RightColor))
	{

		DirForce.X += -DirForce.X * DeAccSpeed * _DeltaTime;
		SetLimitSpeed(2);
		AddActorLocation(DirForce * _DeltaTime);
	}
}

void APlayer::Move(float _DeltaTime)
{
	SpriteRenderer->ChangeAnimation("Walk" + AnimDir);

	// 중력 가속도를 기준으로 지면 체크
	PlayerGroundCheck(GravityForce * _DeltaTime);
	Gravity(_DeltaTime);

	// 키 입력 체크
	PlayerGroundCheck(FVector2D::ZERO * _DeltaTime);
	UColor DownColor = CheckColor[static_cast<int>(CheckDir::Down)];
	UColor UpColor = CheckColor[static_cast<int>(CheckDir::Up)];

	// Fly
	if (IsPressKey(VK_UP) && !CheckYELLOW(UpColor))
	{
		ChangeState(PlayerState::FlyStart);
		return;
	}

	// Bend
	if (IsPressKey(VK_DOWN) && !CheckYELLOW(DownColor))
	{
		ChangeState(PlayerState::Bend);
		return;
	}

	// Climb
	if ((IsPressKey(VK_UP) && CheckYELLOW(UpColor))
		|| (IsPressKey(VK_DOWN) && CheckYELLOW(DownColor)))
	{
		ChangeState(PlayerState::Climb);
		return;
	}

	// Jump
	if (IsPressKey('Z') && !CheckState(PlayerState::Bend))
	{
		ChangeState(PlayerState::Jump);
		return;
	}

	// Idle
	if (!IsPressKey(VK_LEFT) && !IsPressKey(VK_RIGHT))
	{
		ChangeState(PlayerState::Idle);
		return;
	}

	// Moving
	if (IsPressKey(VK_LEFT))
	{
		PlayerGroundCheck(FVector2D::LEFT * Speed * _DeltaTime);
		UColor LeftColor = CheckColor[static_cast<int>(CheckDir::LeftDown)];
		
		if (!CheckMAGENTA(LeftColor))
		{
			AddActorLocation(FVector2D::LEFT * Speed * _DeltaTime);
		}
	}

	if (IsPressKey(VK_RIGHT))
	{
		PlayerGroundCheck(FVector2D::RIGHT * Speed * _DeltaTime);
		UColor RightColor = CheckColor[static_cast<int>(CheckDir::RightDown)];

		if (!CheckMAGENTA(RightColor))
		{
			AddActorLocation(FVector2D::RIGHT * Speed * _DeltaTime);
		}
	}
}

void APlayer::Dash(float _DeltaTime)
{
	SpriteRenderer->ChangeAnimation("Dash" + AnimDir);
	
	PlayerGroundCheck(GravityForce * _DeltaTime);
	Gravity(_DeltaTime);

	// 키 입력 체크
	PlayerGroundCheck(FVector2D::ZERO * _DeltaTime);
	UColor DownColor = CheckColor[static_cast<int>(CheckDir::Down)];
	UColor UpColor = CheckColor[static_cast<int>(CheckDir::Up)];

	// Fly
	if (IsPressKey(VK_UP) && !CheckYELLOW(UpColor))
	{
		ChangeState(PlayerState::FlyStart);
		return;
	}

	// Bend
	if (IsPressKey(VK_DOWN) && !CheckYELLOW(DownColor))
	{
		ChangeState(PlayerState::Bend);
		return;
	}

	// Climb
	if ((IsPressKey(VK_UP) && CheckYELLOW(UpColor))
		|| (IsPressKey(VK_DOWN) && CheckYELLOW(DownColor)))
	{
		ChangeState(PlayerState::Climb);
		return;
	}

	// Jump
	if (IsPressKey('Z') && !CheckState(PlayerState::Bend))
	{
		ChangeState(PlayerState::Jump);
		return;
	}

	// Idle
	if (!IsPressKey(VK_LEFT) && !IsPressKey(VK_RIGHT))
	{
		ChangeState(PlayerState::Idle);
		return;
	}

	// Dashing

	if (IsPressKey(VK_LEFT))
	{
		DirForce += FVector2D::LEFT * _DeltaTime * AccSpeed;
		SetLimitSpeed(1);

		PlayerGroundCheck(DirForce * _DeltaTime);
		UColor LeftColor = CheckColor[static_cast<int>(CheckDir::LeftDown)];

		if (!CheckMAGENTA(LeftColor))
		{
			AddActorLocation(DirForce * _DeltaTime);
		}
		else
		{
			DirForce = FVector2D::ZERO;
		}
	}
	if (IsPressKey(VK_RIGHT))
	{
		DirForce += FVector2D::RIGHT * _DeltaTime * AccSpeed;
		SetLimitSpeed(1);
		
		PlayerGroundCheck(DirForce * _DeltaTime);
		UColor RightColor = CheckColor[static_cast<int>(CheckDir::RightDown)];

		if (!CheckMAGENTA(RightColor))
		{
			AddActorLocation(DirForce * _DeltaTime);
		}
		else
		{
			DirForce = FVector2D::ZERO;
		}
	}
}

void APlayer::Jump(float _DeltaTime)
{
	float Force = JumpForce - GravityForce.Y;

	if (0.0f < Force)
	{
		SpriteRenderer->ChangeAnimation("JumpStart" + AnimDir);
	}
	else
	{
		SpriteRenderer->ChangeAnimation("JumpEnd" + AnimDir);
	}

	// 중력 체크
	PlayerGroundCheck(GravityForce * _DeltaTime);
	Gravity(_DeltaTime);

	// 키 입력 체크
	if (IsPressKey(VK_UP))
	{
		ChangeState(PlayerState::Flying);
		return;
	}

	// 이동 방향 벡터 설정
	DirForce += FVector2D::UP;
	if (IsPressKey(VK_LEFT))
	{
		DirForce += FVector2D::LEFT * _DeltaTime * AccSpeed;
		SetLimitSpeed(1);
	}
	if (IsPressKey(VK_RIGHT))
	{
		DirForce += FVector2D::RIGHT * _DeltaTime * AccSpeed;
		SetLimitSpeed(1);
	}
	DirForce.Normalize();
	AddActorLocation(DirForce * JumpForce * _DeltaTime);

	UColor UpColor = CheckColor[static_cast<int>(CheckDir::Up)];
	UColor DownColor = CheckColor[static_cast<int>(CheckDir::Down)];

	if (CheckMAGENTA(DownColor) || CheckMAGENTA(UpColor) || CheckBLACK(DownColor))
	{
		ChangeState(PlayerState::Idle);
		return;
	}
}

void APlayer::FlyStart(float _DeltaTime)
{
	SpriteRenderer->ChangeAnimation("FlyStart" + AnimDir);

	// 이동
	FVector2D Vector = FVector2D::ZERO;

	if (IsPressKey(VK_UP))
	{
		GravityForce = FVector2D::ZERO;
		Vector += FVector2D::UP;
	}
	if (IsPressKey(VK_LEFT))
	{
		Vector += FVector2D::LEFT;
	}
	if (IsPressKey(VK_RIGHT))
	{
		Vector += FVector2D::RIGHT;
	}
	Vector.Normalize();

	PlayerGroundCheck(FVector2D::ZERO * _DeltaTime);
	UColor UpColor = CheckColor[static_cast<int>(CheckDir::Up)];
	UColor DownColor = CheckColor[static_cast<int>(CheckDir::Down)];

	if (!CheckMAGENTA(UpColor) && !CheckMAGENTA(DownColor))
	{
		AddActorLocation(Vector * Speed * _DeltaTime);
	}

	// 애니메이션 종료 시 행동 변경
	if (true == SpriteRenderer->IsCurAnimationEnd())
	{
		ChangeState(PlayerState::Flying);
		return;
	}

}

void APlayer::Flying(float _DeltaTime)
{
	SpriteRenderer->ChangeAnimation("Flying" + AnimDir);

	// 키 입력 체크
	if (IsPressKey('X'))
	{
		ChangeState(PlayerState::FlyEnd);
		return;
	}

	// 이동
	FVector2D Vector = FVector2D::ZERO;

	if (IsPressKey(VK_UP))
	{
		GravityForce = FVector2D::ZERO;
		SpriteRenderer->SetAnimationSpeed(5.0f);
		Vector += FVector2D::UP;
	}
	else
	{
		// 비행 중일 경우 중력은 평소의 0.7배
		SpriteRenderer->SetAnimationSpeed(1.0f);

		PlayerGroundCheck(FVector2D::DOWN * (Speed * 0.7f) * _DeltaTime);
		UColor DownColor = CheckColor[static_cast<int>(CheckDir::Down)];

		if (!CheckMAGENTA(DownColor))
		{
			AddActorLocation(FVector2D::DOWN * (Speed * 0.7f) * _DeltaTime);
		}
	}

	if (IsPressKey(VK_LEFT))
	{
		Vector += FVector2D::LEFT;
	}
	if (IsPressKey(VK_RIGHT))
	{
		Vector += FVector2D::RIGHT;
	}
	Vector.Normalize();

	PlayerGroundCheck(Vector * (Speed * 2.0f) * _DeltaTime);
	UColor UpColor = CheckColor[static_cast<int>(CheckDir::Up)];

	if (!CheckMAGENTA(UpColor))
	{
		AddActorLocation(Vector * (Speed * 0.7f) * _DeltaTime);
	}
}

void APlayer::FlyEnd(float _DeltaTime)
{
	SpriteRenderer->ChangeAnimation("FlyEnd" + AnimDir);

	if (true == SpriteRenderer->IsCurAnimationEnd())
	{
		PlayerGroundCheck(FVector2D::ZERO * _DeltaTime);
		UColor DownColor = CheckColor[static_cast<int>(CheckDir::Down)];

		if (CheckMAGENTA(DownColor))
		{
			ChangeState(PlayerState::Idle);
			return;
		}
		else
		{
			ChangeState(PlayerState::Fall);
			return;
		}

	}
}

void APlayer::Fall(float _DeltaTime)
{
	SpriteRenderer->ChangeAnimation("Falling" + AnimDir);

	PlayerGroundCheck(GravityForce * _DeltaTime);
	Gravity(_DeltaTime);

	// 이동
	if (IsPressKey(VK_LEFT))
	{
		DirForce += FVector2D::LEFT * _DeltaTime * AccSpeed;
		SetLimitSpeed(1);
	}
	if (IsPressKey(VK_RIGHT))
	{
		DirForce += FVector2D::RIGHT * _DeltaTime * AccSpeed;
		SetLimitSpeed(1);
	}
	DirForce.Normalize();

	//PlayerGroundCheck(DirForce * )

	UColor DownColor = CheckColor[static_cast<int>(CheckDir::Down)];

	if (true == CheckMAGENTA(DownColor))
	{
		ChangeState(PlayerState::Idle);
		return;
	}
}

void APlayer::Bend(float _DeltaTime)
{
	SpriteRenderer->ChangeAnimation("Bend" + AnimDir);

	if (true == UEngineInput::GetInst().IsPress(VK_DOWN))
	{
		PlayerGroundCheck(FVector2D::DOWN + FVector2D(0.0f, 20.0f) * _DeltaTime);
		UColor DownColor = CheckColor[static_cast<int>(CheckDir::Down)];

		if (true == CheckYELLOW(DownColor))
		{
			ChangeState(PlayerState::Climb);
			return;
		}
	}
	else
	{
		ChangeState(PlayerState::Idle);
		return;
	}

	if (true == UEngineInput::GetInst().IsPress('Z')
		|| true == UEngineInput::GetInst().IsPress('X'))
	{
		ChangeState(PlayerState::Slide);
		return;
	}
}

void APlayer::Slide(float _DeltaTime)
{
	SpriteRenderer->ChangeAnimation("Slide" + AnimDir);
	FVector2D Vector = FVector2D::ZERO;
	float CurTime = 0.0f;
	CurTime += _DeltaTime;

	if (false == GetDirLeft())
	{
		Vector += FVector2D::RIGHT;
	}
	else
	{
		Vector += FVector2D::LEFT;
	}

	DirForce += Vector * _DeltaTime * AccSpeed;
	SetLimitSpeed(1);

	UColor LeftColor = CheckColor[static_cast<int>(CheckDir::LeftDown)];
	UColor RightColor = CheckColor[static_cast<int>(CheckDir::RightDown)];

	if (false == CheckMAGENTA(LeftColor) && false == CheckMAGENTA(RightColor))
	{
		AddActorLocation(DirForce * _DeltaTime);
	}
	else if (CurTime >= 2.0f)
	{
		ChangeState(PlayerState::Idle);
		return;
	}
}

void APlayer::Climb(float _DeltaTime)
{	
	if (true == UEngineInput::GetInst().IsPress(VK_DOWN))
	{
		PlayerGroundCheck((FVector2D::DOWN * MaxSpeed * _DeltaTime));
		UColor DownColor = CheckColor[static_cast<int>(CheckDir::Down)];

		if (false == CheckMAGENTA(DownColor))
		{
			SpriteRenderer->ChangeAnimation("ClimbDown");
			AddActorLocation(FVector2D::DOWN * MaxSpeed * _DeltaTime);
		}
		else
		{
			ChangeState(PlayerState::Idle);
			return;
		}
	}
	if (true == UEngineInput::GetInst().IsPress(VK_UP))
	{
		PlayerGroundCheck(FVector2D::UP * MaxSpeed * _DeltaTime);
		UColor DownColor = CheckColor[static_cast<int>(CheckDir::Down)];

		if (true == CheckYELLOW(DownColor))
		{
			SpriteRenderer->ChangeAnimation("ClimbUp");
			AddActorLocation(FVector2D::UP * MaxSpeed * _DeltaTime);
		}
		else
		{
			ChangeState(PlayerState::Idle);
			return;
		}
	}
	if (true == UEngineInput::GetInst().IsPress(VK_LEFT)
		|| true == UEngineInput::GetInst().IsPress(VK_RIGHT))
	{
		ChangeState(PlayerState::Idle);
		return;
	}
	
}

