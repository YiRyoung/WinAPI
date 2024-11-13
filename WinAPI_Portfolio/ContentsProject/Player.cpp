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
		SpriteRenderer->CreateAnimation("FlyStart_Left", "Kirby_Normal_Left.png", 17, 22, 0.1f, false);
		SpriteRenderer->CreateAnimation("FlyStart_Right", "Kirby_Normal_Right.png", 17, 22, 0.1f, false);

		// Flying
		SpriteRenderer->CreateAnimation("Flying_Left", "Kirby_Normal_Left.png", 21, 22, 0.7f);
		SpriteRenderer->CreateAnimation("Flying_Right", "Kirby_Normal_Right.png", 21, 22, 0.7f);

		// FlyEnd
		SpriteRenderer->CreateAnimation("FlyiEnd_Left", "Kirby_Normal_Left.png", 23, 26, 0.1f, false);
		SpriteRenderer->CreateAnimation("FlyEnd_Right", "Kirby_Normal_Right.png", 23, 26, 0.1f, false);

		// JumpStart
		SpriteRenderer->CreateAnimation("JumpStart_Left", "Kirby_Normal_Left.png", 11, 11, 1.0f);
		SpriteRenderer->CreateAnimation("JumpStart_Right", "Kirby_Normal_Right.png", 11, 11, 1.0f);

		// Jumping
		SpriteRenderer->CreateAnimation("Jumping_Left", "Kirby_Normal_Left.png", 12, 14, 0.5f, false);
		SpriteRenderer->CreateAnimation("Jumping_Right", "Kirby_Normal_Right.png", 12, 14, 0.5f, false);

		// JumpEnd
		SpriteRenderer->CreateAnimation("JumpEnd_Left", "Kirby_Normal_Left.png", 15, 15, 1.0f);
		SpriteRenderer->CreateAnimation("JumpEnd_Right", "Kirby_Normal_Right.png", 15, 15, 1.0f);

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
}

