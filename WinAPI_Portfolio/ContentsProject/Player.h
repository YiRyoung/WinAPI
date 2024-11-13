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
	Swim,
	Attack
};

enum class PlayerAbility
{
	Common,
	HaveBullet,
	Beam,
	Cutter,
	Fire,
	Spark,
	Sword
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

	void GetBackImage(std::string_view _ImageName, std::string_view _CollierName);

	void CameraMove();

	void PlayerGroundCheck(FVector2D _MovePos);

	void Gravity(float _DeltaTime);

	void SetAnimDir()
	{
		if (true == IsLeft)
		{
			AnimDir = "_Left";
		}
		else
		{
			AnimDir = "_Right";
		}
	}

	bool CheckMAGENTA(UColor _Color)
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

	PlayerState CurPlayerState = PlayerState::Idle;

protected:

private:
	float Speed = 300.0f;
	float JumpForce = 280.0f;
	int MySpriteIndex = 0;

	bool IsMAGENTA = false;

	FVector2D GravityForce = FVector2D::ZERO;

	class UEngineWinImage* BackImage = nullptr;
	class UEngineWinImage* ColImage = nullptr;
	class USpriteRenderer* SpriteRenderer;

	UColor CheckColor[static_cast<int>(CheckDir::Max)];

	bool IsLeft = false;
	std::string AnimDir = "";

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
};

