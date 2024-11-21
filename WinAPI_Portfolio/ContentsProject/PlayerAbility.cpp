#include "PreCompile.h"
#include "PlayerAbility.h"

#include <EnginePlatform/EngineInput.h>

PlayerAbility::PlayerAbility()
{
}

PlayerAbility::~PlayerAbility()
{
}

void PlayerAbility::Attack(float _DeltaTime)
{
	if (UEngineInput::GetInst().IsPress('X'))
	{
		switch (GetAbility())
		{
		case EAblityType::FIRE:
			FireAttack(_DeltaTime);
			break;
		case EAblityType::CUTTER:
			CutterAttack(_DeltaTime);
			break;
		case EAblityType::SPARK:
			SparkAttack(_DeltaTime);
			break;
		}
	}
}

void PlayerAbility::FireAttack(float _DeltaTime)
{
}

void PlayerAbility::CutterAttack(float _DeltaTime)
{
}

void PlayerAbility::SparkAttack(float _DeltaTime)
{
}

