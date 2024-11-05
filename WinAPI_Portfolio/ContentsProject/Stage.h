#pragma once
#include <EngineCore/Actor.h>

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

	void SetSprite(std::string _Sprite);
	void SetColSprite(std::string _Sprite);

protected:

private:
	FVector2D MapScale = FVector2D::ZERO;

	class USpriteRenderer* SpriteRenderer;
	class USpriteRenderer* ColSpriteRenderer;
};

