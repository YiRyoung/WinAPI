#include "PreCompile.h"
#include "TitleMap.h"

#include <EngineCore/EngineAPICore.h>
#include <EngineCore/SpriteRenderer.h>
#include "ContentsEnum.h"

TitleMap::TitleMap()
{
	{
		USpriteRenderer* SpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
		SpriteRenderer->SetOrder(ERenderOrder::BACKGROUND);
		SpriteRenderer->SetSprite("Title.png");

		FVector2D MapScale = SpriteRenderer->SetSpriteScale(1.0f);
		SpriteRenderer->SetComponentLocation(MapScale.Half());
	}

	{
		USpriteRenderer* KirbyRenderer = CreateDefaultSubObject<USpriteRenderer>();
		KirbyRenderer->SetOrder(ERenderOrder::PLAYER);
		KirbyRenderer->SetSprite("TitleKirby.png");

		FVector2D KirbyScale = KirbyRenderer->SetSpriteScale(1.0f);
		KirbyRenderer->SetComponentLocation({ 380.0f, 360.0f });
	}
}

TitleMap::~TitleMap()
{
}

