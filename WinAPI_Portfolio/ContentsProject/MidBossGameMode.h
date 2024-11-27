#pragma once
#include <EngineCore/GameMode.h>

class AMidBossGameMode : public AGameMode
{
public:
	// constrcuter destructer
	AMidBossGameMode();
	~AMidBossGameMode();

	// delete Function
	AMidBossGameMode(const AMidBossGameMode& _Other) = delete;
	AMidBossGameMode(AMidBossGameMode&& _Other) noexcept = delete;
	AMidBossGameMode& operator=(const AMidBossGameMode& _Other) = delete;
	AMidBossGameMode& operator=(AMidBossGameMode&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;

	void Tick(float _DeltaTime) override;

private:
	class APlayer* NewPlayer = nullptr;
	class AStage* NewActor = nullptr;
	class AHUI* NewUI = nullptr;
	class AHPGauge* NewHP = nullptr;
	class AAbility* NewAbility = nullptr;
	class AScore* NewPlayerLife = nullptr;
	class AScore* NewPlayerScore = nullptr;
	class AFade* NewFade = nullptr;
	class AMonster* NewHotHead = nullptr;
};

