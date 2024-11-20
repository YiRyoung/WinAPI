#include "PreCompile.h"
#include "PlayerAbility.h"

#include <EnginePlatform/EngineInput.h>

PlayerAbility::PlayerAbility()
{
}

PlayerAbility::PlayerAbility(APlayer* _Player) : Player(_Player)
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
		case EAblityType::NORMAL:
			NormalAttack(_DeltaTime);
			break;
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

	if (!UEngineInput::GetInst().IsPress('X'))
	{
		if (EAblityType::NORMAL == GetAbility())
		{
			//ChangeAnimation("InhaleEnd");
			if ("_Right" == GetAnimDir())
			{
				SetInhaleRight(false);
			}
			else
			{
				SetInhaleLeft(false);
			}
		}
		//Player->SetState(StateType::IDLE);
		//return;
	}
}

void PlayerAbility::NormalAttack(float _DeltaTime)
{
	ChangeAnimation("InhaleStart");
	std::string Test = GetAnimDir();
	if ("_Right" == GetAnimDir())
	{
		SetInhaleRight(true);
	}
	else
	{
		SetInhaleLeft(true);
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

