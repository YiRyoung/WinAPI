#include "PreCompile.h"
#include "Stage101GameMode.h"

#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineAPICore.h>

#include <EngineCore/Level.h>
#include "Stage101.h"
#include "Player.h"
#include "Stage.h"

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

	APlayer* Player = dynamic_cast<APlayer*>(GetWorld()->GetPawn());
	Player->CameraPivot = NewActor->MapScale;

}

void AStage101GameMode::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	if (true == UEngineInput::GetInst().IsDown('R'))
	{
		UEngineAPICore::GetCore()->OpenLevel("Stage102");
	}

	// �� �̹����� ũ�⸦ �����ͼ� ī�޶� �ǹ� ������ ���⼭ ���� 
}
 