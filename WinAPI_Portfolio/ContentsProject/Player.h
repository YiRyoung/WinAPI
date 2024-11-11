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
	Move
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

	void GroundUp();

	void GroundDowm();

	PlayerState CurPlayerState = PlayerState::Idle;

protected:

private:
	float Speed = 500.0f;
	int MySpriteIndex = 0;

	int IsGround = false;
	FVector2D GravityForce = FVector2D::ZERO;

	class UEngineWinImage* BackImage = nullptr;
	class UEngineWinImage* ColImage = nullptr;
	class USpriteRenderer* SpriteRenderer;

	UColor CheckColor[static_cast<int>(CheckDir::Max)];

	void ChangeState(PlayerState _CurPlayerState);
	void Idle(float _DeltaTime);
	void Move(float _DeltaTime);

};

