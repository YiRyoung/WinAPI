#pragma once
#include <EngineCore/Actor.h>

class ALiveIcon : public AActor
{
public:
	// constrcuter destructer
	ALiveIcon();
	~ALiveIcon();

	// delete Function
	ALiveIcon(const ALiveIcon& _Other) = delete;
	ALiveIcon(ALiveIcon&& _Other) noexcept = delete;
	ALiveIcon& operator=(const ALiveIcon& _Other) = delete;
	ALiveIcon& operator=(ALiveIcon&& _Other) noexcept = delete;

	void SetSprite(std::string _Sprite);

protected:

private:
	FVector2D MapScale = FVector2D::ZERO;
	class USpriteRenderer* SpriteRenderer;
};

