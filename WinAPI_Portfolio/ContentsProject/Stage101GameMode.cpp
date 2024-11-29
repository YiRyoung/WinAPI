#include "PreCompile.h"
#include "Stage101GameMode.h"

#include <EnginePlatform/EngineInput.h>

#include <EngineCore/EngineAPICore.h>
#include <EngineCore/Level.h>

#include "Player.h"
#include "Stage.h"

#include "SoundManager.h"
#include "HUI.h"
#include "HPGauge.h"
#include "Ability.h"
#include "Score.h"
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

	NewHP = GetWorld()->SpawnActor<AHPGauge>();
	NewAbility = GetWorld()->SpawnActor<AAbility>();

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

void AStage101GameMode::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	
	NewPlayerLife->SetValue(APlayer::PlayerLife);
	NewPlayerScore->SetValue(APlayer::Score);
	NewHP->SetHPGauge(APlayer::PlayerHp);
	NewAbility->SetSprite("Ability.png", static_cast<int>(APlayer::PlayerAbility));

	NewWaddleDee->SetDistance(NewPlayer->GetActorLocation());
	NewWaddleDoo->SetDistance(NewPlayer->GetActorLocation());
	NewKibble->SetDistance(NewPlayer->GetActorLocation());


	if (true == UEngineInput::GetInst().IsDown(VK_UP) 
		&& (NewPlayer->PixelLineCheck(ECheckDir::UP, UColor::RED) || NewPlayer->PixelLineCheck(ECheckDir::DOWN, UColor::RED)))
	{

		UEngineAPICore::GetCore()->OpenLevel("MidBoss");
	}

	if (true == UEngineInput::GetInst().IsDown('T'))
	{
		NewActor->SwitchColSprite();
	}
}

void AStage101GameMode::LevelChangeStart()
{
	Super::LevelChangeStart();

	if (true == BGMPlayer.IsPlaying())
	{
		BGMPlayer.Stop();
	}
	BGMPlayer = UEngineSound::Play("Vegetable Valley 1.mp3");
	SoundManager.SetBGMSoundPlayer(BGMPlayer);
	NewFade->FadeOut();
}

void AStage101GameMode::LevelChangeEnd()
{
	Super::LevelChangeEnd();

	NewFade->FadeIn();
	BGMPlayer.Off();
}
 