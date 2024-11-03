#pragma once
#include <EngineCore/GameMode.h>

class AStage1_1_GameMode : public AGameMode
{
public:
	// constrcuter destructer
	AStage1_1_GameMode();
	~AStage1_1_GameMode();

	// delete Function
	AStage1_1_GameMode(const AStage1_1_GameMode& _Other) = delete;
	AStage1_1_GameMode(AStage1_1_GameMode&& _Other) noexcept = delete;
	AStage1_1_GameMode& operator=(const AStage1_1_GameMode& _Other) = delete;
	AStage1_1_GameMode& operator=(AStage1_1_GameMode&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;

	void Tick(float _DeltaTime) override;
private:

};

