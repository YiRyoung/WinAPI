#include "PreCompile.h"
#include "Stage101.h"

#include <EngineCore/EngineAPICore.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/EngineCoreDebug.h>

#include "ContentsEnum.h"

#include "Player.h"

AStage101::AStage101()
{
	{
		USpriteRenderer* SpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
		SpriteRenderer->SetOrder(ERenderOrder::BACKGROUND);
		SpriteRenderer->SetSprite("Stage101.png");

		FVector2D MapScale = SpriteRenderer->SetSpriteScale(4.0f);
		SpriteRenderer->SetComponentLocation(MapScale.Half());
	}

	LeftTopPos = { 0, 0 };
	RightBottomPos = { 1016, 168 };
}

AStage101::~AStage101()
{
}
