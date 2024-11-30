#pragma once
#include <EngineCore/Actor.h>

#include "Shotzo.h"

class AShotBullet : public AActor
{
public:
	// constrcuter destructer
	AShotBullet();
	~AShotBullet();

	// delete Function
	AShotBullet(const AShotBullet& _Other) = delete;
	AShotBullet(AShotBullet&& _Other) noexcept = delete;
	AShotBullet& operator=(const AShotBullet& _Other) = delete;
	AShotBullet& operator=(AShotBullet&& _Other) noexcept = delete;

	FVector2D GetBulletDirection() const
	{
		return Direction;
	}
	void SetDirection(EShotDirect _CurDirect);

	void CollisionEnter(AActor* _ColActor);

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	float Speed = 200.0f;
	float Time = 0.0f;
	FVector2D Direction = FVector2D::ZERO;

	class USpriteRenderer* ShotBulletRender = nullptr;
	class U2DCollision* ShotBulletCollision = nullptr;
};

