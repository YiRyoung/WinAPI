#include "PreCompile.h"
#include "PlayerState.h"

#include "FireBall.h"

PlayerState::PlayerState()
{
}

PlayerState::PlayerState(APlayer* _Player) : Player(_Player)
{
}

PlayerState::~PlayerState()
{
}

void PlayerState::SetLimitAccel(bool _IsDeAcc, float MaxSpeed)
{
	switch (_IsDeAcc)
	{
	case true:	// ZeroSpeed
		if (DirForce.Length() <= 0.01f)
		{
			DirForce.X = 0.0f;
		}
		break;
	case false:	// MaxSpeed
		if (DirForce.Length() >= MaxSpeed)
		{
			DirForce.Normalize();
			DirForce.X *= MaxSpeed;
		}
		break;
	}
}

void PlayerState::Gravity(float _DeltaTime)
{
	if (!PixelLineCheck(ECheckDir::DOWN, UColor::MAGENTA) && !PixelLineCheck(ECheckDir::DOWN, UColor::BLACK)
		&& !PixelPointCheck(ECheckDir::DOWN, UColor::YELLOW))
	{
		AddActorLocation(GravityForce * _DeltaTime);
		GravityForce += FVector2D::DOWN * GravityPower * _DeltaTime;
	}
	else
	{
		GravityForce = FVector2D::ZERO;
	}
}

void PlayerState::IdleStart(float _DeltaTime)
{
	if (GetPlayerFull() && "_Left" == Player->GetAnimDir())
	{
		Player->GetPlayerRenderer()->SetSprite("Kirby_Normal_Left.png", 30);
	}
	else if (GetPlayerFull() && "_Right" == Player->GetAnimDir())
	{
		Player->GetPlayerRenderer()->SetSprite("Kirby_Normal_Right.png", 30);
	}
	else
	{
		ChangeAnimation("Idle");
	}

	Gravity(_DeltaTime);
	Idle(_DeltaTime);
}

void PlayerState::Idle(float _DeltaTime)
{
	ChangeWalkAndDash();
	ChangeJump();
	ChangeFly();
	ChangeBend();
	ChangeClimb();
	ChangeFall();
	ChangeEat();
	ChangeAttack();

	// DeAccel
	if (!PixelLineCheck(ECheckDir::LEFT, UColor::MAGENTA) && !PixelLineCheck(ECheckDir::RIGHT, UColor::MAGENTA))
	{
		DirForce.X += -DirForce.X * DeAccSpeed * _DeltaTime;
		SetLimitAccel(true);
		AddActorLocation(DirForce * _DeltaTime);
	}
}

void PlayerState::WalkStart(float _DeltaTime)
{
	if (GetPlayerFull())
	{
		ChangeAnimation("WalkFull");
	}
	else
	{
		ChangeAnimation("Walk");
	}

	Gravity(_DeltaTime);
	Walk(_DeltaTime);
}

void PlayerState::Walk(float _DeltaTime)
{
	ChangeJump();
	ChangeFly();
	ChangeBend();
	ChangeClimb();
	ChangeIdle();
	ChangeEat();
	ChangeAttack();

	// Walk
	{
		float NewSpeed = (true == GetPlayerFull()) ? Speed * 0.5f : Speed;
		FVector2D Vector = FVector2D::ZERO;
		ECheckDir Direction = ECheckDir::MAX;

		if (IsPressKey(VK_LEFT))
		{
			Vector = FVector2D::LEFT;
			Direction = ECheckDir::LEFT;

			if (!PixelLineCheck(Direction, UColor::MAGENTA))
			{
				AddActorLocation(Vector * NewSpeed * _DeltaTime);
			}
		}
		if (IsPressKey(VK_RIGHT))
		{
			Vector = FVector2D::RIGHT;
			Direction = ECheckDir::RIGHT;

			if (!PixelLineCheck(Direction, UColor::MAGENTA))
			{
				AddActorLocation(Vector * NewSpeed * _DeltaTime);
			}
		}
	}
}

void PlayerState::DashStart(float _DeltaTime)
{
	if (GetPlayerFull())
	{
		ChangeAnimation("DashFull");
	}
	else
	{
		ChangeAnimation("Dash");
	}

	IsAccel = true;
	Gravity(_DeltaTime);
	Dash(_DeltaTime);
}

