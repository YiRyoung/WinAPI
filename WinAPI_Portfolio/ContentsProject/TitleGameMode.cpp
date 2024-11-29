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

	BGMPlayer = UEngineSound::Play("Title Theme.mp3");
	BGMPlayer.Loop(10);
	SoundManager.SetBGMSoundPlayer(BGMPlayer);

	TitleMap* NewMap = GetWorld()->SpawnActor<TitleMap>();
}


void ATitleGameMode::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	if (true == UEngineInput::GetInst().IsDown(VK_SPACE))
	{
		AFade* Actor = GetWorld()->SpawnActor<AFade>();
		Actor->FadeIn();
		BGMPlayer.Off();
		EffectPlayer = UEngineSound::Play("GameStart.wav");
		SoundManager.SetEffectSoundPlayer(EffectPlayer);
		UEngineAPICore::GetCore()->OpenLevel("Stage101");
	}
}