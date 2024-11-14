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
	UEngineDebug::CoreOutPutString("DirAcc : " + std::to_string(DirAcc));


	// 좌우 방향 및 스피드 체크
	SetAnimDir();
	SetSpeed();

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
	case PlayerState::Fly:
		Fly(_DeltaTime);
		break;
	case PlayerState::Climb:
		Climb(_DeltaTime);
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
	SpriteRenderer->CreateAnimation("FlyStart_Left", "Kirby_Normal_Left.png", 17, 21, 0.1f, false);
	SpriteRenderer->CreateAnimation("FlyStart_Right", "Kirby_Normal_Right.png", 17, 21, 0.1f, false);

	// Flying
	SpriteRenderer->CreateAnimation("Flying_Left", "Kirby_Normal_Left.png", 21, 22, 0.7f);
	SpriteRenderer->CreateAnimation("Flying_Right", "Kirby_Normal_Right.png", 21, 22, 0.7f);

	// FlyEnd
	SpriteRenderer->CreateAnimation("FlyEnd_Left", "Kirby_Normal_Left.png", 23, 26, 0.1f, false);
	SpriteRenderer->CreateAnimation("FlyEnd_Right", "Kirby_Normal_Right.png", 23, 26, 0.1f, false);

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

void APlayer::SetAnimDir()
{
	if (true == UEngineInput::GetInst().IsPress(VK_LEFT))
	{
		AnimDir = "_Left";
	}
	else if (true == UEngineInput::GetInst().IsPress(VK_RIGHT))
	{
		AnimDir = "_Right";
	}
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
		GravityForce += FVector2D::DOWN * _DeltaTime * 360.0f;
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
	// 왼쪽 이동 (Move, Dash)
	if (true == UEngineInput::GetInst().IsDoubleClick(VK_LEFT, 0.2f))
	{
		ChangeState(PlayerState::Dash);
		return;
	}
	else if (true == UEngineInput::GetInst().IsPress(VK_LEFT))
	{
		ChangeState(PlayerState::Move);
		return;
	}

	// 오른쪽 이동 (Move, Dash)
	if (true == UEngineInput::GetInst().IsDoubleClick(VK_RIGHT, 0.2f))
	{
		ChangeState(PlayerState::Dash);
		return;
	}
	else if (true == UEngineInput::GetInst().IsPress(VK_RIGHT))
	{
		ChangeState(PlayerState::Move);
		return;
	}

	UColor DownColor = CheckColor[static_cast<int>(CheckDir::Down)];
	UColor UpColor = CheckColor[static_cast<int>(CheckDir::Up)];

	if (true == UEngineInput::GetInst().IsPress(VK_DOWN))
	{
		// 내려가기
		if (true == CheckYELLOW(DownColor))
		{
			ChangeState(PlayerState::Climb);
			return;
		}
		else  // 숙이기
		{
		ChangeState(PlayerState::Bend);
		return;
		}
	}

	if (true == UEngineInput::GetInst().IsPress(VK_UP))
	{
		// 오르기
		if (true == CheckYELLOW(UpColor))
		{
			ChangeState(PlayerState::Climb);
		}
		else  // 비행
		{
			ChangeState(PlayerState::Fly);
			return;
		}
	}

	// 점프
	if (true == UEngineInput::GetInst().IsPress('Z') && CurPlayerState != PlayerState::Bend)
	{
		ChangeState(PlayerState::Jump);
		return;
	}
}

