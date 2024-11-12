#pragma once
#include <EngineCore/Actor.h>
#include <EngineBase/EngineMath.h>
enum class CheckDir
{
	Left,
	Right,
	Up,
	Down,
	Max
};

enum class PlayerState
{
	Idle,
	Move,
	Fly,
	Jump,
	Bend
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
	float JumpForce = 500.0f;
	int MySpriteIndex = 0;

	bool IsMAGENTA = false;

	FVector2D GravityForce = FVector2D::ZERO;

	class UEngineWinImage* BackImage = nullptr;
	class UEngineWinImage* ColImage = nullptr;
	class USpriteRenderer* SpriteRenderer;

	UColor CheckColor[static_cast<int>(CheckDir::Max)];

	bool IsLeft = true;
	std::string AnimDir = "";

	void ChangeState(PlayerState _CurPlayerState);
	void Idle(float _DeltaTime);
	void Move(float _DeltaTime);
	void Fly(float _DeltaTime);
	void Jump(float _DeltaTime);
	void Bend(float _DeltaTime);
};

