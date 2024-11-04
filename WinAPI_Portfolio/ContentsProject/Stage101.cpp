#include "PreCompile.h"
#include "Stage101.h"

#include <EngineCore/EngineAPICore.h>
#include <EngineCore/SpriteRenderer.h>

#include "ContentsEnum.h"

AStage101::AStage101()
{
	{
		USpriteRenderer* SpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
		SpriteRenderer->SetOrder(ERenderOrder::BACKGROUND);
		SpriteRenderer->SetSprite("Stage101.png");

		FVector2D MapScale = SpriteRenderer->SetSpriteScale(4.0f);
		SpriteRenderer->SetComponentLocation(MapScale.Half());
	}
}

AStage101::~AStage101()
{
}