void APlayer::Move(float _DeltaTime)
{
	SpriteRenderer->ChangeAnimation("Walk" + AnimDir);

	// 중력가속도를 기준으로 지면 체크
	PlayerGroundCheck(GravityForce * _DeltaTime);
	Gravity(_DeltaTime);

	// 이동 방향 벡터 설정
	FVector2D Vector = FVector2D::ZERO;

	if (true == UEngineInput::GetInst().IsPress(VK_LEFT))
	{
		Vector += FVector2D::LEFT;
	}
	if (true == UEngineInput::GetInst().IsPress(VK_RIGHT))
	{
		Vector += FVector2D::RIGHT;
	}

	// 키 입력 체크
	UColor DownColor = CheckColor[static_cast<int>(CheckDir::Down)];
	UColor UpColor = CheckColor[static_cast<int>(CheckDir::Up)];

	if (false == UEngineInput::GetInst().IsPress(VK_LEFT)
		&& false == UEngineInput::GetInst().IsPress(VK_RIGHT))
	{
		ChangeState(PlayerState::Idle);
		return;
	}
	if (true == UEngineInput::GetInst().IsPress(VK_DOWN))
	{
		// 내려가기
		if (true == CheckYELLOW(DownColor))
		{
			ChangeState(PlayerState::Climb);
			return;
		}
		else  // 숙이기
		{
			ChangeState(PlayerState::Bend);
			return;
		}
	}
	if (true == UEngineInput::GetInst().IsPress(VK_UP))
	{
		// 오르기
		if (true == CheckYELLOW(DownColor))
		{
			ChangeState(PlayerState::Climb);
		}
		else  // 비행
		{
			ChangeState(PlayerState::Fly);
			return;
		}
	}
	if (true == UEngineInput::GetInst().IsPress('Z')
		&& CurPlayerState != PlayerState::Bend)
	{
		ChangeState(PlayerState::Jump);
		return;
	}
	Vector.Normalize();

	// 이동할 위치 충돌 체크
	PlayerGroundCheck(Vector * _DeltaTime * DirAcc);
	UColor LeftColor = CheckColor[static_cast<int>(CheckDir::LeftDown)];
	UColor RightColor = CheckColor[static_cast<int>(CheckDir::RightDown)];

	if (false == CheckMAGENTA(LeftColor) && false == CheckMAGENTA(RightColor))
	{
		AddActorLocation(Vector * _DeltaTime * DirAcc);
		DirAcc += 50.0f;
	} 
}

