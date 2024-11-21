#include "PreCompile.h"
#include "PlayerState.h"

#include <EnginePlatform/EngineInput.h>
#include <EngineCore/2DCollision.h>

PlayerState::PlayerState()
{
}

PlayerState::PlayerState(APlayer* _Player) : Player(_Player)
{
}

PlayerState::~PlayerState()
{
}

bool PlayerState::IsDownKey(int _KeyCode) const
{
	if (UEngineInput::GetInst().IsDown(_KeyCode))
	{
		return true;
	}
	else if (!UEngineInput::GetInst().IsDown(_KeyCode))
	{
		return false;
	}
	return false;
}

bool PlayerState::IsPressKey(int _KeyCode) const
{
	if (UEngineInput::GetInst().IsPress(_KeyCode))
	{
		return true;
	}
	else if (!UEngineInput::GetInst().IsPress(_KeyCode))
	{
		return false;
	}
	return false;
}

bool PlayerState::IsDoubleKey(int _KeyCode, float _Count) const
{
	if (UEngineInput::GetInst().IsDoubleClick(_KeyCode, _Count))
	{
		return true;
	}
	else if (!UEngineInput::GetInst().IsDoubleClick(_KeyCode, _Count))
	{
		return false;
	}
	return false;
}

void PlayerState::ChangeAnimation(std::string _Anim)
{
	std::string Text = _Anim + Player->GetAnimDir();
	Player->ChangeAnimation(Text);
}

void PlayerState::SetAnimSpeed(float _Speed)
{
	Player->SetAnimSpeed(_Speed);
}

