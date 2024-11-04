#pragma once
#include <EngineCore/Actor.h>

// Ό³Έν :
class AStage : public AActor
{
public:
	// constrcuter destructer
	AStage();
	~AStage();

	// delete Function
	AStage(const AStage& _Other) = delete;
	AStage(AStage&& _Other) noexcept = delete;
	AStage& operator=(const AStage& _Other) = delete;
	AStage& operator=(AStage&& _Other) noexcept = delete;

	FVector2D MapScale;

	void SetSprite(std::string _Sprite);

protected:

private:
	class USpriteRenderer* SpriteRenderer;

};

