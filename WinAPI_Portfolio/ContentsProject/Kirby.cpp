#include "PreCompile.h"
#include "Kirby.h"

#include <EngineCore/EngineAPICore.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/EngineCoreDebug.h>

#include <EnginePlatform/EngineInput.h>

AKirby::AKirby()
{
	SetActorLocation({ 100, 100 });

	SpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
	SpriteRenderer->SetSprite("Kirby_Idle_Right_18.png");
	SpriteRenderer->SetComponentScale({ 72, 72 });
}

AKirby::~AKirby()
{
	SetActorLocation({ 10, 100 });

	SpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
	SpriteRenderer->SetSprite("Kirby_Idle_Right_18.png");
	SpriteRenderer->SetComponentScale({ 300, 300 });
}

void AKirby::BeginPlay()
{
	Super::BeginPlay();

	FVector2D Size = UEngineAPICore::GetCore()->GetMainWindow().GetWindowSize();
	GetWorld()->SetCameraPivot(Size.Half() * -1.0f);
}

void AKirby::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	UEngineDebug::CoreOutPutString("FPS : " + std::to_string(1.0f / _DeltaTime));
	UEngineDebug::CoreOutPutString("PlayerPos : " + GetActorLocation().ToString());

	if (true == UEngineInput::GetInst().IsDown('R'))
	{
		//UEngineAPICore::GetCore()->OpenLevel("Title");
		// UEngineDebug::SwitchIsDebug();
	}
}

void AKirby::MoveFunction(FVector2D _Dir)
{
}
