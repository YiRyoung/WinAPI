#include "PreCompile.h"
#include "Stage101GameMode.h"

#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineAPICore.h>

#include <EngineCore/Level.h>
#include "Player.h"
#include "WaddleDee.h"
#include "Stage.h"
#include "HUI.h"

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

	NewActor = GetWorld()->SpawnActor<AStage>();
	NewActor->SetSprite("Stage101.png");
	NewActor->SetColSprite("ColStage101.png");
	NewActor->SwitchColSprite();

	APlayer* Player = dynamic_cast<APlayer*>(GetWorld()->GetPawn());
	Player->GetBackImage("Stage101.png", "ColStage101.png");
	Player->SetActorLocation({ 100, 366 });

	NewUI = GetWorld()->SpawnActor<AHUI>();
	NewUI->SetSprite("StageUI.png", "Lives.png");

	AWaddleDee* NewWaddle = GetWorld()->SpawnActor<AWaddleDee>();
	NewWaddle->GetBackImage("ColStage101.png");
	NewWaddle->SetActorLocation({ 801, 400 });

	// AMonster를 상속받은 그대로 호출하면 됨.
	// AMonster NewWaddle = GetWorld()->SpawnActor<AWAddleDee>();
}

void AStage101GameMode::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	if (true == UEngineInput::GetInst().IsDown('R'))
	{
		UEngineAPICore::GetCore()->OpenLevel("MidBoss");
	}

	if (true == UEngineInput::GetInst().IsDown('T'))
	{
		NewActor->SwitchColSprite();
	}
}
 