#include "PreCompile.h"
#include "Stage103GameMode.h"

#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineAPICore.h>

#include <EngineCore/Level.h>
#include "Stage103.h"
#include "Stage104.h"

AStage103GameMode::AStage103GameMode()
{
}

AStage103GameMode::~AStage103GameMode()
{
}

void AStage103GameMode::BeginPlay()
{
	Super::BeginPlay();
	AStage103* NewActor = GetWorld()->SpawnActor<AStage103>();
}

void AStage103GameMode::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	if (true == UEngineInput::GetInst().IsDown('R'))
	{
		UEngineAPICore::GetCore()->OpenLevel("Stage104");
	}
}


