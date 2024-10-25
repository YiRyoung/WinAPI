#include "PreCompile.h"
#include "Player.h"
#include <EngineCore/EngineAPICore.h>

APlayer::APlayer()
{
	SetActorLoaction({ 100, 100 });
	SetActorScale({ 100, 100 });
}

APlayer::~APlayer()
{
}


void APlayer::BeginPlay()
{

}

void APlayer::Tick(float _DeltaTime)
{
	AddActorLoaction(FVector2D::RIGHT * _DeltaTime * Speed);
}