void APlayer::Gravity(float _DeltaTime)
{
	if (UColor::WHITE == CheckColor[static_cast<int>(CheckDir::Down)]
		|| UColor::BLUE == CheckColor[static_cast<int>(CheckDir::Down)]
		|| UColor::RED == CheckColor[static_cast<int>(CheckDir::Down)])
	{
		AddActorLocation(GravityForce * _DeltaTime);
		GravityForce += FVector2D::DOWN * _DeltaTime * 300.0f;
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

	// 키 입력 체크
	// 왼쪽 이동 (Move, Dash)
	if (true == UEngineInput::GetInst().IsDoubleClick(VK_LEFT, 0.2f))
	{
		IsLeft = true;
		ChangeState(PlayerState::Dash);
		return;
	}
	else if (true == UEngineInput::GetInst().IsPress(VK_LEFT))
	{
		IsLeft = true;
		ChangeState(PlayerState::Move);
		return;
	}

	// 오른쪽 이동 (Move, Dash)
	if (true == UEngineInput::GetInst().IsDoubleClick(VK_RIGHT, 0.2f))
	{
		IsLeft = false;
		ChangeState(PlayerState::Dash);
		return;
	}
	else if (true == UEngineInput::GetInst().IsPress(VK_RIGHT))
	{
		IsLeft = false;
		ChangeState(PlayerState::Move);
		return;
	}

	// 숙이기
	if (true == UEngineInput::GetInst().IsPress(VK_DOWN)
		&& false == UEngineInput::GetInst().IsPress(VK_UP))
	{
		ChangeState(PlayerState::Bend);
		return;
	}

	// 비행
	if (true == UEngineInput::GetInst().IsPress(VK_UP))
	{
		ChangeState(PlayerState::Fly);
	}

	// 점프
	if (true == UEngineInput::GetInst().IsPress('Z')
		&& CurPlayerState != PlayerState::Bend)
	{
		ChangeState(PlayerState::Jump);
		return;
	}

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
	if (true == UEngineInput::GetInst().IsPress('Z')
		&& CurPlayerState != PlayerState::Bend)
	{
		ChangeState(PlayerState::Jump);
		return;
	}
	
	// 이동할 위치 충돌 체크
	if (false == IsMAGENTA)
	{
		AddActorLocation(Vector * _DeltaTime * Speed);
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
	PlayerGroundCheck(Vector * _DeltaTime * (Speed * 1.2f));

	if (true == UEngineInput::GetInst().IsPress(VK_LEFT))
	{
		Vector += FVector2D::LEFT;
		UColor LeftColor = CheckColor[static_cast<int>(CheckDir::LeftDown)];
		IsMAGENTA = CheckMAGENTA(LeftColor);
	}
	if (true == UEngineInput::GetInst().IsPress(VK_RIGHT))
	{
		Vector += FVector2D::RIGHT;
		UColor RightColor = CheckColor[static_cast<int>(CheckDir::RightDown)];
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
	if (true == UEngineInput::GetInst().IsPress('Z'))
	{
		ChangeState(PlayerState::Jump);
		return;
	}

	// 이동할 위치 충돌 체크
	if (false == IsMAGENTA)
	{
		AddActorLocation(Vector * _DeltaTime * (Speed * 1.2f));
	}
}

void APlayer::Fly(float _DeltaTime)
{
	PlayerGroundCheck(GravityForce * _DeltaTime * 0.6f);
	

	SpriteRenderer->ChangeAnimation("FlyStart" + AnimDir);

	FVector2D Vector = FVector2D::ZERO;

	if (true == UEngineInput::GetInst().IsPress	(VK_UP))
	{
		GravityForce = FVector2D::ZERO;
		Vector += FVector2D::UP;
		UColor UpColor = CheckColor[static_cast<int>(CheckDir::Up)];
		IsMAGENTA = CheckMAGENTA(UpColor);
	}
	else 
	{
		Gravity(_DeltaTime * 0.6f);
	}


	if (true == UEngineInput::GetInst().IsPress(VK_LEFT))
	{
		Vector += FVector2D::LEFT;
		UColor LeftColor = CheckColor[static_cast<int>(CheckDir::LeftDown)];
		IsMAGENTA = CheckMAGENTA(LeftColor);
	}
	if (true == UEngineInput::GetInst().IsPress(VK_RIGHT))
	{
		Vector += FVector2D::RIGHT;
		UColor RightColor = CheckColor[static_cast<int>(CheckDir::RightDown)];
		IsMAGENTA = CheckMAGENTA(RightColor);
	}

	Vector.Normalize();
	if (false == IsMAGENTA)
	{
		AddActorLocation(Vector * Speed * _DeltaTime * 0.6f);
	}


	if (true == UEngineInput::GetInst().IsPress('X'))
	{
	}


}

void APlayer::Jump(float _DeltaTime)
{
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
	SetAnimDir();
	SpriteRenderer->ChangeAnimation("Bend" + AnimDir);

	if (false == UEngineInput::GetInst().IsPress(VK_DOWN)

		|| true == UEngineInput::GetInst().IsPress(VK_UP))
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
	SetAnimDir();
	SpriteRenderer->ChangeAnimation("Slide" + AnimDir);
	FVector2D Vector = FVector2D::ZERO;

	if (true == IsLeft)
	{
		Vector += FVector2D::LEFT;
	}
	else
	{
		Vector += FVector2D::RIGHT;
	}

	PlayerGroundCheck(Vector * _DeltaTime * Speed);

	UColor LeftColor = CheckColor[static_cast<int>(CheckDir::LeftDown)];
	IsMAGENTA = CheckMAGENTA(LeftColor);
	UColor RightColor = CheckColor[static_cast<int>(CheckDir::RightDown)];
	IsMAGENTA = CheckMAGENTA(RightColor);

	if (false == CheckMAGENTA(LeftColor) && false == CheckMAGENTA(RightColor))
	{
		AddActorLocation(Vector * _DeltaTime * Speed);
	}
	else
	{
		ChangeState(PlayerState::Bend);
	}
}
