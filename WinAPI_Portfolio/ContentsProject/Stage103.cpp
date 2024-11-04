#include "PreCompile.h"
#include "Stage103.h"

#include <EngineCore/EngineAPICore.h>
#include <EngineCore/SpriteRenderer.h>

#include "ContentsEnum.h"

AStage103::AStage103()
{
	{
		USpriteRenderer* SpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
		SpriteRenderer->SetOrder(ERenderOrder::BACKGROUND);
		SpriteRenderer->SetSprite("Stage103.png");

		FVector2D MapScale = SpriteRenderer->SetSpriteScale(4.0f);
		SpriteRenderer->SetComponentLocation(MapScale.Half());
	}
}

AStage103::~AStage103()
{
}

