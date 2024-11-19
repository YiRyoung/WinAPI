#include "PreCompile.h"
#include "PlayerState.h"

#include <EnginePlatform/EngineInput.h>

PlayerState::PlayerState()
{
}

PlayerState::PlayerState(APlayer* _Player) : Player(_Player)
{
}

PlayerState::~PlayerState()
{
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
	if (!CheckColor(CheckDir::DOWN, UColor::MAGENTA) 
		&& !CheckColor(CheckDir::DOWN, UColor::BLACK) && !CheckColor(CheckDir::DOWN, UColor::YELLOW))
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
			DirForce.X = MaxSpeed;
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

void PlayerState::SetState(StateType _State)
{
	Player->SetState(_State);
}

void PlayerState::Idle(float _DeltaTime)
{
	ChangeAnimation("Idle");
	Gravity(_DeltaTime);

	// Move & Dash
	if (IsDoubleKey(VK_LEFT, 0.2f) || IsDoubleKey(VK_RIGHT, 0.2f))
	{
		SetState(StateType::DASH);
		return;
	}
	else if (IsPressKey(VK_LEFT) || IsPressKey(VK_RIGHT))
	{
		SetState(StateType::WALK);
		return;
	}

	// Jump
	if (IsPressKey('Z') && (StateType::BEND != GetState()))
	{
		SetState(StateType::JUMP);
		return;
	}

	// FlyStart
	if (IsPressKey(VK_UP) && !CheckColor(CheckDir::UP, UColor::YELLOW))
	{
		SetState(StateType::FLYSTART);
		return;
	}
	
	// Bend
	if (IsPressKey(VK_DOWN) && !CheckColor(CheckDir::DOWN, UColor::YELLOW))
	{
		SetState(StateType::BEND);
		return;
	}

	// Climb
	if ((IsPressKey(VK_UP) && CheckPointColor(CheckDir::UP, UColor::YELLOW))
		|| (IsPressKey(VK_DOWN) && CheckPointColor(CheckDir::DOWN, UColor::YELLOW)))
	{
		SetState(StateType::CLIMB);
		return;
	}

	// Falling
	if (!CheckColor(CheckDir::DOWN, UColor::MAGENTA) && !CheckColor(CheckDir::DOWN, UColor::YELLOW))
	{
		SetState(StateType::FALLING);
		return;
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
	ChangeAnimation("Walk");
	Gravity(_DeltaTime);

	// Jump
	if (IsPressKey('Z') && StateType::BEND != GetState())
	{
		SetState(StateType::JUMP);
		return;
	}
	
	// FlyStart
	if (IsPressKey(VK_UP) && !CheckColor(CheckDir::DOWN, UColor::YELLOW))
	{
		SetState(StateType::FLYSTART);
		return;
	}

	// Bend
	if (IsPressKey(VK_DOWN) && !CheckColor(CheckDir::DOWN, UColor::YELLOW))
	{
		SetState(StateType::BEND);
		return;
	}

	// Climb
	if ((IsPressKey(VK_UP) && CheckPointColor(CheckDir::UP, UColor::YELLOW))
		|| (IsPressKey(VK_DOWN) && CheckPointColor(CheckDir::DOWN, UColor::YELLOW)))
	{
		SetState(StateType::CLIMB);
		return;
	}

	// Idle
	if (!IsPressKey(VK_LEFT) && !IsPressKey(VK_RIGHT))
	{
		SetState(StateType::IDLE);
		return;
	}

	// Moving
	if (IsPressKey(VK_LEFT))
	{
		if (!CheckColor(CheckDir::LEFT, UColor::MAGENTA))
		{
			Move(FVector2D::LEFT * Speed * _DeltaTime);
		}
	}
	if (IsPressKey(VK_RIGHT))
	{
		if (!CheckColor(CheckDir::RIGHT, UColor::MAGENTA))
		{
			Move(FVector2D::RIGHT * Speed * _DeltaTime);
		}
	}
}

void PlayerState::Dash(float _DeltaTime)
{
	ChangeAnimation("Dash");
	Gravity(_DeltaTime);

	// Jump
	if (IsPressKey('Z') && StateType::BEND != GetState())
	{
		DirForce = FVector2D::ZERO;
		SetState(StateType::JUMP);
		return;
	}

	// FlyStart
	if (IsPressKey(VK_UP) && !CheckColor(CheckDir::UP, UColor::YELLOW))
	{
		DirForce = FVector2D::ZERO;
		SetState(StateType::FLYSTART);
		return;
	}

	// Bend
	if (IsPressKey(VK_DOWN) && !CheckColor(CheckDir::DOWN, UColor::YELLOW))
	{
		SetState(StateType::BEND);
		return;
	}

	// Climb
	if ((IsPressKey(VK_UP) && CheckPointColor(CheckDir::UP, UColor::YELLOW))
		|| (IsPressKey(VK_DOWN) && CheckPointColor(CheckDir::DOWN, UColor::YELLOW)))
	{
		SetState(StateType::CLIMB);
		return;
	}

	// Idle
	if (!IsPressKey(VK_LEFT) && !IsPressKey(VK_RIGHT))
	{
		DirForce = FVector2D::ZERO;
		SetState(StateType::IDLE);
		return;
	}

	// Dashing
	if (IsPressKey(VK_LEFT))
	{
		DirForce += FVector2D::LEFT * AccSpeed * _DeltaTime;
		SetLimitSpeed(true);

		if (!CheckColor(CheckDir::LEFT, UColor::MAGENTA))
		{
			Move(DirForce * _DeltaTime);
		}
		else
		{
			DirForce = FVector2D::ZERO;
		}
	}
	if (IsPressKey(VK_RIGHT))
	{
		DirForce += FVector2D::RIGHT * AccSpeed * _DeltaTime;
		SetLimitSpeed(true);

		if (!CheckColor(CheckDir::RIGHT, UColor::MAGENTA))
		{
			Move(DirForce * _DeltaTime);
		}
		else
		{
			DirForce = FVector2D::ZERO;
		}
	}
}

void PlayerState::Jump(float _DeltaTime)
{
	float Force = JumpForce - GravityForce.Y;

	if (0.0f < Force)
	{
		ChangeAnimation("JumpStart");
	}
	else
	{
		ChangeAnimation("JumpEnd");
	}

	Gravity(_DeltaTime);

	// FlyStart
	if (IsPressKey(VK_UP))
	{
		SetState(StateType::FLYSTART);
		return;
	}

	// Jumping
	DirForce += FVector2D::UP;
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
	Move(DirForce * JumpForce * _DeltaTime);

	if (CheckColor(CheckDir::DOWN, UColor::MAGENTA) || CheckColor(CheckDir::UP, UColor::MAGENTA)
		|| CheckColor(CheckDir::DOWN, UColor::BLACK))
	{
		SetState(StateType::IDLE);
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
		SetState(StateType::FLYING);
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
		SetState(StateType::FLYEND);
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
			SetState(StateType::IDLE);
			return;
		}
		else
		{
			SetState(StateType::FALLING);
			return;
		}
	}
}

void PlayerState::Falling(float _DeltaTime)
{
	ChangeAnimation("Falling");
	Gravity(_DeltaTime);
	// Black �� Yellow �߰�

	// FlyStart
	if (IsPressKey(VK_UP))
	{
		SetState(StateType::FLYSTART);
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
		SetState(StateType::IDLE);
		return;
	}
}

void PlayerState::Bend(float _DeltaTime)
{
	ChangeAnimation("Bend");

	// Slide
	if (IsPressKey('Z') || IsPressKey('X'))
	{
		SetState(StateType::SLIDE);
		return;
	}

	// Idle
	if (!IsPressKey(VK_DOWN))
	{
		SetState(StateType::IDLE);
		return; 
	}
}

void PlayerState::Slide(float _DeltaTime)
{
	ChangeAnimation("Slide");
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
		SetState(StateType::IDLE);
		return;
	}

	if (CurTime >= 0.8f)
	{
		CurTime = 0.0f;
		DirForce = FVector2D::ZERO;
		SetState(StateType::IDLE);
		return;
	}

}

void PlayerState::Climb(float _DeltaTime)
{
	if (IsPressKey(VK_UP))
	{
		ChangeAnimation("ClimbUp");

		if (CheckPointColor(CheckDir::DOWN, UColor::MAGENTA) || CheckPointColor(CheckDir::DOWN, UColor::YELLOW))
		{
			Move(FVector2D::UP * Speed * _DeltaTime);
		}
		if (!CheckPointColor(CheckDir::DOWN, UColor::YELLOW) && CheckPointColor(CheckDir::UP, UColor::WHITE))
		{
			SetState(StateType::IDLE);
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
			SetState(StateType::IDLE);
			return;
		}
	}
}
