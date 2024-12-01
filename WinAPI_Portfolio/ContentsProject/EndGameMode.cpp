#include "PreCompile.h"
#include "EndGameMode.h"

#include "Fade.h"
#include "EndMap.h"

AEndGameMode::AEndGameMode()
{
}

AEndGameMode::~AEndGameMode()
{
}

void AEndGameMode::BeginPlay()
{
	Super::BeginPlay();

	EndMap* NewEndMap = GetWorld()->SpawnActor<EndMap>();
	NewFade = GetWorld()->SpawnActor<AFade>();
}

void AEndGameMode::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
}

void AEndGameMode::LevelChangeStart()
{
	Super::LevelChangeStart();

	if (true == BGMPlayer.IsPlaying())
	{
		BGMPlayer.Stop();
	}
	BGMPlayer = UEngineSound::Play("Stage Clear.mp3");
}

void AEndGameMode::LevelChangeEnd()
{
	Super::LevelChangeEnd();
}
