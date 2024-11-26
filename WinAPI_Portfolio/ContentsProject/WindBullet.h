#pragma once
#include <EngineCore/Actor.h>

class AWindBullet : public AActor
{
public:
	// constrcuter destructer
	AWindBullet();
	~AWindBullet();

	// delete Function
	AWindBullet(const AWindBullet& _Other) = delete;
	AWindBullet(AWindBullet&& _Other) noexcept = delete;
	AWindBullet& operator=(const AWindBullet& _Other) = delete;
	AWindBullet& operator=(AWindBullet&& _Other) noexcept = delete;

	void SetDir(std::string _KirbyAnimDir)
	{
		AnimDir = _KirbyAnimDir;

		if ("_Left" == AnimDir)
		{
			Dir = FVector2D::LEFT;
			SpawnPos = FVector2D({ -55, -45 });
		}
		else
		{
			Dir = FVector2D::RIGHT;
			SpawnPos = FVector2D({ 55, -45 });
		}
	}

	std::string AnimDir = "_Left";			// Sprite 방향
	FVector2D Dir = FVector2D::ZERO;		// 이동 방향
	FVector2D SpawnPos = FVector2D::ZERO;	// 스폰 위치

	void CollisionEnter(AActor* _ColActor);
protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	float Speed = 140.0f;
	float Time = 0.0f;

	class USpriteRenderer* WindBulletRender = nullptr;
	class U2DCollision* WindBulletCollision = nullptr;

};

