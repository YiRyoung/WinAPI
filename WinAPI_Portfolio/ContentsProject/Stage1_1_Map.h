#pragma once
#include <EngineCore/Actor.h>

class AStage1_1_Map : public AActor
{
public:
	// constrcuter destructer
	AStage1_1_Map();
	~AStage1_1_Map();

	// delete Function
	AStage1_1_Map(const AStage1_1_Map& _Other) = delete;
	AStage1_1_Map(AStage1_1_Map&& _Other) noexcept = delete;
	AStage1_1_Map& operator=(const AStage1_1_Map& _Other) = delete;
	AStage1_1_Map& operator=(AStage1_1_Map&& _Other) noexcept = delete;

protected:

private:

};