void PlayerState::Dash(float _DeltaTime)
{
	ChangeJump();
	ChangeClimb();
	ChangeFly();
	ChangeBend();
	ChangeIdle();
	ChangeEat();
	ChangeAttack();

	// Dash
	{
		float NewMaxSpeed = (true == GetPlayerFull()) ? MaxSpeed * 0.7f : MaxSpeed;
		FVector2D Vector = FVector2D::ZERO;
		
		if (IsPressKey(VK_LEFT))
		{
			Vector = FVector2D::LEFT;
		}
		if (IsPressKey(VK_RIGHT))
		{
			Vector = FVector2D::RIGHT;
		}

		DirForce += Vector * AccSpeed * _DeltaTime;
		SetLimitAccel(false, NewMaxSpeed);

		if (!PixelLineCheck(ECheckDir::LEFT, UColor::MAGENTA) && !PixelLineCheck(ECheckDir::RIGHT, UColor::MAGENTA))
		{
			AddActorLocation(DirForce * _DeltaTime);
		}
	}
}

void PlayerState::JumpStart(float _DeltaTime)
{
	float Force = JumpPower - GravityForce.Y;

	if (0.0f < Force)
	{
		if (GetPlayerFull())
		{
			if ("_Left" == Player->GetAnimDir())
			{
				Player->GetPlayerRenderer()->SetSprite("Kirby_Normal_Left.png", 31);
			}
			else if ("_Right" == Player->GetAnimDir())
			{
				Player->GetPlayerRenderer()->SetSprite("Kirby_Normal_Right.png", 31);
			}
		}
		else
		{
			if ("_Left" == Player->GetAnimDir())
			{
				Player->GetPlayerRenderer()->SetSprite("Kirby_Normal_Left.png", 11);
			}
			else if ("_Right" == Player->GetAnimDir())
			{
				Player->GetPlayerRenderer()->SetSprite("Kirby_Normal_Right.png", 11);
			}
		}
	}
	else
	{
		if (GetPlayerFull())
		{
			ChangeAnimation("JumpFull");
		}
		else
		{
			ChangeAnimation("Jump");
		}
	}

	Gravity(_DeltaTime);
	Jump(_DeltaTime);
}

