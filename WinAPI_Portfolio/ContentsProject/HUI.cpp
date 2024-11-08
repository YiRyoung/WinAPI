#include "PreCompile.h"
#include "HUI.h"

#include <EngineCore/EngineAPICore.h>
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

		MapScale = SpriteRenderer->SetSpriteScale(1.0f);

		FVector2D Size = UEngineAPICore::GetCore()->GetMainWindow().GetWindowSize();
		FVector2D Pos = { 0.0f, Size.Y - MapScale.Y + 1.0f };
		SpriteRenderer->SetComponentLocation(MapScale.Half() + Pos);
}

