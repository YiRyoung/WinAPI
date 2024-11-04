#pragma once
#include <EngineCore/Actor.h>

class AStage101 : public AActor
{
public:
	// constrcuter destructer
	AStage101();
	~AStage101();

	// delete Function
	AStage101(const AStage101& _Other) = delete;
	AStage101(AStage101&& _Other) noexcept = delete;
	AStage101& operator=(const AStage101& _Other) = delete;
	AStage101& operator=(AStage101&& _Other) noexcept = delete;

protected:

private:

};

