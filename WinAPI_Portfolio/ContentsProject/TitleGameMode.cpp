#include "PreCompile.h"
#include "TitleGameMode.h"

#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineAPICore.h>

#include "Fade.h"
#include "TitleMap.h"

ATitleGameMode::ATitleGameMode()
{
}

ATitleGameMode::~ATitleGameMode()
{
}

void ATitleGameMode::BeginPlay()
{
	Super::BeginPlay();

	TitleMap* NewMap = GetWorld()->SpawnActor<TitleMap>();
	Fade = GetWorld()->SpawnActor<AFade>();
}


void ATitleGameMode::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	if (true == UEngineInput::GetInst().IsDown(VK_SPACE))
	{
		Fade->FadeIn();
		UEngineAPICore::GetCore()->OpenLevel("Stage101");
	}
}

void ATitleGameMode::LevelChangeStart()
{
	Super::LevelChangeStart();
	
	BGMPlayer = UEngineSound::Play("Title Theme.mp3");
	Fade->FadeOut();
	SoundManager.SetBGMSoundPlayer(BGMPlayer);
}

void ATitleGameMode::LevelChangeEnd()
{
	Super::LevelChangeEnd();

	BGMPlayer.Off();
	BGMPlayer = UEngineSound::Play("GameStart.wav");
	SoundManager.SetBGMSoundPlayer(BGMPlayer);

}