void PlayerState::Jump(float _DeltaTime)
{
	ChangeFly();
	ChangeAttack();

	// Jumping
	{
		FVector2D Vector = FVector2D::UP;

		if (IsPressKey(VK_LEFT) && !PixelLineCheck(ECheckDir::LEFT, UColor::MAGENTA))
		{
			Vector += FVector2D::LEFT * 0.5f;
		}
		if (IsPressKey(VK_RIGHT) && !PixelLineCheck(ECheckDir::RIGHT, UColor::MAGENTA))
		{
			Vector += FVector2D::RIGHT * 0.5f;
		}
		DirForce.Normalize();
		AddActorLocation(Vector * JumpPower * _DeltaTime);

		if (PixelLineCheck(ECheckDir::DOWN, UColor::MAGENTA) || PixelLineCheck(ECheckDir::UP, UColor::MAGENTA)
			|| PixelLineCheck(ECheckDir::DOWN, UColor::BLACK) || PixelLineCheck(ECheckDir::DOWN, UColor::YELLOW))
		{
			SetPlayerState(EPlayerState::IDLE);
			return;
		}
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
	if (IsPressKey(VK_LEFT) && !PixelLineCheck(ECheckDir::LEFT, UColor::MAGENTA))
	{
		Vector += FVector2D::LEFT;
	}
	if (IsPressKey(VK_RIGHT) && !PixelLineCheck(ECheckDir::RIGHT, UColor::MAGENTA))
	{
		Vector += FVector2D::RIGHT;
	}
	Vector.Normalize();

	if (!PixelLineCheck(ECheckDir::UP, UColor::MAGENTA))
	{
		AddActorLocation(Vector * (Speed * 0.7f) * _DeltaTime);
	}

	if (IsAnimFinish())
	{
		SetPlayerState(EPlayerState::FLY);
		return;
	}
}

void PlayerState::Fly(float _DeltaTime)
{
	ChangeAnimation("Flying");

	if (IsPressKey('X'))
	{
		Player->GetPlayerRenderer()->ResetAnimationSpeed();
		ChangeAnimation("FlyEnd");
		SetPlayerState(EPlayerState::FLYEND);
		return;
	}

	FVector2D Vector = FVector2D::ZERO;

	if (IsPressKey(VK_UP) && !PixelLineCheck(ECheckDir::UP, UColor::MAGENTA))	// Fly High!
	{
		GravityForce = FVector2D::ZERO;
		Player->GetPlayerRenderer()->SetAnimationSpeed(5.0f);
		Vector += FVector2D::UP;
	}
	else					// Landing
	{
		Player->GetPlayerRenderer()->ResetAnimationSpeed();
		if (!PixelLineCheck(ECheckDir::DOWN, UColor::MAGENTA) && !PixelLineCheck(ECheckDir::DOWN, UColor::BLACK)
			&& !PixelLineCheck(ECheckDir::DOWN, UColor::YELLOW))
		{
			Vector += FVector2D::DOWN;
		}
	}

	if (IsPressKey(VK_LEFT) && !PixelLineCheck(ECheckDir::LEFT, UColor::MAGENTA))
	{
		Vector += FVector2D::LEFT;
	}
	if (IsPressKey(VK_RIGHT) && !PixelLineCheck(ECheckDir::RIGHT, UColor::MAGENTA))
	{
		Vector += FVector2D::RIGHT;
	}

	Vector.Normalize();

	AddActorLocation(Vector * (Speed * 0.7f) * _DeltaTime);
}

void PlayerState::FlyEnd(float _DeltaTime)
{
	if (IsAnimFinish())
	{
		Player->SpawnWind();

		if (PixelLineCheck(ECheckDir::DOWN, UColor::MAGENTA) || PixelLineCheck(ECheckDir::DOWN, UColor::BLACK)
			|| PixelLineCheck(ECheckDir::DOWN, UColor::YELLOW))
		{
			SetPlayerState(EPlayerState::IDLE);
			return;
		}
		else
		{
			SetPlayerState(EPlayerState::FALL);
			return;
		}
	}
}

void PlayerState::FallStart(float _DeltaTime)
{
	if (!GetPlayerFull())
	{
		if ("_Left" == Player->GetAnimDir())
		{
			Player->GetPlayerRenderer()->SetSprite("Kirby_Normal_Left.png", 15);
		}
		else
		{
			Player->GetPlayerRenderer()->SetSprite("Kirby_Normal_Right.png", 15);
		}
	}
	else
	{
		if ("_Left" == Player->GetAnimDir())
		{
			Player->GetPlayerRenderer()->SetSprite("Kirby_Normal_Left.png", 31);
		}
		else
		{
			Player->GetPlayerRenderer()->SetSprite("Kirby_Normal_Right.png", 31);
		}
	}

	Gravity(_DeltaTime);
	Fall(_DeltaTime);
}

void PlayerState::Fall(float _DeltaTime)
{
	ChangeFly();

	if (PixelLineCheck(ECheckDir::DOWN, UColor::MAGENTA) || PixelLineCheck(ECheckDir::DOWN, UColor::BLACK)
		|| PixelPointCheck(ECheckDir::DOWN, UColor::YELLOW) || PixelLineCheck(ECheckDir::LEFT, UColor::MAGENTA)
		|| PixelLineCheck(ECheckDir::RIGHT, UColor::MAGENTA))
	{
		SetPlayerState(EPlayerState::IDLE);
		return;
	}

	FVector2D Vector = FVector2D::ZERO;

	if (IsPressKey(VK_LEFT) && !PixelLineCheck(ECheckDir::LEFT, UColor::MAGENTA))
	{
		Vector += FVector2D::LEFT;
	}
	if (IsPressKey(VK_RIGHT) && !PixelLineCheck(ECheckDir::RIGHT, UColor::MAGENTA))
	{
		Vector += FVector2D::RIGHT;
	}
	Vector.Normalize();	
	AddActorLocation(Vector * Speed * _DeltaTime);
}

void PlayerState::BendStart(float _DeltaTime)
{
	if ("_Left" == Player->GetAnimDir())
	{
		Player->GetPlayerRenderer()->SetSprite("Kirby_Normal_Left.png", 47);
	}
	else
	{
		Player->GetPlayerRenderer()->SetSprite("Kirby_Normal_Right.png", 47);
	}
	Bend(_DeltaTime);
}

void PlayerState::Bend(float _DeltaTime)
{
	if (!IsPressKey(VK_DOWN))
	{
		SetPlayerState(EPlayerState::IDLE);
		return;
	}

	if (IsPressKey('Z') || IsPressKey('X'))
	{
		SetPlayerState(EPlayerState::SLIDE);
		return;
	}

	// DeAccel
	if (!PixelLineCheck(ECheckDir::LEFT, UColor::MAGENTA) && !PixelLineCheck(ECheckDir::RIGHT, UColor::MAGENTA))
	{
		DirForce.X += -DirForce.X * DeAccSpeed * _DeltaTime;
		SetLimitAccel(true);
		AddActorLocation(DirForce * _DeltaTime);
	}
}

void PlayerState::SlideStart(float _DeltaTime)
{
	if ("_Left" == Player->GetAnimDir())
	{
		Player->GetPlayerRenderer()->SetSprite("Kirby_Normal_Left.png", 7);
	}
	else
	{
		Player->GetPlayerRenderer()->SetSprite("Kirby_Normal_Right.png", 7);
	}

	CurTime += _DeltaTime;
	Player->SliderCollisionSwitch(true);
	Gravity(_DeltaTime);
	Slide(_DeltaTime);
}

void PlayerState::Slide(float _DeltaTime)
{
	if ("_Left" == Player->GetAnimDir())
	{
		AddActorLocation(FVector2D::LEFT * Speed * _DeltaTime);
	}

	if ("_Right" == Player->GetAnimDir())
	{
		AddActorLocation(FVector2D::RIGHT * Speed * _DeltaTime);
	}

	if (CurTime >= 0.8f || PixelLineCheck(ECheckDir::LEFT, UColor::MAGENTA) || PixelLineCheck(ECheckDir::RIGHT, UColor::MAGENTA))
	{
		CurTime = 0.0f;
		Player->SliderCollisionSwitch(false);
		SetPlayerState(EPlayerState::IDLE);
		return;
	}
}

void PlayerState::ClimbStart(float _DeltaTime)
{
	if (IsPressKey(VK_UP))
	{
		ChangeAnimation("ClimbUp");
	}
	else
	{
		Player->GetPlayerRenderer()->SetSprite("Kirby_Normal_Left.png", 52);
	}

	if (IsPressKey(VK_DOWN))
	{
		Player->GetPlayerRenderer()->SetSprite("Kirby_Normal_Left.png", 54);
	}

	Climb(_DeltaTime);
}

void PlayerState::Climb(float _DeltaTime)
{
	if (IsPressKey(VK_LEFT) || IsPressKey(VK_RIGHT))
	{
		SetPlayerState(EPlayerState::FALL);
		return;
	}

	if (IsPressKey(VK_UP))
	{
		if (PixelPointCheck(ECheckDir::DOWN, UColor::MAGENTA) || PixelPointCheck(ECheckDir::DOWN, UColor::YELLOW)
			|| PixelPointCheck(ECheckDir::DOWN, UColor::BLACK))
		{
			AddActorLocation(FVector2D::UP * Speed * _DeltaTime);
		}
		if (!PixelPointCheck(ECheckDir::DOWN, UColor::YELLOW) && PixelPointCheck(ECheckDir::UP, UColor::WHITE))
		{
			SetPlayerState(EPlayerState::IDLE);
			return;
		}
	}

	if (IsPressKey(VK_DOWN))
	{
		if (PixelPointCheck(ECheckDir::UP, UColor::WHITE) || PixelPointCheck(ECheckDir::UP, UColor::YELLOW))
		{
			AddActorLocation(FVector2D::DOWN * Speed * _DeltaTime);
		}
		if (PixelPointCheck(ECheckDir::DOWN, UColor::MAGENTA))
		{
			SetPlayerState(EPlayerState::IDLE);
			return;
		}
	}
}

void PlayerState::EatStart(float _DeltaTime)
{
	ChangeAnimation("Eat");

	if (IsAnimFinish())
	{
		Eat(_DeltaTime);
	}
}

void PlayerState::Eat(float _DeltaTime)
{
	SetPlayerFull(false);

	if (EAbilityType::NORMAL == Player->GetCurMonsterAbility())
	{
	}
	else
	{
	}

	Player->SetCurAbility(Player->GetCurMonsterAbility());
	Player->SetCurMosnterAbility(EAbilityType::MAX);
	SetPlayerState(EPlayerState::IDLE);
	return;
}

void PlayerState::HurtStart(float _DeltaTime)
{
	Player->GetPlayerRenderer()->ResetAnimationSpeed();

	if (GetPlayerFull())
	{
		ChangeAnimation("HurtFull");

	}
	else
	{
		ChangeAnimation("Hurt");
	}

	Hurt(_DeltaTime);
}

void PlayerState::Hurt(float _DeltaTime)
{
	Gravity(_DeltaTime);
	CurTime += _DeltaTime;
	FVector2D Vector = FVector2D::ZERO;

	// Force »ç¿ë
	if ("_Left" == Player->GetAnimDir() && !PixelLineCheck(ECheckDir::RIGHT, UColor::MAGENTA))
	{
		Vector += FVector2D::RIGHT;
	}
	else if ("_Right" == Player->GetAnimDir() && !PixelLineCheck(ECheckDir::LEFT, UColor::MAGENTA))
	{
		Vector += FVector2D::LEFT;
	}
	Vector.Normalize();
	
	AddActorLocation(Vector * 100.0f * _DeltaTime);

	if (CurTime >= 0.5f)
	{
		CurTime = 0.0f;
		Player->SetCanHurt(true);
		SetPlayerState(EPlayerState::IDLE);
		return;
	}
}

void PlayerState::InhaleStart(float _DeltaTime)
{
	ChangeAnimation("InhaleStart");
	Player->SkillBoxCollisionSwitch(true);

	if (IsAnimFinish())
	{
		SetPlayerState(EPlayerState::INHALE);
		return;
	}
}

void PlayerState::Inhale(float _DeltaTime)
{
	Gravity(_DeltaTime);

	if (!IsPressKey('X'))
	{
		ChangeAnimation("InhaleEnd");
		Player->SkillBoxCollisionSwitch(false);
		SetPlayerFull(false);

		if (IsAnimFinish())
		{
			InhaleEnd(_DeltaTime);
		}
	}
}

void PlayerState::InhaleEnd(float _DeltaTime)
{
	SetPlayerState(EPlayerState::IDLE);
	return;
}

void PlayerState::Spit(float _DeltaTime)
{
	ChangeAnimation("Spit");

	if (IsAnimFinish())
	{
		SetPlayerFull(false);
		SetPlayerState(EPlayerState::IDLE);
		return;
	}
}

void PlayerState::BeamStart(float _DeltaTime)
{
	ChangeAnimation("Beam");
	Player->SpawnBeam();
	Gravity(_DeltaTime);
	SetPlayerState(EPlayerState::SKILL);
	return;
}

void PlayerState::Beam(float _DeltaTime)
{
	CurTime += _DeltaTime;

	if (CurTime >= 0.18f)
	{
		CurTime = 0.0f;
		SetPlayerState(EPlayerState::IDLE);
		return;
	}
}

void PlayerState::CutterStart(float _DeltaTime)
{
	CurTime += _DeltaTime;
	Gravity(_DeltaTime);
	ChangeAnimation("Cutter");
	
	if (CurTime >= 0.2f)
	{
		CurTime = 0.0f;
		SetPlayerState(EPlayerState::SKILL);
		return;
	}
}

void PlayerState::Cutter(float _DeltaTime)
{
	Player->SpawnSlide();
	SetPlayerState(EPlayerState::IDLE);
	return;
}

void PlayerState::FireStart(float _DeltaTime)
{
	Gravity(_DeltaTime);
	ChangeAnimation("Fire");
	Player->SpawnFire();
	SetPlayerState(EPlayerState::SKILL);
	return;
}

void PlayerState::Fire(float _DeltaTime)
{
	if (IsUpKey('X'))
	{
		(Player->NewFireBall)->Destroy();
		SetPlayerState(EPlayerState::IDLE);
		return;
	}
}

void PlayerState::SkillStart(float _DeltaTime)
{
	// Animation
	switch (Player->GetCurAbility())
	{
	case EAbilityType::BEAM:
		BeamStart(_DeltaTime);
		break;
	case EAbilityType::CUTTER:
		CutterStart(_DeltaTime);
		break;
	case EAbilityType::FIRE:
		FireStart(_DeltaTime);
		break;
	}
}

void PlayerState::Skill(float _DeltaTime)
{
	switch (Player->GetCurAbility())
	{
	case EAbilityType::BEAM:
		Beam(_DeltaTime);
		break;
	case EAbilityType::CUTTER:
		Cutter(_DeltaTime);
		break;
	case EAbilityType::FIRE:
		Fire(_DeltaTime);
		break;
	}
}

void PlayerState::ChangeIdle()
{
	if (!IsPressKey(VK_LEFT) && !IsPressKey(VK_RIGHT))
	{
		ResetDirForce();
		SetPlayerState(EPlayerState::IDLE);
		return;
	}
}

void PlayerState::ChangeWalkAndDash()
{
	// Move & Dash
	if (IsDoubleKey(VK_LEFT, 0.2f) || IsDoubleKey(VK_RIGHT, 0.2f))
	{
		SetPlayerState(EPlayerState::DASH);
		return;
	}
	else if (IsPressKey(VK_LEFT) || IsPressKey(VK_RIGHT))
	{
		SetPlayerState(EPlayerState::WALK);
		return;
	}
}

void PlayerState::ChangeJump()
{
	// Jump
	if (EPlayerState::BEND != GetPlayerState() && IsPressKey('Z'))
	{
		ResetDirForce();
		SetPlayerState(EPlayerState::JUMP);
		return;
	}
}

void PlayerState::ChangeFly()
{
	// Fly
	if (IsPressKey(VK_UP) && !GetPlayerFull())
	{
		ResetDirForce();
		SetPlayerState(EPlayerState::FLYSTART);
		return;
	}
}

void PlayerState::ChangeFall()
{
	// Fall
	if (!PixelLineCheck(ECheckDir::DOWN, UColor::MAGENTA) && !PixelLineCheck(ECheckDir::DOWN, UColor::BLACK) 
		&& !PixelLineCheck(ECheckDir::DOWN, UColor::YELLOW))
	{
		ResetDirForce();
		SetPlayerState(EPlayerState::FALL);
		return;
	}
}

void PlayerState::ChangeBend()
{
	// Bend
	if (IsPressKey(VK_DOWN) && !GetPlayerFull()
		&& !PixelPointCheck(ECheckDir::DOWN, UColor::YELLOW))
	{
		SetPlayerState(EPlayerState::BEND);
		return;
	}
}

void PlayerState::ChangeClimb()
{
	if ((IsPressKey(VK_UP) && PixelPointCheck(ECheckDir::UP, UColor::YELLOW))
		|| (IsPressKey(VK_DOWN) && PixelPointCheck(ECheckDir::DOWN, UColor::YELLOW)))
	{
		SetPlayerState(EPlayerState::CLIMB);
		return;
	}
}

void PlayerState::ChangeEat()
{
	// Eat
	if (IsPressKey(VK_DOWN) && GetPlayerFull())
	{
		SetPlayerState(EPlayerState::EAT);
		return;
	}
}

void PlayerState::ChangeAttack()
{
	if (IsDownKey('X'))
	{
		GravityForce = FVector2D::ZERO;

		if (EAbilityType::NORMAL == Player->GetCurAbility() && !GetPlayerFull())
		{

			SetPlayerState(EPlayerState::INHALESTART);
			return;
		}
		else if (EAbilityType::NORMAL == Player->GetCurAbility() && GetPlayerFull())
		{
			Player->SpawnSpit();
			SetPlayerState(EPlayerState::SPIT);
			return;
		}
		else
		{
			SetPlayerState(EPlayerState::SKILLSTART);
			return;
		}
	}
}