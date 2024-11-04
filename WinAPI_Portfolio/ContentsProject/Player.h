#pragma once
#include <EngineCore/Actor.h>

class APlayer : public AActor
{
public:
	// constrcuter destructer
	APlayer();
	~APlayer();

	// delete Function
	APlayer(const APlayer& _Other) = delete;
	APlayer(APlayer&& _Other) noexcept = delete;
	APlayer& operator=(const APlayer& _Other) = delete;
	APlayer& operator=(APlayer&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void LevelChangeStart();
	void LevelChangeEnd();

	FVector2D CameraPivot;
	class AStage101* CurStage = nullptr;

protected:

private:

	float Speed = 300.0f;
	int MySpriteIndex = 0;

	class USpriteRenderer* SpriteRenderer;
};

