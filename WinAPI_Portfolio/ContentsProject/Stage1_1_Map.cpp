#include "PreCompile.h"
#include "Stage1_1_Map.h"

#include <EngineCore/SpriteRenderer.h>

#include "ContentsEnum.h"

AStage1_1_Map::AStage1_1_Map()
{
	USpriteRenderer* SpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
	SpriteRenderer->SetOrder(EContentsEnum::BACKGROUND);
	SpriteRenderer->SetSprite("Stage 1-1.png");

	FVector2D MapScale = SpriteRenderer->SetSpriteScale(0.8f);
	SpriteRenderer->SetComponentLocation((MapScale.Half()));
}

AStage1_1_Map::~AStage1_1_Map()
{
}

