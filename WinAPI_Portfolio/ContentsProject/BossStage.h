#pragma once
#include <EngineCore/Actor.h>

class ABossStage : public AActor
{
public:
	// constrcuter destructer
	ABossStage();
	~ABossStage();

	// delete Function
	ABossStage(const ABossStage& _Other) = delete;
	ABossStage(ABossStage&& _Other) noexcept = delete;
	ABossStage& operator=(const ABossStage& _Other) = delete;
	ABossStage& operator=(ABossStage&& _Other) noexcept = delete;

protected:

private:

};

