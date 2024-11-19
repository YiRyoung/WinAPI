#pragma once
#include <EngineCore/Actor.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/2DCollision.h>

#include "ContentsEnum.h"

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
	FALLING,
	ATTACK,
	HURT
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

	bool UpperPointCheck(UColor _Color);
	bool BottomPointCheck(UColor _Color);
	bool PixelLineColor(CheckDir _Dir, UColor _Color);

	StateType GetState() const
	{
		return CurState;
	}

	void SetState(StateType _NextState)
	{
		CurState = _NextState;
	}

	void CollisionEnter(AActor* _ColActor);

protected:

private:
	int MySpriteIndex = 0;
	std::string AnimDir = "_Right";
	U2DCollision* CollisionComponent;

	StateType CurState = StateType::IDLE;
	EAblityType CurType = EAblityType::NORMAL;

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

