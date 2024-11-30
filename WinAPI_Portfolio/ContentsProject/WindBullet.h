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

	void SetDir(std::string _KirbyAnimDir);

	std::string AnimDir = "_Left";			// Sprite ����
	FVector2D Dir = FVector2D::ZERO;		// �̵� ����
	FVector2D SpawnPos = FVector2D::ZERO;	// ���� ��ġ

	void CollisionEnter(AActor* _ColActor);
protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	float Speed = 195.0f;
	float Time = 0.0f;

	class USpriteRenderer* WindBulletRender = nullptr;
	class U2DCollision* WindBulletCollision = nullptr;
};

