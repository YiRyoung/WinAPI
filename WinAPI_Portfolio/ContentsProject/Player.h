#pragma once
#include <EngineCore/Actor.h>
#include <EngineBase/EngineMath.h>

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
	Fly,
	Flying,
	Jump,
	Bend,
	Slide,
	Climb,
	Attack
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

	// 좌우
	void SetAnimDir();

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

	// 스피드(가속도)
	inline void SetSpeed()
	{
		if (DirForce.Length() >= MaxSpeed)
		{
			// 길이를 1로 바꾼다.
			DirForce.Normalize();
			DirForce *= MaxSpeed;
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

protected:

private:
	float Speed = 300.0f;

	// 가속도
	float MaxSpeed = 300.0f;
	float MinSpeed = 0.1f;

	float JumpForce = 300.0f;
	int MySpriteIndex = 0;


	std::string AnimDir = "_Right";


	float DeAccSpeed = 5.0f;
	float AccSpeed  = 500.0f;
	FVector2D DirForce = FVector2D::ZERO;
	FVector2D GravityForce = FVector2D::ZERO;
	UColor CheckColor[static_cast<int>(CheckDir::Max)];

	class UEngineWinImage* BackImage = nullptr;
	class UEngineWinImage* ColImage = nullptr;
	class USpriteRenderer* SpriteRenderer;

	void ChangeState(PlayerState _CurPlayerState);
	void Idle(float _DeltaTime);
	void Move(float _DeltaTime);
	void Dash(float _DeltaTime);
	void Fly(float _DeltaTime);
	void Flying(float _DeltaTime);
	void Jump(float _DeltaTime);
	void Bend(float _DeltaTime);
	void Slide(float _DeltaTime);
	void Climb(float _DeltaTime);
};

