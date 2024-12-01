#pragma once
#include <EngineCore/Actor.h>

class EndMap : public AActor
{
public:
	// constrcuter destructer
	EndMap();
	~EndMap();

	// delete Function
	EndMap(const EndMap& _Other) = delete;
	EndMap(EndMap&& _Other) noexcept = delete;
	EndMap& operator=(const EndMap& _Other) = delete;
	EndMap& operator=(EndMap&& _Other) noexcept = delete;

protected:

private:
	class USpriteRenderer* KirbyRenderer = nullptr;
	class USpriteRenderer* EndRenderer = nullptr;
	FVector2D KirbyScale = FVector2D::ZERO;
};

