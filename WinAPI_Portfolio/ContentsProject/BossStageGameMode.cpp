#include "PreCompile.h"
#include "BossStageGameMode.h"

#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineAPICore.h>

#include <EngineCore/Level.h>

#include "Player.h"
#include "Stage.h"
#include "HUI.h"

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
	NewActor->SetColSprite("ColWhispy Woods.png");

	APlayer* Player = dynamic_cast<APlayer*>(GetWorld()->GetPawn());
	Player->GetBackImage("Whispy Woods.png");
	Player->GetColImage("ColWhispy Woods.png");
	Player->SetActorLocation({ 70, 174 });

	NewUI = GetWorld()->SpawnActor<AHUI>();
	NewUI->SetSprite("StageUI.png");
}

void ABossStageGameMode::Tick(float _DeltaTime)
{
	if (true == UEngineInput::GetInst().IsDown('T'))
	{
		NewActor->SwitchColSprite();
	}
}

