#pragma once
#include <EngineCore/Actor.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/2DCollision.h>

#include "ContentsEnum.h"

enum class EStateType
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
	INHALESTART,
	INHALEEND,	// Spit
	SKILL,
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

	// Collider
	void SetSlide(bool _OnOff)
	{
		if ("_Right" == AnimDir)
		{
			SlideRightCollision->SetActive(_OnOff);
		}
		else
		{
			SlideLeftCollision->SetActive(_OnOff);
		}
	}
	void SetInhale(bool _OnOff)
	{
		if ("_Right" == AnimDir)
		{
			InhaleRightCollision->SetActive(_OnOff);
		}
		else
		{
			InhaleLeftCollision->SetActive(_OnOff);
		}
	}

	void CollisionEnter(AActor* _ColActor);
	void CollisionStay(AActor* _ColActor);

	EStateType GetState() const
	{
		return CurState;
	}
	void SetState(EStateType _NextState)
	{
		CurState = _NextState;
	}

	EAblityType GetAbility() const
	{
		return CurType;
	}
	void SetAbility(EAblityType _NewTpye)
	{
		CurType = _NewTpye;
	}

	class U2DCollision* CollisionComponent = nullptr;

	inline bool GetFull() const
	{
		return IsFull;
	}
	inline void SetFull(bool _IsFull)
	{
		IsFull = _IsFull;
	}

protected:

private:
	int MySpriteIndex = 0;
	std::string AnimDir = "_Right";

	EStateType CurState = EStateType::IDLE;
	EAblityType CurType = EAblityType::NORMAL;
	EAblityType EatType = EAblityType::NORMAL;

	class PlayerState* State;
	class PlayerAbility* Ability;

	class UEngineWinImage* BackImage = nullptr;
	class UEngineWinImage* ColImage = nullptr;
	class USpriteRenderer* SpriteRenderer;

	class U2DCollision* SlideLeftCollision = nullptr;
	class U2DCollision* SlideRightCollision = nullptr;
	class U2DCollision* InhaleLeftCollision = nullptr;
	class U2DCollision* InhaleRightCollision = nullptr;

	void SetPlayer();
	void SetAnimation();
	void SetAnimDir();
	void SetPlayerCollision();
	void CameraMove();

	void FSM(float _DeltaTime);

	// Attack
	bool IsFull = false;
};

