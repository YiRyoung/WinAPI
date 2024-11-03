#include "PreCompile.h"
#include "TitleMap.h"

#include <EngineCore/SpriteRenderer.h>

#include "ContentsEnum.h"

ATitleMap::ATitleMap()
{
	USpriteRenderer* SpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
	SpriteRenderer->SetOrder(EContentsEnum::BACKGROUND);
	SpriteRenderer->SetSprite("Title_Eng.png");

	FVector2D MapScale = SpriteRenderer->SetSpriteScale(1.0f);
	SpriteRenderer->SetComponentLocation(MapScale.Half());
}

ATitleMap::~ATitleMap()
{
}

