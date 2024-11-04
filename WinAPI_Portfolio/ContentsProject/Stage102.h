#pragma once
#include <EngineCore/Actor.h>

class AStage102 : public AActor
{
public:
	// constrcuter destructer
	AStage102();
	~AStage102();

	// delete Function
	AStage102(const AStage102& _Other) = delete;
	AStage102(AStage102&& _Other) noexcept = delete;
	AStage102& operator=(const AStage102& _Other) = delete;
	AStage102& operator=(AStage102&& _Other) noexcept = delete;

protected:

private:

};

