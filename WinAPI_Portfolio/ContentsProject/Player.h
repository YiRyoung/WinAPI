#pragma once
#include <EngineCore/Actor.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/2DCollision.h>

#include "ContentsEnum.h"

class APlayer : public AActor
{
public:
	static int PlayerLife;
	static int PlayerHp;
	static int Score;
	static EAbilityType PlayerAbility;

	// constrcuter destructer
	APlayer();
	~APlayer();

	// delete Function
	APlayer(const APlayer& _Other) = delete;
	APlayer(APlayer&& _Other) noexcept = delete;
	APlayer& operator=(const APlayer& _Other) = delete;
	APlayer& operator=(APlayer&& _Other) noexcept = delete;

	// SpriteRenderer
	FVector2D GetDir() const;
	std::string GetAnimDir() const;
	USpriteRenderer* GetPlayerRenderer() const
	{
		return PlayerRenderer;
	}

	void ChangeAnimation(std::string _Anim);

	// PlayerState
	inline EPlayerState GetCurState() const
	{
		return CurState;
	}
	inline void SetCurState(EPlayerState _ChangeState)
	{
		CurState = _ChangeState;
	}

	// PlayerAbility
	inline EAbilityType GetCurAbility() const
	{
		return PlayerAbility;
	}
	inline void SetCurAbility(EAbilityType _NewAbility)
	{
		PlayerAbility = _NewAbility;
	}

	// CurMonsterAbility (IsFull == true)
	inline EAbilityType GetCurMonsterAbility() const
	{
		return CurMonsterAbility;
	}
	inline void SetCurMosnterAbility(EAbilityType _Ability)
	{
		CurMonsterAbility = _Ability;
	}

	// Pixel Collision
	void GetBackgroundImage(std::string_view _BackImage, std::string_view _ColImage);

	inline bool GetIsFull() const
	{
		return IsFull;
	}
	inline void SetIsFull(bool _Full)
	{
		IsFull = _Full;
	}

	void SetAdjustSize();
	void DrawDebugPoint(FVector2D _Point);
	bool PixelPointCheck(ECheckDir _Dir, UColor _Color);
	bool PixelLineCheck(ECheckDir _Dir, UColor _Color);

	// Collision Switch
	void SliderCollisionSwitch(bool _IsOn);
	void SkillBoxCollisionSwitch(bool _IsOn);

	// Spawn Bullet & Ability
	void SpawnWind();
	void SpawnSpit();
	void SpawnBeam();
	void SpawnSlide();
	void SpawnFire();
	class AFireBall* NewFireBall = nullptr;

	void CollisionEnter(AActor* _ColActor);
	void CollisionStay(AActor* _ColActor);

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	bool IsFull = false;
	int MySpriteIndex = 0;
	std::string AnimDir = "_Right";
	FVector2D PlayerScale = FVector2D::ZERO;
	FVector2D AdjustValue = FVector2D::ZERO;

	class PlayerState* State = nullptr;
	class PlayerState* Ability = nullptr;
	class UEngineWinImage* BackgroundImage = nullptr;
	class UEngineWinImage* ColliderImage = nullptr;
	USpriteRenderer* PlayerRenderer = nullptr;
	EPlayerState CurState = EPlayerState::IDLE;
	EAbilityType CurMonsterAbility = EAbilityType::MAX;	// IsFull
	void FSM(float _DeltaTime);

	U2DCollision* PlayerCollision = nullptr;
	U2DCollision* SlideLeftCollision = nullptr;
	U2DCollision* SlideRightCollision = nullptr;
	U2DCollision* InhaleBoxLeftCollision = nullptr;
	U2DCollision* InhaleBoxRightCollision = nullptr;


	void SetPlayer();
	void SetAnimation();
	void SetAnimDir();
	void SetPlayerCollision();
	void CameraMove();
	
};

