#pragma once
#include "Player.h"

class PlayerState
{
public:
	PlayerState();
	PlayerState(APlayer * _Player);
	~PlayerState();

	void Idle(float _DeltaTime);
	void Walk(float _DeltaTime);
	void Dash(float _DeltaTime);
	void Jump(float _DeltaTime);
	void FlyStart(float _DeltaTime);
	void Flying(float _DeltaTime);
	void FlyEnd(float _DeltaTime);
	void Falling(float _DeltaTime);
	void Bend(float _DeltaTime);
	void Slide(float _DeltaTime);
	void Climb(float _DeltaTime);
	void Attack(float _DeltaTime);
	void Hurt(float _DeltaTime);

protected:

private:
	bool IsFull = false;

	float CurTime = 0.0f;
	float DeAccSpeed = 10.0f;	// 관성
	float AccSpeed = 320.0f;	// 가속도
	float Speed = 180.0f;		// 평소 속도
	float MaxSpeed = 200;	// 최대 속도
	float JumpForce = 350.0f;	// 점프력

	FVector2D DirForce = FVector2D::ZERO;
	FVector2D GravityForce = FVector2D::ZERO;

	APlayer* Player = nullptr;
	StateType GetState() const
	{
		return Player->GetState();
	}
	void SetState(StateType _State);

	// 키 입력
	bool IsPressKey(int _KeyCode) const;
	bool IsDoubleKey(int _KeyCode, float _Count) const;

	// 애니메이션
	void ChangeAnimation(std::string _Anim);
	void SetAnimSpeed(float _Speed);
	bool IsAnimFinish();

	// 충돌 확인
	bool CheckPointColor(CheckDir _Dir, UColor _Color);
	bool CheckColor(CheckDir _Dir, UColor _Color);

	// 이동
	void Gravity(float _DeltaTime);
	void Move(FVector2D _NextPos);
	void SetLimitSpeed(bool _IsAccel);
};

