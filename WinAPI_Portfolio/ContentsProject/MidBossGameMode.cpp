#include "PreCompile.h"
#include "MidBossGameMode.h"

#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineAPICore.h>

#include <EngineCore/Level.h>

#include "Player.h"
#include "Monster.h"
#include "HotHead.h"

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

	NewPlayer = dynamic_cast<APlayer*>(GetWorld()->GetPawn());
	NewPlayer->GetBackgroundImage("MidBoss.png", "ColMidBoss.png");
	NewPlayer->SetActorLocation({ 185, 915 });

	NewUI = GetWorld()->SpawnActor<AHUI>();
	NewUI->SetSprite("StageUI.png", "Lives.png");

	NewHotHead = GetWorld()->SpawnActor<AHotHead>();
	NewHotHead->GetColImage("ColMidBoss.png");
	NewHotHead->SetActorLocation({ 670, 920 });
}

void AMidBossGameMode::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	NewHotHead->SetDistance(NewPlayer->GetActorLocation());

	if (true == UEngineInput::GetInst().IsDown('R'))
	{
		UEngineAPICore::GetCore()->OpenLevel("Boss");
	}

	if (true == UEngineInput::GetInst().IsDown('T'))
	{
		NewActor->SwitchColSprite();
	}
}

