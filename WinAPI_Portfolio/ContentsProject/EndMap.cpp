#include "PreCompile.h"
#include "EndMap.h"

#include <EngineCore/SpriteRenderer.h>

#include "ContentsEnum.h"

EndMap::EndMap()
{
	{
		USpriteRenderer* SpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
		SpriteRenderer->SetOrder(ERenderOrder::BACKGROUND);
		SpriteRenderer->SetSprite("Fade.png");

		FVector2D MapScale = SpriteRenderer->SetSpriteScale(1.0f);
		SpriteRenderer->SetComponentLocation(MapScale.Half());
	}

	{
		USpriteRenderer* KirbyRenderer = CreateDefaultSubObject<USpriteRenderer>();
		KirbyRenderer->SetOrder(ERenderOrder::PLAYER);
		KirbyRenderer->SetSprite("Kirby_Normal_Right.png");
		KirbyRenderer->CreateAnimation("Ending", "Kirby_Normal_Right.png", 65, 70, 0.04f, false);

		FVector2D KirbyScale = KirbyRenderer->SetSpriteScale(1.0f);
		KirbyRenderer->SetComponentLocation({ 380.0f, 360.0f });
	}
}

EndMap::~EndMap()
{
}

