#include "PreCompile.h"
#include "Stage101GameMode.h"

#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineAPICore.h>

#include <EngineCore/Level.h>
#include "Player.h"
#include "WindBullet.h"
#include "SpitBullet.h"
#include "Monster.h"
#include "WaddleDee.h"
#include "HotHead.h"
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

	AMonster* NewWaddle = GetWorld()->SpawnActor<AWaddleDee>();
	NewWaddle->GetColImage("ColStage101.png");
	NewWaddle->SetAnimDir(NewPlayer->GetActorLocation());
	NewWaddle->SetActorLocation({ 1191, 280 });

	NewHotHead = GetWorld()->SpawnActor<AHotHead>();
	NewHotHead->GetColImage("ColStage101.png");
	NewHotHead->SetAnimDir(NewPlayer->GetActorLocation());
	NewHotHead->SetPos(NewPlayer->GetActorLocation());
	NewHotHead->SetActorLocation({ 400, 360 });
}

void AStage101GameMode::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	NewHotHead->SetPos(NewPlayer->GetActorLocation());

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
		PlayerDir = NewPlayer->GetAnimDir();
		NewBullet->SetDir(PlayerDir);
		FVector2D Pos = NewBullet->GetSpawnPos();
		NewBullet->SetActorLocation(NewPlayer->GetActorLocation() + Pos);
	}

	if (NewPlayer->GetFull() && true == UEngineInput::GetInst().IsDown('X'))
	{
		NewStar = GetWorld()->SpawnActor<ASpitBullet>();
		PlayerDir = NewPlayer->GetAnimDir();
		NewStar->SetDir(PlayerDir);
		FVector2D Pos = NewStar->GetSpawnPos();
		NewStar->SetActorLocation(NewPlayer->GetActorLocation() + Pos);
	}

}
 