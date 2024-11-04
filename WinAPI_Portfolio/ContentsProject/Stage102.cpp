#include "PreCompile.h"
#include "Stage102.h"

#include <EngineCore/EngineAPICore.h>
#include <EngineCore/SpriteRenderer.h>

#include "ContentsEnum.h"

AStage102::AStage102()
{
	{
		USpriteRenderer* SpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
		SpriteRenderer->SetOrder(ERenderOrder::BACKGROUND);
		SpriteRenderer->SetSprite("Stage102.png");

		FVector2D MapScale = SpriteRenderer->SetSpriteScale(4.0f);
		SpriteRenderer->SetComponentLocation(MapScale.Half());
	}
}

AStage102::~AStage102()
{
}

