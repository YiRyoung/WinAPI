#pragma once
#include <EngineCore/GameMode.h>
#include <EnginePlatform/EngineSound.h>
#include "SoundManager.h"

class AStage101GameMode : public AGameMode
{
public:
	// constrcuter destructer
	AStage101GameMode();
	~AStage101GameMode();

	// delete Function
	AStage101GameMode(const AStage101GameMode& _Other) = delete;
	AStage101GameMode(AStage101GameMode&& _Other) noexcept = delete;
	AStage101GameMode& operator=(const AStage101GameMode& _Other) = delete;
	AStage101GameMode& operator=(AStage101GameMode&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void LevelChangeStart() override;
	void LevelChangeEnd() override;

private:
	class APlayer* NewPlayer = nullptr;
	class AStage* NewActor = nullptr;
	class AHUI* NewUI = nullptr;
	class AHPGauge* NewHP = nullptr;
	class AAbility* NewAbility = nullptr;
	class AScore* NewPlayerLife = nullptr;
	class AScore* NewPlayerScore = nullptr;
	class AFade* NewFade = nullptr;

	class AMonster* NewWaddleDee = nullptr;
	class AMonster* NewWaddleDoo = nullptr;
	class AMonster* NewKibble = nullptr;
	class AMonster* NewShotZo = nullptr;

	SoundManager& SoundManager = SoundManager::GetInst();
	USoundPlayer& BGMPlayer = SoundManager.GetBGMSoundPlayer();
};