void APlayer::Dash(float _DeltaTime)
{
	PlayerGroundCheck(GravityForce * _DeltaTime);
	Gravity(_DeltaTime);

	// 방향 체크 및 애니메이션
	SetAnimDir();
	SpriteRenderer->ChangeAnimation("Dash" + AnimDir);

	// 이동 방향 벡터 설정
	FVector2D Vector = FVector2D::ZERO;
	PlayerGroundCheck(Vector * _DeltaTime * (DirAcc * 1.2f));

	if (true == UEngineInput::GetInst().IsPress(VK_LEFT))
	{
		Vector += FVector2D::LEFT;
	}
	if (true == UEngineInput::GetInst().IsPress(VK_RIGHT))
	{
		Vector += FVector2D::RIGHT;
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
	if (true == UEngineInput::GetInst().IsPress('Z'))
	{
		ChangeState(PlayerState::Jump);
		return;
	}
	
	// 이동할 위치 충돌 체크
	UColor LeftColor = CheckColor[static_cast<int>(CheckDir::LeftDown)];
	UColor RightColor = CheckColor[static_cast<int>(CheckDir::RightDown)];
	
	if (false == CheckMAGENTA(LeftColor) && false == CheckMAGENTA(RightColor))
	{
		AddActorLocation(Vector * _DeltaTime * (DirAcc * 1.2f));
		DirAcc += 50.0f;
	}
}

void APlayer::Fly(float _DeltaTime)
{
	PlayerGroundCheck(FVector2D::UP * _DeltaTime * (Speed * 0.5f));

	SpriteRenderer->ChangeAnimation("FlyStart" + AnimDir);

	FVector2D Vector = FVector2D::ZERO;

	if (true == UEngineInput::GetInst().IsPress	(VK_UP))
	{
		Vector += FVector2D::UP;
	}
	else 
	{
		PlayerGroundCheck(FVector2D::DOWN * _DeltaTime * (Speed * 0.5f));
		UColor DownColor = CheckColor[static_cast<int>(CheckDir::Down)];
		if (false == CheckMAGENTA(DownColor) && false == CheckBLACK(DownColor))
		{
			AddActorLocation(FVector2D::DOWN * _DeltaTime * (Speed * 0.5f));
		}
	}

	if (true == UEngineInput::GetInst().IsPress(VK_LEFT))
	{
		Vector += FVector2D::LEFT;
	}
	if (true == UEngineInput::GetInst().IsPress(VK_RIGHT))
	{
		Vector += FVector2D::RIGHT;
	}

	Vector.Normalize();
	
	UColor UpColor = CheckColor[static_cast<int>(CheckDir::Up)];
	UColor LeftColor = CheckColor[static_cast<int>(CheckDir::LeftDown)];
	UColor RightColor = CheckColor[static_cast<int>(CheckDir::RightDown)];

	if (false == CheckMAGENTA(UpColor) && false == CheckMAGENTA(LeftColor) 
		&& false == CheckMAGENTA(RightColor))
	{
		AddActorLocation(Vector * Speed * _DeltaTime * 0.6f);
	}

	if (true == UEngineInput::GetInst().IsPress('X'))
	{
		SpriteRenderer->ChangeAnimation("FlyEnd" + AnimDir);
		ChangeState(PlayerState::Idle);
		return;
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

	PlayerGroundCheck(GravityForce * _DeltaTime);
	Gravity(_DeltaTime);

	FVector2D NextPos = FVector2D::UP;
	if (true == UEngineInput::GetInst().IsPress(VK_LEFT))
	{
		NextPos += FVector2D::LEFT;
	}
	if (true == UEngineInput::GetInst().IsPress(VK_RIGHT))
	{
		NextPos += FVector2D::RIGHT;
	}
	NextPos.Normalize();

	if (true == UEngineInput::GetInst().IsPress(VK_UP))
	{
		ChangeState(PlayerState::Fly);
		return;
	}

	AddActorLocation(NextPos * JumpForce * _DeltaTime);

	UColor UpColor = CheckColor[static_cast<int>(CheckDir::Up)];
	UColor DownColor = CheckColor[static_cast<int>(CheckDir::Down)];

	if (true == CheckMAGENTA(DownColor) || true == CheckMAGENTA(UpColor))
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
		PlayerGroundCheck(FVector2D::DOWN + FVector2D(0.0f, 20.0f) * _DeltaTime * Speed);
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

	if (false == GetDirLeft())
	{
		Vector += FVector2D::RIGHT;
	}
	else
	{
		Vector += FVector2D::LEFT;
	}

	PlayerGroundCheck(Vector * _DeltaTime * Speed);
	UColor LeftColor = CheckColor[static_cast<int>(CheckDir::LeftDown)];
	UColor RightColor = CheckColor[static_cast<int>(CheckDir::RightDown)];

	if (true == CheckMAGENTA(LeftColor) && false == CheckMAGENTA(RightColor))
	{
		AddActorLocation(Vector * _DeltaTime * Speed);
	}
	else
	{
		ChangeState(PlayerState::Bend);
		return;
	}
}

void APlayer::Climb(float _DeltaTime)
{	
	if (true == UEngineInput::GetInst().IsPress(VK_DOWN))
	{
		PlayerGroundCheck((FVector2D::DOWN * Speed * _DeltaTime));
		UColor DownColor = CheckColor[static_cast<int>(CheckDir::Down)];

		if (false == CheckMAGENTA(DownColor))
		{
			SpriteRenderer->ChangeAnimation("ClimbDown");
			AddActorLocation(FVector2D::DOWN * Speed * _DeltaTime);
		}
		else
		{
			ChangeState(PlayerState::Idle);
			return;
		}
	}
	if (true == UEngineInput::GetInst().IsPress(VK_UP))
	{
		PlayerGroundCheck(FVector2D::UP * Speed * _DeltaTime);
		UColor DownColor = CheckColor[static_cast<int>(CheckDir::Down)];

		if (true == CheckYELLOW(DownColor))
		{
			SpriteRenderer->ChangeAnimation("ClimbUp");
			AddActorLocation(FVector2D::UP * Speed * _DeltaTime);
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
