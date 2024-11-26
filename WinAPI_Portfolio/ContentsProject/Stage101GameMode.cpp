#include "PreCompile.h"
#include "Stage101GameMode.h"

#include <EnginePlatform/EngineInput.h>

#include <EngineCore/EngineAPICore.h>
#include <EngineCore/Level.h>

#include "Player.h"
#include "Stage.h"
#include "HUI.h"
#include "Fade.h"

#include "Monster.h"
#include "WaddleDee.h"
#include "WaddleDoo.h"
#include "Kibble.h"

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

	NewFade = GetWorld()->SpawnActor<AFade>();
	NewFade->FadeOut();

	NewActor = GetWorld()->SpawnActor<AStage>();
	NewActor->SetSprite("Stage101.png");
	NewActor->SetColSprite("ColStage101.png");
	NewActor->SwitchColSprite();

	NewPlayer = dynamic_cast<APlayer*>(GetWorld()->GetPawn());
	NewPlayer->GetBackgroundImage("Stage101.png", "ColStage101.png");
	NewPlayer->SetActorLocation({ 80, 398 });

	NewUI = GetWorld()->SpawnActor<AHUI>();
	NewUI->SetSprite("StageUI.png", "Lives.png");

	NewWaddleDee = GetWorld()->SpawnActor<AWaddleDee>();
	NewWaddleDee->GetColImage("ColStage101.png");
	NewWaddleDee->SetActorLocation({ 1191, 280 });

	NewWaddleDoo = GetWorld()->SpawnActor<AWaddleDoo>();
	NewWaddleDoo->GetColImage("ColStage101.png");
	NewWaddleDoo->SetActorLocation({ 1850, 298 });

	NewKibble = GetWorld()->SpawnActor<AKibble>();
	NewKibble->GetColImage("ColStage101.png");
	NewKibble->SetActorLocation({ 2400, 230 });
}

void AStage101GameMode::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	NewWaddleDee->SetDistance(NewPlayer->GetActorLocation());
	NewWaddleDoo->SetDistance(NewPlayer->GetActorLocation());
	NewKibble->SetDistance(NewPlayer->GetActorLocation());

	if (true == UEngineInput::GetInst().IsDown(VK_UP) 
		&& (NewPlayer->PixelLineCheck(ECheckDir::UP, UColor::RED) || NewPlayer->PixelLineCheck(ECheckDir::DOWN, UColor::RED)))
	{
		NewFade->FadeIn();
		UEngineAPICore::GetCore()->OpenLevel("MidBoss");
	}

	if (true == UEngineInput::GetInst().IsDown('T'))
	{
		NewActor->SwitchColSprite();
	}
}
 