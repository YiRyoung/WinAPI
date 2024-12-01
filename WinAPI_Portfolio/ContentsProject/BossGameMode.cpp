#include "PreCompile.h"
#include "BossGameMode.h"

#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineAPICore.h>

#include <EngineCore/Level.h>

#include "Player.h"
#include "Stage.h"
#include "HUI.h"
#include "HPGauge.h"
#include "Ability.h"
#include "Score.h"
#include "Fade.h"

#include "WisphyWood.h"

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

	NewFade = GetWorld()->SpawnActor<AFade>();

	NewHP = GetWorld()->SpawnActor<AHPGauge>();
	NewAbility = GetWorld()->SpawnActor<AAbility>();

	NewActor = GetWorld()->SpawnActor<AStage>();
	NewActor->SetSprite("Boss.png");
	NewActor->SetColSprite("ColBoss.png");
	NewActor->SwitchColSprite();

	Player = dynamic_cast<APlayer*>(GetWorld()->GetPawn());
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

	NewBoss = GetWorld()->SpawnActor<AWisphyWood>();
	NewBoss->SetActorLocation({ 588, 910});
	NewBoss->SetColImage("ColBoss.png");
}

void ABossGameMode::Tick(float _DeltaTime)
{
	NewPlayerLife->SetValue(APlayer::PlayerLife);
	NewPlayerScore->SetValue(APlayer::Score);
	NewHP->SetHPGauge(APlayer::PlayerHp);
	NewAbility->SetSprite("Ability.png", static_cast<int>(APlayer::PlayerAbility));
	NewBoss->SetPlayerLocation(Player->GetActorLocation());

	if (true == UEngineInput::GetInst().IsDown('T'))
	{
		NewActor->SwitchColSprite();
	}
}

void ABossGameMode::LevelChangeStart()
{
	Super::LevelChangeStart();
	
	NewFade->FadeOut();
	
	if (true == BGMPlayer.IsPlaying())
	{
		BGMPlayer.Stop();
	}
	BGMPlayer = UEngineSound::Play("Boss Battle.mp3");
	SoundManager.SetBGMSoundPlayer(BGMPlayer);
}

void ABossGameMode::LevelChangeEnd()
{
	Super::LevelChangeEnd();
	
	NewFade->FadeIn();
}

