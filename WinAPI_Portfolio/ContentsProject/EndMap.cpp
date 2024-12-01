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

	KirbyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	KirbyRenderer->SetOrder(ERenderOrder::PLAYER);
	KirbyRenderer->SetSprite("Kirby_Normal_Right.png");
	KirbyRenderer->CreateAnimation("Ending", "Kirby_Normal_Right.png", 58, 60, 0.4f, false);
	KirbyRenderer->ChangeAnimation("Ending");
	FVector2D KirbyScale = KirbyRenderer->SetSpriteScale(3.0f);
	KirbyRenderer->SetComponentLocation({ 380.0f, 420.0f });

	EndRenderer = CreateDefaultSubObject<USpriteRenderer>();
	EndRenderer->SetOrder(ERenderOrder::HUI);
	EndRenderer->SetSprite("End Flag.png");
	FVector2D EndScale = EndRenderer->SetSpriteScale(1.5f);
	EndRenderer->SetComponentLocation({ 423.0f, 315.0f });
}

EndMap::~EndMap()
{
}
