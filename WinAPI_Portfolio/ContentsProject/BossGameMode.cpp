#include "PreCompile.h"
#include "BossGameMode.h"

#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineAPICore.h>

#include <EngineCore/Level.h>

#include "Player.h"
#include "Stage.h"
#include "HUI.h"
#include "Score.h"

#include "ContentsEnum.h"

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

	NewPlayerLife = GetWorld()->SpawnActor<AScore>();
	NewPlayerLife->SetActorLocation({ 686, 608 });
	NewPlayerLife->SetHPSpriteName("Score.png");
	NewPlayerLife->SetOrder(ERenderOrder::HUITEXT);
	NewPlayerLife->SetTextScale({ 26, 27 });

	NewPlayerScore = GetWorld()->SpawnActor<AScore>();
	NewPlayerScore->SetActorLocation({ 388, 633 });
	NewPlayerScore->SetHPSpriteName("Score.png");
	NewPlayerScore->SetOrder(ERenderOrder::HUITEXT);
	NewPlayerScore->SetTextScale({ 26, 27 });
}

void ABossGameMode::Tick(float _DeltaTime)
{

	NewPlayerLife->SetValue(APlayer::PlayerLife);
	NewPlayerScore->SetValue(54890);

	if (true == UEngineInput::GetInst().IsDown('T'))
	{
		NewActor->SwitchColSprite();
	}
}

