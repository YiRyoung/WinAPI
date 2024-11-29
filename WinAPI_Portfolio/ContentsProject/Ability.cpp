#include "PreCompile.h"
#include "Ability.h"

#include <EngineCore/EngineAPICore.h>
#include <EngineCore/SpriteRenderer.h>

#include "ContentsEnum.h"

AAbility::AAbility()
{
	AbilityRenderer = CreateDefaultSubObject<USpriteRenderer>();
	AbilityRenderer->SetOrder(ERenderOrder::HUITEXT);
	AbilityRenderer->SetCameraEffect(false);
}

AAbility::~AAbility()
{
}

void AAbility::SetSprite(std::string _Ability, int _CurAbilityTpye)
{
	AbilityRenderer->SetSprite(_Ability, _CurAbilityTpye);
	SpriteScale = AbilityRenderer->SetSpriteScale(1.0f);

	FVector2D SprtieSize = UEngineAPICore::GetCore()->GetMainWindow().GetWindowSize();
	FVector2D SprtiePos = { 0.0f, SprtieSize.Y - SpriteScale.Y + 1.0f };
	AbilityRenderer->SetComponentLocation(SpriteScale.Half() + SprtiePos + FVector2D({450, -50}));

}

