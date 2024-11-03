#include "PreCompile.h"
#include "TitleKirby.h"

#include <EngineCore/SpriteRenderer.h>

#include "ContentsEnum.h"

ATitleKirby::ATitleKirby()
{
	USpriteRenderer* SpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
	SpriteRenderer->SetOrder(EContentsEnum::FOREGROUND);
	SpriteRenderer->SetSprite("Title_Kirby_64.png");

	FVector2D MapScale = SpriteRenderer->SetSpriteScale(4.0f);
	SpriteRenderer->SetComponentLocation(FVector2D({500.f, 480.f}));
}

ATitleKirby::~ATitleKirby()
{
}

