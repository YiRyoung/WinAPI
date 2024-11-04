#include "PreCompile.h"
#include "Player.h"
#include "Stage101.h"

#include <EngineCore/EngineAPICore.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/EngineCoreDebug.h>

#include <EnginePlatform/EngineInput.h>

APlayer::APlayer()
{
	SetActorLocation({100, 100});

	{
		SpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
		SpriteRenderer->SetSprite("Kirby_Idle_Right_18.png");
		SpriteRenderer->SetComponentScale({ 72, 72 });

		// 오른쪽
		SpriteRenderer->CreateAnimation("Walk_Right", "Kirby_Walk_Right_18.png", 0, 4, 1.0f);
		SpriteRenderer->CreateAnimation("Idle_Right", "Kirby_Idle_Right_18.png", 0, 1, 1.0f);

		SpriteRenderer->ChangeAnimation("Idle_Right");
	}
}

APlayer::~APlayer()
{
}


void APlayer::BeginPlay()
{
	Super::BeginPlay();

	// 메인 윈도우 사이즈 가져오는 코드 {1040, 960}
	FVector2D Size = UEngineAPICore::GetCore()->GetMainWindow().GetWindowSize();

	// {520, 480} * -1 = {-520, -480}
	GetWorld()->SetCameraPivot(Size.Half() * -1.0f);
	//APlayer* player = dynamic_cast GetWorld()->GetPawn();
}

void APlayer::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime); 

	UEngineDebug::CoreOutPutString("FPS : " + std::to_string(1.0f / _DeltaTime));
	UEngineDebug::CoreOutPutString("PlayerPos : " + GetActorLocation().ToString());

	if (true == UEngineInput::GetInst().IsPress(VK_RIGHT))
	{
		SpriteRenderer->ChangeAnimation("Walk_Right");
		AddActorLocation(FVector2D::RIGHT * _DeltaTime * Speed);
	}
	if (true == UEngineInput::GetInst().IsPress(VK_LEFT))
	{
		SpriteRenderer->ChangeAnimation("Walk_Right");
		AddActorLocation(FVector2D::LEFT * _DeltaTime * Speed);
	}
	if (true == UEngineInput::GetInst().IsPress(VK_DOWN))
	{
		AddActorLocation(FVector2D::DOWN * _DeltaTime * Speed);
	}
	if (true == UEngineInput::GetInst().IsPress(VK_UP))
	{
		AddActorLocation(FVector2D::UP * _DeltaTime * Speed);
	}
}

void APlayer::LevelChangeStart()
{
	Super::LevelChangeStart();
}

void APlayer::LevelChangeEnd()
{
	Super::LevelChangeEnd();


}