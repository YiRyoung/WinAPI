#pragma once
#include "SceneComponent.h"

class USpriteRenderer : public USceneComponent
{
public:
	// constrcuter destructer
	USpriteRenderer();
	~USpriteRenderer();

	// delete Function
	USpriteRenderer(const USpriteRenderer& _Other) = delete;
	USpriteRenderer(USpriteRenderer&& _Other) noexcept = delete;
	USpriteRenderer& operator=(const USpriteRenderer& _Other) = delete;
	USpriteRenderer& operator=(USpriteRenderer&& _Other) noexcept = delete;

	void Render();
	void BeginPlay() override;
	void ComponentTick(float _DeltaTime) override;

protected:

public:
	class UEngineSprite* Sprite = nullptr;
	int CurIndex = 0;
	void SetSprite(std::string_view _Name, int _CurIndex = 0);
};

