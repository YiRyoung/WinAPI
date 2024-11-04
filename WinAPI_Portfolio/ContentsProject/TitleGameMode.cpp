#include "PreCompile.h"
#include "TitleGameMode.h"

#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineAPICore.h>

#include "TitleMap.h"
#include "TitleKirby.h"

ATitleGameMode::ATitleGameMode()
{
}

ATitleGameMode::~ATitleGameMode()
{
}


// 언리얼에서는 MainPawn 주인공 무조건 지정하게 해요.
void ATitleGameMode::BeginPlay()
{
	Super::BeginPlay();
	TitleMap* NewMap = GetWorld()->SpawnActor<TitleMap>();
	TitleKirby* NewActor = GetWorld()->SpawnActor<TitleKirby>();
}


void ATitleGameMode::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	if (true == UEngineInput::GetInst().IsDown(VK_SPACE))
	{
		UEngineAPICore::GetCore()->OpenLevel("Stage101");
	}

}