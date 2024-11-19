#pragma once
#include <EngineCore/Actor.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/2DCollision.h>

enum class CheckDir
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

enum class StateType
{
	IDLE,
	WALK,
	DASH,
	FLYSTART,
	FLYING,
	FLYEND,
	JUMP,
	BEND,
	SLIDE,
	CLIMB,
	FALLING
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

	void GetBackImage(std::string_view _ImageName, std::string_view _ColImageName);

	inline std::string GetAnimDir() const
	{
		return AnimDir;
	}

	bool UpperPointCheck(UColor _Color);
	bool BottomPointCheck(UColor _Color);

	bool PixelLineColor(CheckDir _Dir, UColor _Color);

	bool IsAnimFinish()
	{
		if (true == SpriteRenderer->IsCurAnimationEnd())
		{
			return true;
		}
		return false;
	}
	void SetAnimSpeed(float _Speed)
	{
		SpriteRenderer->SetAnimationSpeed(_Speed);
	}
	void ChangeAnimation(std::string _Anim)
	{
		SpriteRenderer->ChangeAnimation(_Anim);
	}

	StateType GetState() const
	{
		return CurState;
	}

	void SetState(StateType _NextState)
	{
		CurState = _NextState;
	}

protected:

private:
	int MySpriteIndex = 0;
	std::string AnimDir = "_Right";
	StateType CurState = StateType::IDLE;
	U2DCollision* CollisionComponent;

	class PlayerState* State;
	class UEngineWinImage* BackImage = nullptr;
	class UEngineWinImage* ColImage = nullptr;
	class USpriteRenderer* SpriteRenderer;

	void SetPlayer();
	void SetAnimation();
	void SetAnimDir();
	void CameraMove();

	void FSM(float _DeltaTime);
};

