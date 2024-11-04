#include "PreCompile.h"
#include "BossStageGameMode.h"

#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineAPICore.h>

#include <EngineCore/Level.h>

ABossStageGameMode::ABossStageGameMode()
{
}

ABossStageGameMode::~ABossStageGameMode()
{
}

void ABossStageGameMode::BeginPlay()
{
	Super::BeginPlay();
	//ABossStage* NewActor = GetWorld()->SpawnActor<ABossStage>();
}

void ABossStageGameMode::Tick(float _DeltaTime)
{
}

