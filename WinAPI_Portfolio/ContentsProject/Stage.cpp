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
}

AStage::~AStage()
{
}

void AStage::SetSprite(std::string _Sprite)
{
	SpriteRenderer->SetSprite(_Sprite);

	MapScale = SpriteRenderer->SetSpriteScale(4.0f);
	SpriteRenderer->SetComponentLocation(MapScale.Half());
}