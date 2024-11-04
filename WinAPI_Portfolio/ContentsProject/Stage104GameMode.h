#pragma once
#include <EngineCore/GameMode.h>

class AStage104GameMode : public AGameMode
{
public:
	// constrcuter destructer
	AStage104GameMode();
	~AStage104GameMode();

	// delete Function
	AStage104GameMode(const AStage104GameMode& _Other) = delete;
	AStage104GameMode(AStage104GameMode&& _Other) noexcept = delete;
	AStage104GameMode& operator=(const AStage104GameMode& _Other) = delete;
	AStage104GameMode& operator=(AStage104GameMode&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;

	void Tick(float _DeltaTime) override;

private:

};

