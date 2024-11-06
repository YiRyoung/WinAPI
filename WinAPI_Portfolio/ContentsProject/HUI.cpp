#include "PreCompile.h"
#include "HUI.h"

#include <EngineCore/SpriteRenderer.h>

#include "ContentsEnum.h"

AHUI::AHUI()
{
	SpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
	SpriteRenderer->SetOrder(ERenderOrder::HUI);
	SpriteRenderer->SetCameraEffect(false);
}

AHUI::~AHUI()
{
}

void AHUI::SetSprite(std::string _Sprite)
{
		SpriteRenderer->SetSprite(_Sprite);

		MapScale = SpriteRenderer->SetSpriteScale(0.75f);
		SpriteRenderer->SetComponentLocation(MapScale.Half() + FVector2D({0, 525}));
}

