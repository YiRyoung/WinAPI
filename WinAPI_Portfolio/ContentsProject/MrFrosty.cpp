#include "PreCompile.h"
#include "MrFrosty.h"

#include <EnginePlatform/EngineWinImage.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/2DCollision.h>

AMrFrosty::AMrFrosty()
{
	SpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
	SpriteRenderer->SetSprite("MrFrosty_Left.png");
	SpriteRenderer->SetComponentScale({ 75, 84 });
}

AMrFrosty::~AMrFrosty()
{
}

void AMrFrosty::BeginPlay()
{
	Super::BeginPlay();
}

void AMrFrosty::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
}

void AMrFrosty::SetMonster()
{

}

void AMrFrosty::SetAnimation()
{
}

