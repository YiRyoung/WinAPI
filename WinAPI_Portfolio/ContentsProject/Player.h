#pragma once
#include <EngineCore/Actor.h>
#include <EngineBase/EngineMath.h>
#include <EnginePlatform/EngineInput.h>

enum class CheckDir
{
	Up,
	Down,
	Left,
	Right,
	LeftUp,
	RightUp,
	LeftDown,
	RightDown,
	Max
};

enum class PlayerState
{
	Idle,
	Move,
	Dash,
	FlyStart,
	Flying,
	FlyEnd,
	Jump,
	Bend,
	Slide,
	Climb,
	Fall
};

class APlayer : public AActor
{
public:
	// constrcuter destructer
	APlayer();
	~APlayer();

	// delete Function
	APlayer(const APlayer& _Other) = delete;
	APlayer(APlayer&& _Other) noexcept = delete;
	APlayer& operator=(const APlayer& _Other) = delete;
	APlayer& operator=(APlayer&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void SetAnim();

	void GetBackImage(std::string_view _ImageName, std::string_view _CollierName);

	void CameraMove();

	void Gravity(float _DeltaTime);
	
	// 키 입력
	inline bool IsPressKey(int _KeyCode) const
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

	inline bool IsDoubleKey(int _KeyCode, float _Count) const
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

	// 좌우
	inline void SetAnimDir()
	{
		if (UEngineInput::GetInst().IsPress(VK_LEFT))
		{
			AnimDir = "_Left";
		}
		else if (UEngineInput::GetInst().IsPress(VK_RIGHT))
		{
			AnimDir = "_Right";
		}
	};

	inline bool GetDirLeft() const
	{
		if (AnimDir == "_Right")
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	// 스피드(가속도 제한)
	inline void SetLimitSpeed(bool _LimitType)
	{
		switch (_LimitType)
		{
		case true:	// MaxSpeed
			if (DirForce.Length() >= MaxSpeed)
			{
				DirForce.Normalize();
				DirForce *= MaxSpeed;
			}
			break;
		case false:	// ZeroSpeed
			if (DirForce.Length() <= 0.01f)
			{
				DirForce.X = 0.0f;
			}
			break;
		}
	}

	// 충돌 색상 체크
	void PlayerGroundCheck(FVector2D _MovePos);

	inline bool CheckMAGENTA(UColor _Color)
	{
		if (UColor::MAGENTA == _Color)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	inline bool CheckYELLOW(UColor _Color)
	{
		if (UColor::YELLOW == _Color)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	inline bool CheckBLACK(UColor _Color)
	{
		if (UColor::BLACK == _Color)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	inline bool CheckWHITE(UColor _Color)
	{
		if (UColor::WHITE == _Color)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	PlayerState CurPlayerState = PlayerState::Idle;

	// 현재 상태 비교
	inline bool CheckState(PlayerState _State)
	{
		if (CurPlayerState == _State)
		{
			return true;
		}
		return false;
	}

protected:

private:
	float DeAccSpeed = 10.0f;
	float AccSpeed  = 500.0f;
	float Speed = 180.0;
	float MaxSpeed = 300.0f;
	FVector2D DirForce = FVector2D::ZERO;

	float JumpForce = 300.0f;
	int MySpriteIndex = 0;


	std::string AnimDir = "_Right";

	FVector2D GravityForce = FVector2D::ZERO;
	UColor CheckColor[static_cast<int>(CheckDir::Max)];

	class UEngineWinImage* BackImage = nullptr;
	class UEngineWinImage* ColImage = nullptr;
	class USpriteRenderer* SpriteRenderer;

	void ChangeState(PlayerState _CurPlayerState);
	void Idle(float _DeltaTime);
	void Move(float _DeltaTime);
	void Dash(float _DeltaTime);
	void FlyStart(float _DeltaTime);
	void Flying(float _DeltaTime);
	void FlyEnd(float _DeltaTime);
	void Jump(float _DeltaTime);
	void Bend(float _DeltaTime);
	void Slide(float _DeltaTime);
	void Climb(float _DeltaTime);
	void Fall(float _DeltaTime);
};

