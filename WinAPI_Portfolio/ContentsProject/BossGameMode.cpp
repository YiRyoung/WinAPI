#include "PreCompile.h"
#include "BossGameMode.h"

#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineAPICore.h>

#include <EngineCore/Level.h>

#include "Player.h"
#include "Stage.h"
#include "HUI.h"

ABossGameMode::ABossGameMode()
{
}

ABossGameMode::~ABossGameMode()
{
}

void ABossGameMode::BeginPlay()
{
	Super::BeginPlay();
	NewActor = GetWorld()->SpawnActor<AStage>();
	NewActor->SetSprite("Boss.png");
	NewActor->SetColSprite("ColBoss.png");
	NewActor->SwitchColSprite();

	APlayer* Player = dynamic_cast<APlayer*>(GetWorld()->GetPawn());
	Player->GetBackgroundImage("Boss.png", "ColBoss.png");
	Player->SetActorLocation({ 90, 116 });

	NewUI = GetWorld()->SpawnActor<AHUI>();
	NewUI->SetSprite("StageUI.png", "Lives.png");
}

void ABossGameMode::Tick(float _DeltaTime)
{
	if (true == UEngineInput::GetInst().IsDown('T'))
	{
		NewActor->SwitchColSprite();
	}
}

