#include "PreCompile.h"
#include "BossStage.h"

#include <EngineCore/EngineAPICore.h>
#include <EngineCore/SpriteRenderer.h>

#include "ContentsEnum.h"

ABossStage::ABossStage()
{
	{
		USpriteRenderer* SpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
		SpriteRenderer->SetOrder(ERenderOrder::BACKGROUND);
		SpriteRenderer->SetSprite("Whispy Woods.png");

		FVector2D MapScale = SpriteRenderer->SetSpriteScale(4.0f);
		SpriteRenderer->SetComponentLocation(MapScale.Half());
	}
}

ABossStage::~ABossStage()
{
}

