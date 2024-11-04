#pragma once
#include <EngineCore/GameMode.h>

class AStage102GameMode : public AGameMode
{
public:
	// constrcuter destructer
	AStage102GameMode();
	~AStage102GameMode();

	// delete Function
	AStage102GameMode(const AStage102GameMode& _Other) = delete;
	AStage102GameMode(AStage102GameMode&& _Other) noexcept = delete;
	AStage102GameMode& operator=(const AStage102GameMode& _Other) = delete;
	AStage102GameMode& operator=(AStage102GameMode&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;

	void Tick(float _DeltaTime) override;

private:

};

