#pragma once
#include <EngineCore/Actor.h>

class ACutterSlider : public AActor
{
public:
	// constrcuter destructer
	ACutterSlider();
	~ACutterSlider();

	// delete Function
	ACutterSlider(const ACutterSlider& _Other) = delete;
	ACutterSlider(ACutterSlider&& _Other) noexcept = delete;
	ACutterSlider& operator=(const ACutterSlider& _Other) = delete;
	ACutterSlider& operator=(ACutterSlider&& _Other) noexcept = delete;

	void SetDir(const FVector2D& _Dir);

	void SetMosnterSkillCollision();
	void SetPlayerSkillCollision();


protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	float PushForce = 320.0f;
	float Speed = 180.0f;
	float CurTime = 0.0f;

	FVector2D Dir = FVector2D::ZERO;
	FVector2D ReversePush = FVector2D::ZERO;

	class USpriteRenderer* CutterSliderRender = nullptr;
	class U2DCollision* CutterSliderCollision = nullptr;
};

