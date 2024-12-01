#pragma once
#include <EngineCore/GameMode.h>
#include <EnginePlatform/EngineSound.h>

#include "SoundManager.h"

class AEndGameMode : public AGameMode
{
public:
	// constrcuter destructer
	AEndGameMode();
	~AEndGameMode();

	// delete Function
	AEndGameMode(const AEndGameMode& _Other) = delete;
	AEndGameMode(AEndGameMode&& _Other) noexcept = delete;
	AEndGameMode& operator=(const AEndGameMode& _Other) = delete;
	AEndGameMode& operator=(AEndGameMode&& _Other) noexcept = delete;


protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void LevelChangeStart() override;
	void LevelChangeEnd() override;

private:
	float CurTime = 0.0f;
	class EndMap* NewActor = nullptr;
	class AFade* NewFade = nullptr;

	SoundManager& SoundManager = SoundManager::GetInst();
	USoundPlayer& BGMPlayer = SoundManager.GetBGMSoundPlayer();
};

