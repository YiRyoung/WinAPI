#pragma once
#include <EngineCore/Actor.h>

#include <EnginePlatform/EngineSound.h>

#include "SoundManager.h"

class AFireBall : public AActor
{
public:
	// constrcuter destructer
	AFireBall();
	~AFireBall();

	// delete Function
	AFireBall(const AFireBall& _Other) = delete;
	AFireBall(AFireBall&& _Other) noexcept = delete;
	AFireBall& operator=(const AFireBall& _Other) = delete;
	AFireBall& operator=(AFireBall&& _Other) noexcept = delete;

	void SetDir(const FVector2D& _Dir);

	void SetMosnterSkillCollision();
	void SetPlayerSkillCollision();

	void ChangeAnimation(std::string _AnimName);
	void StopSound();

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	float Speed = 180.0f;
	float CurTime = 0.0f;
	std::string AnimDir = "";

	FVector2D Dir = FVector2D::ZERO;
	class USpriteRenderer* FireballRender = nullptr;
	class U2DCollision* FireballCollision = nullptr;

	USoundPlayer BGMPlayer;
};

