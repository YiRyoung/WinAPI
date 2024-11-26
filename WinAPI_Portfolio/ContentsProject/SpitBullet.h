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

	void SetDir(std::string _KirbyAnimDir);

	std::string AnimDir = "_Left";			// Sprite ����
	FVector2D Dir = FVector2D::ZERO;		// �̵� ����
	FVector2D SpawnPos = FVector2D::ZERO;	// ���� ��ġ

	void CollisionEnter(AActor* _ColActor);

protected:
	void BeginPlay(float _DeltaTime);
	void Tick(float _DeltaTime);

private:
	float Speed = 200.0f;
	float Time = 0.0f;

	class USpriteRenderer* SpitBulletRender = nullptr;
	class U2DCollision* SpitBulletCollision = nullptr;
};

