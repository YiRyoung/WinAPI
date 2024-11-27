#include "PreCompile.h"
#include "HPGauge.h"

#include <EngineCore/SpriteRenderer.h>

#include "ContentsEnum.h"

AHPGauge::AHPGauge()
{
	for (int i = 0; i < 6; i++)
	{
		HPRender[i] = CreateDefaultSubObject<USpriteRenderer>();
		HPRender[i]->SetSprite("HP.png", 1);
		HPRender[i]->SetOrder(ERenderOrder::HUITEXT);
		HPRender[i]->SetComponentLocation({240 + (i * 25), 585});
		HPRender[i]->SetComponentScale({ 25, 44 });
		HPRender[i]->SetCameraEffect(false);

		HPRender[i]->CreateAnimation("HP", "HP.png", 0, 1, 0.4f);
		HPRender[i]->ChangeAnimation("HP");
	}
}

AHPGauge::~AHPGauge()
{
}

void AHPGauge::SetHPGauge(int _PlayerHP)
{
	for (int i = 6; i > _PlayerHP; i--)
	{
		HPRender[i - 1]->SetActive(false);
	}
}

