#include "PreCompile.h"
#include "Stage101GameMode.h"

#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineAPICore.h>

#include <EngineCore/Level.h>
#include "Player.h"
#include "Stage.h"
#include "HUI.h"

#include "ContentsEnum.h"

AStage101GameMode::AStage101GameMode()
{
}

AStage101GameMode::~AStage101GameMode()
{
}

void AStage101GameMode::BeginPlay()
{
	Super::BeginPlay();

	NewActor = GetWorld()->SpawnActor<AStage>();
	NewActor->SetSprite("Stage101.png");
	NewActor->SetColSprite("ColStage101.png");
	NewActor->SwitchColSprite();

	NewPlayer = dynamic_cast<APlayer*>(GetWorld()->GetPawn());
	NewPlayer->GetBackgroundImage("Stage101.png", "ColStage101.png");
	NewPlayer->SetActorLocation({ 80, 398 });

	NewUI = GetWorld()->SpawnActor<AHUI>();
	NewUI->SetSprite("StageUI.png", "Lives.png");
}

void AStage101GameMode::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	if (true == UEngineInput::GetInst().IsDown('R'))
	{
		UEngineAPICore::GetCore()->OpenLevel("MidBoss");
	}

	if (true == UEngineInput::GetInst().IsDown('T'))
	{
		NewActor->SwitchColSprite();
	}
}
 