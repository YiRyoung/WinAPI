#include "PreCompile.h"
#include "BossStageGameMode.h"

#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineAPICore.h>

#include <EngineCore/Level.h>

#include "Player.h"
#include "Stage.h"

ABossStageGameMode::ABossStageGameMode()
{
}

ABossStageGameMode::~ABossStageGameMode()
{
}

void ABossStageGameMode::BeginPlay()
{
	Super::BeginPlay();

	NewActor = GetWorld()->SpawnActor<AStage>();
	NewActor->SetSprite("Whispy Woods.png");
	FVector2D Pos = NewActor->GetActorLocation();

	APlayer* Player = dynamic_cast<APlayer*>(GetWorld()->GetPawn());
	Player->GetBackImage("Whispy Woods.png");
}

void ABossStageGameMode::Tick(float _DeltaTime)
{
}

