#include "PreCompile.h"
#include "MidBossGameMode.h"

#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineAPICore.h>

#include <EngineCore/Level.h>

#include "Player.h"
#include "Stage.h"
#include "HUI.h"
#include "Fade.h"

AMidBossGameMode::AMidBossGameMode()
{
}

AMidBossGameMode::~AMidBossGameMode()
{
}

void AMidBossGameMode::BeginPlay()
{
	Super::BeginPlay();
	NewFade = GetWorld()->SpawnActor<AFade>();
	NewFade->FadeOut();

	NewActor = GetWorld()->SpawnActor<AStage>();
	NewActor->SetSprite("MidBoss.png");
	NewActor->SetColSprite("ColMidBoss.png");
	NewActor->SwitchColSprite();

	APlayer* Player = dynamic_cast<APlayer*>(GetWorld()->GetPawn());
	Player->GetBackgroundImage("MidBoss.png", "ColMidBoss.png");
	Player->SetActorLocation({ 185, 915 });

	NewUI = GetWorld()->SpawnActor<AHUI>();
	NewUI->SetSprite("StageUI.png", "Lives.png");

}

void AMidBossGameMode::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	if (true == UEngineInput::GetInst().IsDown('R'))
	{
		UEngineAPICore::GetCore()->OpenLevel("Boss");
	}

	if (true == UEngineInput::GetInst().IsDown('T'))
	{
		NewActor->SwitchColSprite();
	}
}

