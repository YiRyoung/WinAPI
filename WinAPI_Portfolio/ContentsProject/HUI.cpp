#include "PreCompile.h"
#include "HUI.h"

#include <EngineCore/EngineAPICore.h>
#include <EngineCore/SpriteRenderer.h>

#include "ContentsEnum.h"

AHUI::AHUI()
{
	HUIRenderer = CreateDefaultSubObject<USpriteRenderer>();
	HUIRenderer->SetOrder(ERenderOrder::HUI);
	HUIRenderer->SetCameraEffect(false);

	IconRenderer = CreateDefaultSubObject<USpriteRenderer>();
	IconRenderer->SetOrder(ERenderOrder::HUITEXT);
	IconRenderer->SetCameraEffect(false);

	IconRenderer->CreateAnimation("Living", "Lives.png", 0, 3, 0.4f);
	IconRenderer->ChangeAnimation("Living");
}

AHUI::~AHUI()
{
}

void AHUI::SetSprite(std::string _HUI, std::string_view _Icon)
{
		HUIRenderer->SetSprite(_HUI);

		MapScale = HUIRenderer->SetSpriteScale(1.0f);

		FVector2D HUISize = UEngineAPICore::GetCore()->GetMainWindow().GetWindowSize();
		FVector2D HUIPos = { 0.0f, HUISize.Y - MapScale.Y + 1.0f };
		HUIRenderer->SetComponentLocation(MapScale.Half() + HUIPos);
		

		IconRenderer->SetSprite(_Icon);
		MapScale = IconRenderer->SetSpriteScale(1.0f);
		FVector2D IconSize = UEngineAPICore::GetCore()->GetMainWindow().GetWindowSize();
		FVector2D IconPos = { IconSize.X - MapScale.X, IconSize.Y - MapScale.Y };
		IconRenderer->SetComponentLocation(MapScale.Half() + IconPos + FVector2D({ -143, -99 }));
}

