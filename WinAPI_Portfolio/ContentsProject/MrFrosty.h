#pragma once
#include <EngineCore/Actor.h>

enum class EFrostyPhase
{
	Phase1,
	Phase2,
	Phase3
};

class AMrFrosty : public AActor
{
public:
	// constrcuter destructer
	AMrFrosty();
	~AMrFrosty();

	// delete Function
	AMrFrosty(const AMrFrosty& _Other) = delete;
	AMrFrosty(AMrFrosty&& _Other) noexcept = delete;
	AMrFrosty& operator=(const AMrFrosty& _Other) = delete;
	AMrFrosty& operator=(AMrFrosty&& _Other) noexcept = delete;

	void SetMonster();
protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	void SetAnimation();

	class UEngineWinImage* ColImage = nullptr;
	class USpriteRenderer* SpriteRenderer = nullptr;
	class U2DCollision* CollisionComponent = nullptr;
};

