#pragma once
#include <EngineCore/GameMode.h>

class ABossStageGameMode : public AGameMode
{
public:
	// constrcuter destructer
	ABossStageGameMode();
	~ABossStageGameMode();

	// delete Function
	ABossStageGameMode(const ABossStageGameMode& _Other) = delete;
	ABossStageGameMode(ABossStageGameMode&& _Other) noexcept = delete;
	ABossStageGameMode& operator=(const ABossStageGameMode& _Other) = delete;
	ABossStageGameMode& operator=(ABossStageGameMode&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;

	void Tick(float _DeltaTime) override;

private:
	class AStage* NewActor = nullptr;
	class AHUI* NewUI = nullptr;
};

