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
#include "HPGauge.h"
#include "Ability.h"
#include "Score.h"
#include "Fade.h"

#include "ContentsEnum.h"

AMidBossGameMode::AMidBossGameMode()
{
}

AMidBossGameMode::~AMidBossGameMode()
{
}

void AMidBossGameMode::BeginPlay()
{
	Super::BeginPlay();

	BGMPlayer = UEngineSound::Play("Vegetable Valley 2.mp3");
	BGMPlayer.Loop(10);
	SoundManager.SetBGMSoundPlayer(BGMPlayer);

	NewFade = GetWorld()->SpawnActor<AFade>();
	NewFade->FadeOut();

	NewHP = GetWorld()->SpawnActor<AHPGauge>();
	NewAbility = GetWorld()->SpawnActor<AAbility>();

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
	NewHotHead->SetActorLocation({ 670, 917 });

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

void AMidBossGameMode::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	NewPlayerLife->SetValue(APlayer::PlayerLife);
	NewPlayerScore->SetValue(APlayer::Score);
	NewHP->SetHPGauge(APlayer::PlayerHp);
	NewAbility->SetSprite("Ability.png", static_cast<int>(APlayer::PlayerAbility));
	
	NewHotHead->SetDistance(NewPlayer->GetActorLocation());

	if (true == UEngineInput::GetInst().IsDown('R'))
	{
		BGMPlayer.Off();
		UEngineAPICore::GetCore()->OpenLevel("Boss");
	}

	if (true == UEngineInput::GetInst().IsDown('T'))
	{
		NewActor->SwitchColSprite();
	}
}

