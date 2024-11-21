#pragma once
#include "Player.h"

class WindBullet : public APlayer
{
public:
	// constrcuter destructer
	WindBullet();
	~WindBullet();

	// delete Function
	WindBullet(const WindBullet& _Other) = delete;
	WindBullet(WindBullet&& _Other) noexcept = delete;
	WindBullet& operator=(const WindBullet& _Other) = delete;
	WindBullet& operator=(WindBullet&& _Other) noexcept = delete;

	void SetKirbyDir()
	{
		if ("_Right" == APlayer::GetAnimDir())
		{
			KirbyDir = FVector2D::RIGHT;
		}
		else
		{
			KirbyDir = FVector2D::LEFT;
		}
	}

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
private:
	class USpriteRenderer* SpriteRenderer;
	class UEngineWinImage* BackImage = nullptr;
	FVector2D KirbyDir = FVector2D::ZERO;

	float Speed = 80.0f;
	float Time = 0.0f;

};

