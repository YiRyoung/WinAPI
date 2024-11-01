#include "PreCompile.h"
#include "Player.h"
#include <EngineCore/EngineAPICore.h>
#include <EnginePlatform/EngineInput.h>
#include "Bullet.h"

APlayer::APlayer()
{
	// UEngineAPICore::GetCore()->CreateLevel("Title");
	SetActorLocation({ 100, 100 });
	SetActorScale({ 100, 100 });
	SetSprite("Title_Kirby.png");
}

APlayer::~APlayer()
{
}


void APlayer::BeginPlay()
{
	Super::BeginPlay();

	UEngineInput::GetInst().BindAction(VK_LEFT, KeyEvent::Press, std::bind(&APlayer::LeftMove, this, std::placeholders::_1));
	UEngineInput::GetInst().BindAction(VK_RIGHT, KeyEvent::Press, std::bind(&APlayer::RightMove, this, std::placeholders::_1));
	UEngineInput::GetInst().BindAction(VK_DOWN, KeyEvent::Press, std::bind(&APlayer::DownMove, this, std::placeholders::_1));
	UEngineInput::GetInst().BindAction(VK_UP, KeyEvent::Press, std::bind(&APlayer::UpMove, this, std::placeholders::_1));

}

void APlayer::LeftMove(float _DeltaTime)
{
	AddActorLocation(FVector2D::LEFT * _DeltaTime * Speed);
}

void APlayer::RightMove(float _DeltaTime)
{
	AddActorLocation(FVector2D::RIGHT * _DeltaTime * Speed);
}

void APlayer::UpMove(float _DeltaTime)
{
	AddActorLocation(FVector2D::UP * _DeltaTime * Speed);
}

void APlayer::DownMove(float _DeltaTime)
{
	AddActorLocation(FVector2D::DOWN * _DeltaTime * Speed);
}

void APlayer::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	if (true == UEngineInput::GetInst().IsDown('R'))
	{
		SetSprite("Title_Kirby.png", MySpriteIndex);
		++MySpriteIndex;
	}


}