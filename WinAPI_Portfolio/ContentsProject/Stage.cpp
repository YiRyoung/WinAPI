#include "PreCompile.h"
#include "Stage.h"

#include <EngineCore/EngineAPICore.h>
#include <EngineCore/SpriteRenderer.h>

#include "ContentsEnum.h"

AStage::AStage()
{
	{
		SpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
		SpriteRenderer->SetOrder(ERenderOrder::BACKGROUND);
	}

	{
		ColSpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
		ColSpriteRenderer->SetOrder(ERenderOrder::COLGROUND);
	}
}

AStage::~AStage()
{
}

void AStage::SetSprite(std::string _Sprite)
{
	SpriteRenderer->SetSprite(_Sprite);
	
	MapScale = SpriteRenderer->SetSpriteScale(1.0f);
	SpriteRenderer->SetComponentLocation(MapScale.Half());
}

void AStage::SetColSprite(std::string _Sprite)
{
	ColSpriteRenderer->SetSprite(_Sprite);

	MapScale = ColSpriteRenderer->SetSpriteScale(1.0f);
	ColSpriteRenderer->SetComponentLocation(MapScale.Half());
}