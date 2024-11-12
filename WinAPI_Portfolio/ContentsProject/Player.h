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

	void DirCheck()
	{
		if (true == IsLeft)
		{
			DirString = "_Left";
		}
		
		DirString = "_Right";
	}

	PlayerState CurPlayerState = PlayerState::Idle;

protected:

private:
	float Speed = 300.0f;
	float JumpForce = 500.0f;
	int MySpriteIndex = 0;

	bool IsWall = false;
	FVector2D GravityForce = FVector2D::ZERO;

	class UEngineWinImage* BackImage = nullptr;
	class UEngineWinImage* ColImage = nullptr;
	class USpriteRenderer* SpriteRenderer;

	UColor CheckColor[static_cast<int>(CheckDir::Max)];

	bool IsLeft = false;
	std::string DirString = "";

	void ChangeState(PlayerState _CurPlayerState);
	void Idle(float _DeltaTime);
	void Move(float _DeltaTime);
	void Fly(float _DeltaTime);
	void Jump(float _DeltaTime);
	void Bend(float _DeltaTime);
};

