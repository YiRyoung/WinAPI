#include "PreCompile.h"
#include "Stage1_1_GameMode.h"

#include "EngineCore/Level.h"

#include "Stage1_1_Map.h"
#include "Kirby.h"

AStage1_1_GameMode::AStage1_1_GameMode()
{
}

AStage1_1_GameMode::~AStage1_1_GameMode()
{
}

void AStage1_1_GameMode::BeginPlay()
{
	AStage1_1_Map* Background = GetWorld()->SpawnActor<AStage1_1_Map>();
	
	AKirby* Kirby = GetWorld()->SpawnActor<AKirby>();
}

void AStage1_1_GameMode::Tick(float _DeltaTime)
{
}

