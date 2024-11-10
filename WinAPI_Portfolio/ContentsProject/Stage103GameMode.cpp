#include "PreCompile.h"
#include "Stage103GameMode.h"

#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineAPICore.h>

#include <EngineCore/Level.h>

#include "Player.h"
#include "Stage.h"
#include "HUI.h"

AStage103GameMode::AStage103GameMode()
{
}

AStage103GameMode::~AStage103GameMode()
{
}

void AStage103GameMode::BeginPlay()
{
	Super::BeginPlay();
	NewActor = GetWorld()->SpawnActor<AStage>();
	NewActor->SetSprite("Stage103.png");
	NewActor->SetColSprite("ColStage103.png");

	APlayer* Player = dynamic_cast<APlayer*>(GetWorld()->GetPawn());
	Player->GetBackImage("Stage103.png", "ColStage103.png");
	Player->SetActorLocation({ 150, 195 });

	NewUI = GetWorld()->SpawnActor<AHUI>();
	NewUI->SetSprite("StageUI.png", "Lives.png");
}

void AStage103GameMode::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	if (true == UEngineInput::GetInst().IsDown('R'))
	{
		UEngineAPICore::GetCore()->OpenLevel("Stage104");
	}

	if (true == UEngineInput::GetInst().IsDown('T'))
	{
		NewActor->SwitchColSprite();
	}
}


