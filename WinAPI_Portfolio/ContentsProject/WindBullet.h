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
	class USpriteRenderer* WindBulletRender = nullptr;
	class UEngineWinImage* BackImage = nullptr;
	class U2DCollision* WindBulletCollision = nullptr;

	std::string AnimDir = "_Left";
	FVector2D Dir = FVector2D::ZERO;
	FVector2D SpawnPos = FVector2D::ZERO;

	float Speed = 80.0f;
	float Time = 0.0f;

};

