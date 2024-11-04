#pragma once
#include <EngineCore/GameMode.h>

class AStage103GameMode : public AGameMode
{
public:
	// constrcuter destructer
	AStage103GameMode();
	~AStage103GameMode();

	// delete Function
	AStage103GameMode(const AStage103GameMode& _Other) = delete;
	AStage103GameMode(AStage103GameMode&& _Other) noexcept = delete;
	AStage103GameMode& operator=(const AStage103GameMode& _Other) = delete;
	AStage103GameMode& operator=(AStage103GameMode&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;

	void Tick(float _DeltaTime) override;

private:

};

