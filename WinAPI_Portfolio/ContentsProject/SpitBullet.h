#pragma once
#include <EngineCore/Actor.h>

class ASpitBullet : public AActor
{
public:
	// constrcuter destructer
	ASpitBullet();
	~ASpitBullet();

	// delete Function
	ASpitBullet(const ASpitBullet& _Other) = delete;
	ASpitBullet(ASpitBullet&& _Other) noexcept = delete;
	ASpitBullet& operator=(const ASpitBullet& _Other) = delete;
	ASpitBullet& operator=(ASpitBullet&& _Other) noexcept = delete;

	FVector2D GetSpawnPos() const
	{
		return SpawnPos;
	}

	void SetDir(std::string _KirbyDir)
	{
		AnimDir = _KirbyDir;

		if ("_Left" == AnimDir)
		{
			Dir = FVector2D::LEFT;
			SpawnPos = FVector2D({ -50, 0 });
		}
		else
		{
			Dir = FVector2D::RIGHT;
			SpawnPos = FVector2D({ 50, 0 });
		}
	}

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	class USpriteRenderer* SpitBulletRender = nullptr;
	class U2DCollision* SpitBulletCollision = nullptr;

	std::string AnimDir = "_Left";
	FVector2D Dir = FVector2D::ZERO;
	FVector2D SpawnPos = FVector2D::ZERO;

	float Speed = 200.0f;
	float Time = 0.0f;
};

