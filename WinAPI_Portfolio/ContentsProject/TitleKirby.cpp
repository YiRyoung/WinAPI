#include "PreCompile.h"
#include "TitleKirby.h"

#include <EngineCore/EngineAPICore.h>
#include <EngineCore/SpriteRenderer.h>

#include "ContentsEnum.h"

TitleKirby::TitleKirby()
{
	{
		USpriteRenderer* SpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
		SpriteRenderer->SetOrder(ERenderOrder::PLAYER);
		SpriteRenderer->SetSprite("TitleKirby.png");

		FVector2D MapScale = SpriteRenderer->SetSpriteScale(1.0f);
		SpriteRenderer->SetComponentLocation({380.0f, 360.0f});
	}
}

TitleKirby::~TitleKirby()
{
}

