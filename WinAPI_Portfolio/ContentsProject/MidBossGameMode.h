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
	class AStage* NewActor = nullptr;
	class AHUI* NewUI = nullptr;
};

