#include "PreCompile.h"7
#include "LiveIcon.h"


#include <EngineCore/EngineAPICore.h>
#include <EngineCore/SpriteRenderer.h>

#include "ContentsEnum.h"


ALiveIcon::ALiveIcon()
{
	SpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
	SpriteRenderer->SetOrder(ERenderOrder::HUITEXT);
	SpriteRenderer->SetCameraEffect(false);

	SpriteRenderer->CreateAnimation("Living", "Lives.png", 0, 3, 0.4f);
	SpriteRenderer->ChangeAnimation("Living");
}

ALiveIcon::~ALiveIcon()
{
}

void ALiveIcon::SetSprite(std::string _Sprite)
{
	SpriteRenderer->SetSprite(_Sprite);

	MapScale = SpriteRenderer->SetSpriteScale(1.0f);

	FVector2D Size = UEngineAPICore::GetCore()->GetMainWindow().GetWindowSize();
	FVector2D Pos = { Size.X - MapScale.X, Size.Y - MapScale.Y };
	SpriteRenderer->SetComponentLocation(MapScale.Half() + Pos + FVector2D({-143, -99}));
}

