#pragma once
#include <EngineCore/Actor.h>

#include "ContentsEnum.h"

enum class EAppleState
{
	IDLE,
	CHASE,
	INHALE
};

class AApple : public AActor
{
public:
	// constrcuter destructer
	AApple();
	~AApple();

	// delete Function
	AApple(const AApple& _Other) = delete;
	AApple(AApple&& _Other) noexcept = delete;
	AApple& operator=(const AApple& _Other) = delete;
	AApple& operator=(AApple&& _Other) noexcept = delete;

	void SetColImage(UEngineWinImage* _ColImage);
	void SetPlayerLocation(FVector2D _PlayerPos);

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
private:
	float CurTime = 0.0f;
	bool PixelPointCheck(ECheckDir _Dir, UColor _Color);
	void Gravity(float _DeltaTime);

	EAppleState CurState = EAppleState::IDLE;
	void AppleState(float _DeltaTime);
	void Idle(float _DeltaTime);
	void Chase(float _DeltaTime);
	void Inhale(float _DeltaTime);

	FVector2D GravityForce = FVector2D::ZERO;
	FVector2D PlayerPos = FVector2D::ZERO;
	FVector2D AppleDir = FVector2D::ZERO;

	class UEngineWinImage* ColImage = nullptr;
	class USpriteRenderer* SpriteRenderer = nullptr;
	class U2DCollision* CollisionComponent = nullptr;
};

