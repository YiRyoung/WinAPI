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

	void PlayerGroundCheck(FVector2D _MovePos);

	void Gravity(float _DeltaTime);

	void SetAnimDir();
	
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

	PlayerState CurPlayerState = PlayerState::Idle;

protected:

private:
	float Speed = 300.0f;
	float JumpForce = 280.0f;
	int MySpriteIndex = 0;

	FVector2D GravityForce = FVector2D::ZERO;

	class UEngineWinImage* BackImage = nullptr;
	class UEngineWinImage* ColImage = nullptr;
	class USpriteRenderer* SpriteRenderer;

	UColor CheckColor[static_cast<int>(CheckDir::Max)];

	std::string AnimDir = "_Right";

	void ChangeState(PlayerState _CurPlayerState);
	void Idle(float _DeltaTime);
	void Move(float _DeltaTime);
	void Dash(float _DeltaTime);
	void Fly(float _DeltaTime);
	void Jump(float _DeltaTime);
	void Bend(float _DeltaTime);
	void Slide(float _DeltaTime);
	void Climb(float _DeltaTime);
};

