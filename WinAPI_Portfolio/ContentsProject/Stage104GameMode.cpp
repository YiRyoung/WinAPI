#include "PreCompile.h"
#include "Stage104GameMode.h"

#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineAPICore.h>

#include <EngineCore/Level.h>
#include "Stage104.h"
#include "BossStage.h"

AStage104GameMode::AStage104GameMode()
{
}

AStage104GameMode::~AStage104GameMode()
{
}

void AStage104GameMode::BeginPlay()
{
	Super::BeginPlay();
	AStage104* NewActor = GetWorld()->SpawnActor<AStage104>();
}

void AStage104GameMode::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	if (true == UEngineInput::GetInst().IsDown('R'))
	{
		UEngineAPICore::GetCore()->OpenLevel("BossStage");
	}
}

