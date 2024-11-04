#pragma once
#include <EngineCore/Actor.h>

class AStage103 : public AActor
{
public:
	// constrcuter destructer
	AStage103();
	~AStage103();

	// delete Function
	AStage103(const AStage103& _Other) = delete;
	AStage103(AStage103&& _Other) noexcept = delete;
	AStage103& operator=(const AStage103& _Other) = delete;
	AStage103& operator=(AStage103&& _Other) noexcept = delete;

protected:

private:

};

