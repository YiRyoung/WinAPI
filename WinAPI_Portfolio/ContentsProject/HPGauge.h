#pragma once
#include <EngineCore/Actor.h>

class AHPGauge : public AActor
{
public:
	// constrcuter destructer
	AHPGauge();
	~AHPGauge();

	// delete Function
	AHPGauge(const AHPGauge& _Other) = delete;
	AHPGauge(AHPGauge&& _Other) noexcept = delete;
	AHPGauge& operator=(const AHPGauge& _Other) = delete;
	AHPGauge& operator=(AHPGauge&& _Other) noexcept = delete;

	void SetHPGauge(int _PlayerHP);

protected:

private:
	FVector2D SpriteScale = FVector2D::ZERO;
	class USpriteRenderer* HPRender[6] = { nullptr, };
};