bool PlayerState::IsAnimFinish()
{
	if (true == Player->IsAnimFinish())
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool PlayerState::CheckPointColor(CheckDir _Dir, UColor _Color)
{
	switch (_Dir)
	{
	case CheckDir::UP:
		return Player->UpperPointCheck(_Color);
		break;
	case CheckDir::DOWN:
		return Player->BottomPointCheck(_Color);
		break;
	}
	return false;
}

bool PlayerState::CheckColor(CheckDir _Dir, UColor _Color)
{
	if (true == Player->PixelLineColor(_Dir, _Color))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void PlayerState::Gravity(float _DeltaTime)
{
	if (!CheckColor(CheckDir::DOWN, UColor::MAGENTA) && !CheckColor(CheckDir::DOWN, UColor::BLACK))
	{
		Move(GravityForce * _DeltaTime);
		GravityForce += FVector2D::DOWN * 500.0f * _DeltaTime;
	}
	else
	{
		GravityForce = FVector2D::ZERO;
	}
}

void PlayerState::Move(FVector2D _NextPos)
{
	Player->AddActorLocation(_NextPos);
}

void PlayerState::SetLimitSpeed(bool _IsAccel)
{
	switch (_IsAccel)
	{
	case true:  // MaxSpeed
		if (DirForce.Length() >= MaxSpeed)
		{
			DirForce.Normalize();
			DirForce.X *= MaxSpeed;
		}
		break;
	case false: // ZeroSpeed
		if (DirForce.Length() <= 0.01f)
		{
			DirForce.X = 0.0f;
		}
		break;
	}
}

void PlayerState::Attack()
{
	if (Player->GetAbility() == EAblityType::NORMAL && !Player->GetFull())
	{
		SetState(EStateType::INHALESTART);
		return;
	}
	else if (Player->GetAbility() == EAblityType::NORMAL && Player->GetFull())
	{
		SetState(EStateType::INHALEEND);
		return;
	}
	else
	{
		SetState(EStateType::SKILL);
		return;
	}
}

void PlayerState::SetState(EStateType _State)
{
	Player->SetState(_State);
}

void PlayerState::Idle(float _DeltaTime)
{
	if (Player->GetFull())
	{
		ChangeAnimation("IdleFull");
	}
	else
	{
		ChangeAnimation("Idle");
	}

	Gravity(_DeltaTime);

	// Move & Dash
	if (IsDoubleKey(VK_LEFT, 0.2f) || IsDoubleKey(VK_RIGHT, 0.2f) && !Player->GetFull())
	{
		SetState(EStateType::DASH);
		return;
	}
	else if (IsPressKey(VK_LEFT) || IsPressKey(VK_RIGHT))
	{
		SetState(EStateType::WALK);
		return;
	}

	// Jump
	if (IsPressKey('Z') && (EStateType::BEND != GetState()))
	{
		SetState(EStateType::JUMP);
		return;
	}

	// FlyStart
	if (IsPressKey(VK_UP) && !CheckColor(CheckDir::UP, UColor::YELLOW)
		&& !Player->GetFull())
	{
		SetState(EStateType::FLYSTART);
		return;
	}
	
	// Bend
	if (IsPressKey(VK_DOWN) && !CheckColor(CheckDir::DOWN, UColor::YELLOW))
	{
		if (!Player->GetFull())
		{
			SetState(EStateType::BEND);
			return;
		}
		else
		{
			SetState(EStateType::EAT);
			return;
		}
	}

	// Climb
	if ((IsPressKey(VK_UP) && CheckPointColor(CheckDir::UP, UColor::YELLOW))
		|| (IsPressKey(VK_DOWN) && CheckPointColor(CheckDir::DOWN, UColor::YELLOW)))
	{
		SetState(EStateType::CLIMB);
		return;
	}

	// Falling
	if (!CheckColor(CheckDir::DOWN, UColor::MAGENTA) && !CheckColor(CheckDir::DOWN, UColor::YELLOW))
	{
		SetState(EStateType::FALLING);
		return;
	}

	// Attack
	if (IsDownKey('X'))
	{
		Attack();
	}

	// DeAccel
	if (!CheckColor(CheckDir::LEFT, UColor::MAGENTA)
		&& !CheckColor(CheckDir::RIGHT, UColor::MAGENTA))
	{
		DirForce.X += -DirForce.X * DeAccSpeed * _DeltaTime;
		SetLimitSpeed(false);
		Move(DirForce * _DeltaTime);
	}
}

void PlayerState::Walk(float _DeltaTime)
{
	if (Player->GetFull())
	{
		ChangeAnimation("WalkFull");
	}
	else
	{
		ChangeAnimation("Walk");
	}

	Gravity(_DeltaTime);

	// Jump
	if (IsPressKey('Z') && EStateType::BEND != GetState())
	{
		SetState(EStateType::JUMP);
		return;
	}
	
	// FlyStart
	if (IsPressKey(VK_UP) && !CheckColor(CheckDir::DOWN, UColor::YELLOW)
		&& !Player->GetFull())
	{
		SetState(EStateType::FLYSTART);
		return;
	}

	// Bend
	if (IsPressKey(VK_DOWN) && !CheckColor(CheckDir::DOWN, UColor::YELLOW))
	{
		if (!Player->GetFull())
		{
			SetState(EStateType::BEND);
			return;
		}
		else
		{
			SetState(EStateType::EAT);
			return;
		}
	}

	// Climb
	if ((IsPressKey(VK_UP) && CheckPointColor(CheckDir::UP, UColor::YELLOW))
		|| (IsPressKey(VK_DOWN) && CheckPointColor(CheckDir::DOWN, UColor::YELLOW)))
	{
		SetState(EStateType::CLIMB);
		return;
	}

	// Idle
	if (!IsPressKey(VK_LEFT) && !IsPressKey(VK_RIGHT))
	{
		SetState(EStateType::IDLE);
		return;
	}

	// Moving
	float NewSpeed = 0;
	if (Player->GetFull())
	{
		NewSpeed = Speed * 0.5f;
	}
	else
	{
		NewSpeed = Speed;
	}

	// Attack
	if (IsPressKey('X'))
	{
		Attack();
	}


	if (IsPressKey(VK_LEFT))
	{
		if (!CheckColor(CheckDir::LEFT, UColor::MAGENTA))
		{
			Move(FVector2D::LEFT * NewSpeed * _DeltaTime);
		}
	}
	if (IsPressKey(VK_RIGHT))
	{
		if (!CheckColor(CheckDir::RIGHT, UColor::MAGENTA))
		{
			Move(FVector2D::RIGHT * NewSpeed * _DeltaTime);
		}
	}
}

void PlayerState::Dash(float _DeltaTime)
{
	ChangeAnimation("Dash");
	Gravity(_DeltaTime);

	// Jump
	if (IsPressKey('Z') && EStateType::BEND != GetState())
	{
		DirForce = FVector2D::ZERO;
		SetState(EStateType::JUMP);
		return;
	}

	// FlyStart
	if (IsPressKey(VK_UP) && !CheckColor(CheckDir::UP, UColor::YELLOW)
		&& !Player->GetFull())
	{
		DirForce = FVector2D::ZERO;
		SetState(EStateType::FLYSTART);
		return;
	}

	// Bend
	if (IsPressKey(VK_DOWN) && !CheckColor(CheckDir::DOWN, UColor::YELLOW))
	{
		if (!Player->GetFull())
		{
			SetState(EStateType::BEND);
			return;
		}
		else
		{
			SetState(EStateType::EAT);
			return;
		}
	}

	// Climb
	if ((IsPressKey(VK_UP) && CheckPointColor(CheckDir::UP, UColor::YELLOW))
		|| (IsPressKey(VK_DOWN) && CheckPointColor(CheckDir::DOWN, UColor::YELLOW)))
	{
		SetState(EStateType::CLIMB);
		return;
	}

	// Idle
	if (!IsPressKey(VK_LEFT) && !IsPressKey(VK_RIGHT))
	{
		DirForce = FVector2D::ZERO;
		SetState(EStateType::IDLE);
		return;
	}

	// Attack
	if (IsPressKey('X'))
	{
		Attack();
	}

	// Dashing
	FVector2D Vector = FVector2D::ZERO;

	if (IsPressKey(VK_LEFT))
	{
		Vector = FVector2D::LEFT;
	}
	if (IsPressKey(VK_RIGHT))
	{
		Vector += FVector2D::RIGHT;
	}

	DirForce += Vector * AccSpeed * _DeltaTime;
	SetLimitSpeed(true);

	if (!CheckColor(CheckDir::LEFT, UColor::MAGENTA) && !CheckColor(CheckDir::RIGHT, UColor::MAGENTA))
	{
		Move(DirForce * _DeltaTime);
	}
}

void PlayerState::Jump(float _DeltaTime)
{
	float Force = JumpForce - GravityForce.Y;

	if (0.0f < Force)
	{
		if (Player->GetFull())
		{
			ChangeAnimation("JumpStartFull");
		}
		else
		{
			ChangeAnimation("JumpStart");
		}
	}
	else
	{
		if (Player->GetFull())
		{
			ChangeAnimation("JumpEndFull");
		}
		else
		{
			ChangeAnimation("JumpEnd");
		}
	}

	Gravity(_DeltaTime);

	// Attack
	if (IsPressKey('X'))
	{
		Attack();
	}

	// FlyStart
	if (IsPressKey(VK_UP) && !Player->GetFull())
	{
		SetState(EStateType::FLYSTART);
		return;
	}

	// Jumping
	DirForce += FVector2D::UP;
	float PushForce = 0.0f;

	if (Player->GetFull())
	{
		PushForce = JumpForce * 0.8f;
	}
	else
	{
		PushForce = JumpForce;
	}

	if (IsPressKey(VK_LEFT))
	{
		DirForce += FVector2D::LEFT * _DeltaTime * AccSpeed;
		SetLimitSpeed(true);
	}
	if (IsPressKey(VK_RIGHT))
	{
		DirForce += FVector2D::RIGHT * _DeltaTime * AccSpeed;
		SetLimitSpeed(true);
	}
	DirForce.Normalize();
	Move(DirForce * PushForce * _DeltaTime);

	if (CheckColor(CheckDir::DOWN, UColor::MAGENTA) || CheckColor(CheckDir::UP, UColor::MAGENTA)
		|| CheckColor(CheckDir::DOWN, UColor::BLACK))
	{
		SetState(EStateType::IDLE);
		return;
	}
}

void PlayerState::FlyStart(float _DeltaTime)
{
	ChangeAnimation("FlyStart");

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

	if (!CheckColor(CheckDir::UP, UColor::MAGENTA) && !CheckColor(CheckDir::DOWN, UColor::MAGENTA))
	{
		Move(Vector * Speed * _DeltaTime);
	}

	if (true == Player->IsAnimFinish())
	{
		SetState(EStateType::FLYING);
		return;
	}
}

void PlayerState::Flying(float _DeltaTime)
{
	ChangeAnimation("Flying");

	// Falling
	if (true == IsPressKey('X'))
	{
		SetAnimSpeed(1.0f);
		SetState(EStateType::FLYEND);
		return;
	}

	FVector2D Vector = FVector2D::ZERO;

	if (IsPressKey(VK_UP))  // Fly High!
	{
		GravityForce = FVector2D::ZERO;
		SetAnimSpeed(5.0f);
		Vector += FVector2D::UP;
	}
	else // Landing
	{
		SetAnimSpeed(1.0f);
		if (!CheckColor(CheckDir::DOWN, UColor::MAGENTA) && !CheckColor(CheckDir::DOWN, UColor::BLACK))
		{
			Move(FVector2D::DOWN * (Speed * 0.7f) * _DeltaTime);
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

	if (!CheckColor(CheckDir::UP, UColor::MAGENTA))
	{
		Move(Vector * (Speed * 0.7f) * _DeltaTime);
	}
}

void PlayerState::FlyEnd(float _DeltaTime)
{
	ChangeAnimation("FlyEnd");

	if (true == Player->IsAnimFinish())
	{
		if (CheckColor(CheckDir::DOWN, UColor::MAGENTA))
		{
			SetState(EStateType::IDLE);
			return;
		}
		else
		{
			SetState(EStateType::FALLING);
			return;
		}
	}
}

void PlayerState::Falling(float _DeltaTime)
{
	if (!Player->GetFull())
	{
		ChangeAnimation("Falling");
	}
	else
	{
		ChangeAnimation("JumpEndFull");
	}

	Gravity(_DeltaTime);
	// Black ¹× Yellow Ãß°¡

	// FlyStart
	if (IsPressKey(VK_UP))
	{
		SetState(EStateType::FLYSTART);
		return;
	}

	FVector2D Vector = FVector2D::ZERO;
	if (IsPressKey(VK_LEFT))
	{
		Vector += FVector2D::LEFT;
	}
	if (IsPressKey(VK_RIGHT))
	{
		Vector += FVector2D::RIGHT;
	}
	Move(Vector * Speed * _DeltaTime);

	//Idle
	if (CheckColor(CheckDir::DOWN, UColor::MAGENTA) 
		|| CheckColor(CheckDir::DOWN, UColor::YELLOW) || CheckColor(CheckDir::DOWN, UColor::BLACK))
	{
		DirForce = FVector2D::ZERO;
		SetState(EStateType::IDLE);
		return;
	}
}

void PlayerState::Bend(float _DeltaTime)
{
	ChangeAnimation("Bend");

	// Slide
	if (IsPressKey('Z') || IsPressKey('X'))
	{
		SetState(EStateType::SLIDE);
		return;
	}

	// Idle
	if (!IsPressKey(VK_DOWN))
	{
		SetState(EStateType::IDLE);
		return; 
	}
}

void PlayerState::Slide(float _DeltaTime)
{
	ChangeAnimation("Slide");
	SetSlideCollision(true);
	CurTime += _DeltaTime;
	Gravity(_DeltaTime);

	FVector2D Vector = FVector2D::ZERO;

	if ("_Right" == Player->GetAnimDir())
	{
		Vector += FVector2D::RIGHT;
	}
	else
	{
		Vector += FVector2D::LEFT;
	}

	if (!CheckColor(CheckDir::LEFT, UColor::MAGENTA) && !CheckColor(CheckDir::RIGHT, UColor::MAGENTA))
	{
		Move(Vector * Speed * _DeltaTime);
	}
	else
	{
		CurTime = 0.0f;
		DirForce = FVector2D::ZERO;
		SetSlideCollision(false);
		SetState(EStateType::IDLE);
		return;
	}

	if (CurTime >= 0.8f)
	{
		CurTime = 0.0f;
		DirForce = FVector2D::ZERO;
		SetSlideCollision(false);
		SetState(EStateType::IDLE);
		return;
	}

}

void PlayerState::Climb(float _DeltaTime)
{
	if (IsPressKey(VK_LEFT) || IsPressKey(VK_RIGHT))
	{
		SetState(EStateType::FALLING);
		return;
	}

	if (IsPressKey(VK_UP))
	{
		ChangeAnimation("ClimbUp");

		if (CheckPointColor(CheckDir::DOWN, UColor::MAGENTA) || CheckPointColor(CheckDir::DOWN, UColor::YELLOW)
			|| CheckPointColor(CheckDir::DOWN, UColor::BLACK))
		{
			Move(FVector2D::UP * Speed * _DeltaTime);
		}
		if (!CheckPointColor(CheckDir::DOWN, UColor::YELLOW) && CheckPointColor(CheckDir::UP, UColor::WHITE))
		{
			SetState(EStateType::IDLE);
			return;
		}
	}

	if (IsPressKey(VK_DOWN))
	{
		ChangeAnimation("ClimbDown");

		if (CheckPointColor(CheckDir::UP, UColor::WHITE) || CheckPointColor(CheckDir::UP, UColor::YELLOW))
		{
			Move(FVector2D::DOWN * Speed * _DeltaTime);
		}
		if (CheckPointColor(CheckDir::DOWN, UColor::MAGENTA))
		{
			SetState(EStateType::IDLE);
			return;
		}
	}
}

void PlayerState::InhaleStart(float _DeltaTime)
{
	ChangeAnimation("InhaleStart");
	SetInhaleCollision(true);


	if (!IsPressKey('X'))
	{
		SetInhaleCollision(false);
		SetState(EStateType::IDLE);
		return;
	}
}

void PlayerState::InhaleEnd(float _DeltaTime)
{
	ChangeAnimation("InhaleEnd");
	SetInhaleCollision(false);

	if (IsAnimFinish())
	{
		Player->SetFull(false);
		SetState(EStateType::IDLE);
		return;
	}
}

void PlayerState::Eat(float _DeltaTime)
{
	ChangeAnimation("Eat");

	if (IsAnimFinish())
	{
		Player->SetFull(false);
		SetState(EStateType::IDLE);
		return;
	}
}

void PlayerState::Hurt(float _DeltaTime)
{
}
