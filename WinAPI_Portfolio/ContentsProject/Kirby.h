#pragma once
#include <EngineCore/Actor.h>

class AKirby : public AActor
{
public:
	// constrcuter destructer
	AKirby();
	~AKirby();

	// delete Function
	AKirby(const AKirby& _Other) = delete;
	AKirby(AKirby&& _Other) noexcept = delete;
	AKirby& operator=(const AKirby& _Other) = delete;
	AKirby& operator=(AKirby&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void MoveFunction(FVector2D _Dir);

protected:

private:
	float Speed = 100.f;
	int MySpriteIndex = 0;

	class USpriteRenderer* SpriteRenderer;
};

