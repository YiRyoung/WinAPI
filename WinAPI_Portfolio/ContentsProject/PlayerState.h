#pragma once
#include "Player.h"

#include "EnginePlatform/EngineInput.h"

class PlayerState
{
public:
	// constrcuter destructer
	PlayerState();
	PlayerState(APlayer* _Player);
	~PlayerState();

	// delete Function
	PlayerState(const PlayerState& _Other) = delete;
	PlayerState(PlayerState&& _Other) noexcept = delete;
	PlayerState& operator=(const PlayerState& _Other) = delete;
	PlayerState& operator=(PlayerState&& _Other) noexcept = delete;

	//FSM Function
	void IdleStart(float _DeltaTime);
	void WalkStart(float _DeltaTime);
	void DashStart(float _DeltaTime);
	void JumpStart(float _DeltaTime);
	void FlyStart(float _DeltaTime);
	void Fly(float _DeltaTime);
	void FlyEnd(float _DeltaTime);
	void FallStart(float _DeltaTime);
	void BendStart(float _DeltaTime);
	void SlideStart(float _DeltaTime);
	void ClimbStart(float _DeltaTime);
	void EatStart(float _DeltaTime);
	void HurtStart(float _DeltaTime);

	// Attack
	void SkillStart(float _DeltaTime);
	void Skill(float _DeltaTime);
	void SkillEnd(float _DeltaTime);

	void InhaleStart(float _DeltaTime);
	void Inhale(float _DeltaTime);
	void InhaleEnd(float _DeltaTime);
	void Spit(float _DeltaTime);

	void BeamStart(float _DeltaTime);
	void Beam(float _DeltaTime);
	void CutterStart(float _DeltaTime);
	void Cutter(float _DeltaTime);
	//void FireStart(float _DeltaTime);
	//void Fire(float _DeltaTime);

protected:
	APlayer* Player = nullptr;

	// Wrapping Code
	bool IsDownKey(int _KeyCode) const
	{
		bool IsTrue = (true == UEngineInput::GetInst().IsDown(_KeyCode)) ? true : false;
		return IsTrue;
	}
	bool IsUpKey(int _KeyCode) const
	{
		bool IsTrue = (true == UEngineInput::GetInst().IsUp(_KeyCode)) ? true : false;
		return IsTrue;
	}
	bool IsPressKey(int _KeyCode) const
	{
		bool IsTrue = (true == UEngineInput::GetInst().IsPress(_KeyCode)) ? true : false;
		return IsTrue;
	}
	bool IsFreeKey(int _KeyCode) const
	{
		bool IsTrue = (true == UEngineInput::GetInst().IsFree(_KeyCode)) ? true : false;
		return IsTrue;
	}
	bool IsDoubleKey(int _KeyCode, float _TimeCount) const
	{
		bool IsTrue = (true == UEngineInput::GetInst().IsDoubleClick(_KeyCode, _TimeCount)) ? true : false;
		return IsTrue;
	}

	EPlayerState GetPlayerState() const
	{
		return Player->GetCurState();
	}
	void SetPlayerState(EPlayerState _ChangeState)
	{
		Player->SetCurState(_ChangeState);
	}

	bool GetPlayerFull() const
	{
		return Player->GetIsFull();
	}
	void SetPlayerFull(bool _IsFull)
	{
		return Player->SetIsFull(_IsFull);
	}

	void ChangeAnimation(std::string _NextAnim)
	{
		Player->ChangeAnimation(_NextAnim);
	}
	bool IsAnimFinish() const
	{
		bool IsTrue = (true == Player->GetPlayerRenderer()->IsCurAnimationEnd()) ? true : false;
		return IsTrue;
	}

private:
	void AddActorLocation(FVector2D _NextPos)
	{
		Player->AddActorLocation(_NextPos);
	}
	bool PixelPointCheck(ECheckDir _Dir, UColor _Color)
	{
		bool IsTrue = (true == Player->PixelPointCheck(_Dir, _Color)) ? true : false;
		return IsTrue;
	}
	bool PixelLineCheck(ECheckDir _Dir, UColor _Color)
	{
		bool IsTrue = (true == Player->PixelLineCheck(_Dir, _Color)) ? true : false;
		return IsTrue;
	}

	//FSM Function
	bool IsAccel = false;
	float CurTime = 0.0f;
	float Speed = 200.0f;		// 기본 속도
	float AccSpeed = 320.0f;	// 가속도
	float DeAccSpeed = 10.0f;	// 관성
	float MaxSpeed = 230.0f;	// 최대 속도
	FVector2D DirForce = FVector2D::ZERO;		// 가속
	void SetLimitAccel(bool _IsDeAcc, float MaxSpeed = 200.0f);

	float JumpPower = 280.0f;
	float GravityPower = 500.0f;				// 기본 중력
	FVector2D GravityForce = FVector2D::ZERO;	// 중력 가속도
	void Gravity(float _DeltaTime);

	void Idle(float _DeltaTime);
	void Walk(float _DeltaTime);
	void Dash(float _DeltaTime);
	void Jump(float _DeltaTime);
	void Fall(float _DeltaTime);
	void Bend(float _DeltaTime);
	void Slide(float _DeltaTime);
	void Climb(float _DeltaTime);
	void Eat(float _DeltaTime);
	void Hurt(float _DeltaTime);

	void ResetDirForce()
	{
		if (IsAccel)
		{
			DirForce = FVector2D::ZERO;
			IsAccel = false;
		}
	}
	void ChangeIdle();
	void ChangeWalkAndDash();
	void ChangeJump();
	void ChangeFly();
	void ChangeFall();
	void ChangeBend();
	void ChangeClimb();
	void ChangeEat();
	void ChangeAttack();

};

