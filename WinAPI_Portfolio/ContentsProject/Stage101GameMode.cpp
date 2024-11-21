#include "PreCompile.h"
#include "Stage101GameMode.h"

#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineAPICore.h>

#include <EngineCore/Level.h>
#include "Player.h"
#include "WindBullet.h"
#include "Monster.h"
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

	NewPlayer = dynamic_cast<APlayer*>(GetWorld()->GetPawn());
	NewPlayer->GetBackImage("Stage101.png", "ColStage101.png");
	NewPlayer->SetActorLocation({ 100, 366 });

	NewUI = GetWorld()->SpawnActor<AHUI>();
	NewUI->SetSprite("StageUI.png", "Lives.png");

	// AMonster를 상속받은 그대로 호출하면 됨.
	AMonster* NewWaddle = GetWorld()->SpawnActor<AWaddleDee>();
	NewWaddle->SetActorLocation({ 300, 360 });
}

void AStage101GameMode::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	PlayerDir = NewPlayer->GetAnimDir();

	if (true == UEngineInput::GetInst().IsDown('R'))
	{
		UEngineAPICore::GetCore()->OpenLevel("MidBoss");
	}

	if (true == UEngineInput::GetInst().IsDown('T'))
	{
		NewActor->SwitchColSprite();
	}

	if (NewPlayer->GetState() == EStateType::FLYING &&
		true == UEngineInput::GetInst().IsDown('X'))
	{
		NewBullet = GetWorld()->SpawnActor<AWindBullet>();
		NewBullet->SetDir(PlayerDir);
		FVector2D Pos = NewBullet->GetSpawnPos();
		NewBullet->SetActorLocation(NewPlayer->GetActorLocation() + Pos);
	}
}
 