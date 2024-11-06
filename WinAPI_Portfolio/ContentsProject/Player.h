#pragma once
#include <EngineCore/Actor.h>

enum class PlayerState
{
	Idle,
	Move
};

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

	void GetBackImage(std::string_view _ImageName);
	void GetColImage(std::string_view _ImageName);

	void CameraMove();

	PlayerState CurPlayerState = PlayerState::Idle;

protected:

private:
	float Speed = 500.0f;
	int MySpriteIndex = 0;

	class UEngineWinImage* BackImage = nullptr;
	class UEngineWinImage* ColImage = nullptr;
	class USpriteRenderer* SpriteRenderer;

	void ChangeState(PlayerState _CurPlayerState);

	void Idle(float _DeltaTime);
	void Move(float _DeltaTime);

};

