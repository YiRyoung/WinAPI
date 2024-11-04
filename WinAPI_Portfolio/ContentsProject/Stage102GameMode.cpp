#include "PreCompile.h"
#include "Stage102GameMode.h"

#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineAPICore.h>

#include <EngineCore/Level.h>
#include "Stage102.h"
#include "Stage103.h"

AStage102GameMode::AStage102GameMode()
{
}

AStage102GameMode::~AStage102GameMode()
{
}

void AStage102GameMode::BeginPlay()
{
	Super::BeginPlay();
	AStage102* NewActor = GetWorld()->SpawnActor<AStage102>();
}

void AStage102GameMode::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	if (true == UEngineInput::GetInst().IsDown('R'))
	{
		UEngineAPICore::GetCore()->OpenLevel("Stage103");
	}
}

