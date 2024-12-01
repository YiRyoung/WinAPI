#include "PreCompile.h"
#include "WisphyWood.h"

#include <EnginePlatform/EngineInput.h>

#include <EngineCore/EngineCoreDebug.h>
#include <EngineCore/EngineAPICore.h>
#include <EngineCore/ImageManager.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/2DCollision.h>

#include "Fade.h"
#include "ContentsEnum.h"

AWisphyWood::AWisphyWood()
{
	SpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
	SpriteRenderer->SetSprite("WhispyWood.png");
	SpriteRenderer->SetComponentScale({ 175, 300 });

	CollisionComponent = CreateDefaultSubObject<U2DCollision>();
	CollisionComponent->SetComponentLocation({ 12, -50 });
	CollisionComponent->SetComponentScale({ 150, 280 });
	CollisionComponent->SetCollisionGroup(ECollisionGroup::BOSS);
	CollisionComponent->SetCollisionType(ECollisionType::Rect);
	GetWorld()->CollisionGroupLink(ECollisionGroup::BOSS, ECollisionGroup::PLAYERSKILL);
	CollisionComponent->SetCollisionEnter(std::bind(&AWisphyWood::CollisionEnter, this, std::placeholders::_1));
	
	SetAnimation();
	DebugOn();
}

AWisphyWood::~AWisphyWood()
{
}

void AWisphyWood::SetPlayerLocation(FVector2D _KirbyPos)
{
	PlayerLocation = _KirbyPos;
}

void AWisphyWood::SetColImage(std::string _ColImageName)
{
	ColImage = UImageManager::GetInst().FindImage(_ColImageName);
}

void AWisphyWood::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	CurTime += _DeltaTime;

	UEngineDebug::CoreOutPutString("CurTime : " + std::to_string(CurTime));
	UEngineDebug::CoreOutPutString("CurState : " + std::to_string(static_cast<int>(CurState)));
	UEngineDebug::CoreOutPutString("PlayerLocation : " + std::to_string(PlayerLocation.X) + "," + std::to_string(PlayerLocation.Y));
	UEngineDebug::CoreOutPutString("TempPos0 : " + std::to_string(PlayerPos[0].X) + ", " + std::to_string(PlayerPos[0].Y));
	UEngineDebug::CoreOutPutString("TempPos1 : " + std::to_string(PlayerPos[1].X) + ", " +std::to_string(PlayerPos[1].Y));
	UEngineDebug::CoreOutPutString("TempPos2 : " + std::to_string(PlayerPos[2].X) + ", " +std::to_string(PlayerPos[2].Y));
	UEngineDebug::CoreOutPutString("TempPos3 : " + std::to_string(PlayerPos[3].X) + ", " +std::to_string(PlayerPos[3].Y));

	MonsterFSM(_DeltaTime);

	if (NewApple != nullptr)
	{
		NewApple->SetPlayerLocation(PlayerLocation);
	}
}

void AWisphyWood::SetAnimation()
{
	// Idle
	SpriteRenderer->CreateAnimation("Idle", "WhispyWood.png", { 0, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 0.18f);

	// Hurt
	SpriteRenderer->CreateAnimation("Hurt", "WhispyWood.png", 6, 7, 0.05f, false);

	// Die
	SpriteRenderer->CreateAnimation("Die", "WhispyWood.png", 8, 9, 0.02f, false);

	SpriteRenderer->ChangeAnimation("Idle");
}

void AWisphyWood::Pos1()
{
	if (CurTime >= 1.2f)
	{
		CurTime = 0.0f;
		PlayerPos[0] = PlayerLocation;
		CurSavePos = ESavePos::Pos2;
		return;
	}
}

void AWisphyWood::Pos2()
{
	if (CurTime >= 1.2f)
	{
		CurTime = 0.0f;
		PlayerPos[1] = PlayerLocation;
		CurSavePos = ESavePos::Pos3;
		return;
	}
}

void AWisphyWood::Pos3()
{
	if (CurTime >= 1.2f)
	{
		CurTime = 0.0f;
		PlayerPos[2] = PlayerLocation;
		CurSavePos = ESavePos::Pos4;
		return;
	}
}

void AWisphyWood::Pos4()
{
	if (CurTime >= 1.2f)
	{
		CurTime = 0.0f;
		PlayerPos[3] = PlayerLocation;
		CurSavePos = ESavePos::Pos5;
		return;
	}
}

void AWisphyWood::Pos5()
{
	CurTime = 0.0f;
	CurPhase = EAttackPhase::Phase1;
	CurState = EBossState::ATTACK;
	return;
}

void AWisphyWood::SavePlayerPos()
{
	switch (CurSavePos)
	{
	case ESavePos::Pos1:
		Pos1();
		break;
	case ESavePos::Pos2:
		Pos2();
		break;
	case ESavePos::Pos3:
		Pos3();
		break;
	case ESavePos::Pos4:
		Pos4();
		break;
	case ESavePos::Pos5:
		Pos5();
		break;
	}
}

void AWisphyWood::Phase1()
{
	if (CurTime >= 1.2f)
	{
		CurTime = 0.0f;
		NewApple = GetWorld()->SpawnActor<AApple>();
		NewApple->SetColImage(ColImage);
		NewApple->SetActorLocation({ PlayerPos[0].X, 620.0f });
		PlayerPos[0] = FVector2D::ZERO;
		CurPhase = EAttackPhase::Phase2;
		return;
	}
}

void AWisphyWood::Phase2()
{
	if (CurTime >= 1.2f)
	{
		CurTime = 0.0f;
		NewApple = GetWorld()->SpawnActor<AApple>();
		NewApple->SetColImage(ColImage);
		NewApple->SetActorLocation({ PlayerPos[1].X, 620.0f });
		PlayerPos[1] = FVector2D::ZERO;
		CurPhase = EAttackPhase::Phase3;
		return;
	}
}

void AWisphyWood::Phase3()
{
	if (CurTime >= 1.2f)
	{
		CurTime = 0.0f;
		NewApple = GetWorld()->SpawnActor<AApple>();
		NewApple->SetColImage(ColImage);
		NewApple->SetActorLocation({ PlayerPos[2].X, 620.0f });
		PlayerPos[2] = FVector2D::ZERO;
		CurPhase = EAttackPhase::Phase4;
		return;
	}
}

void AWisphyWood::Phase4()
{
	if (CurTime >= 1.2f)
	{
		CurTime = 0.0f;
		NewApple = GetWorld()->SpawnActor<AApple>();
		NewApple->SetColImage(ColImage);
		NewApple->SetActorLocation({ PlayerPos[3].X, 620.0f });
		PlayerPos[3] = FVector2D::ZERO;
		CurPhase = EAttackPhase::Phase5;
		return;
	}
}

void AWisphyWood::Phase5()
{
	if (CurTime >= 2.0f)
	{
		CurTime = 0.0f;
		CurSavePos = ESavePos::Pos1;
		CurState = EBossState::CHASE;
		return;
	}
}

void AWisphyWood::AttackApple()
{
	switch (CurPhase)
	{
	case EAttackPhase::Phase1:
		Phase1();
		break;
	case EAttackPhase::Phase2:
		Phase2();
		break;
	case EAttackPhase::Phase3:
		Phase3();
		break;
	case EAttackPhase::Phase4:
		Phase4();
		break;
	case EAttackPhase::Phase5:
		Phase5();
		break;
	}
}

void AWisphyWood::Pause(float _DeltaTime)
{
	if (CurTime >= 0.5f && PlayerLocation.Y >= 520.0f)
	{
		CurTime = 0.0f;
		CurState = EBossState::CHASE;
		return;
	}
}

void AWisphyWood::Chase(float _DeltaTime)
{
	SavePlayerPos();
}

void AWisphyWood::Attack(float _DeltaTime)
{
	AttackApple();
}

void AWisphyWood::HurtStart(float _DeltaTime)
{
	if (CurTime >= 1.0f)
	{
		CurTime = 0.0f;
		Hurt(_DeltaTime);
	}
}

void AWisphyWood::Hurt(float _DeltaTime)
{
	SpriteRenderer->ChangeAnimation("Idle");
	CurSavePos = ESavePos::Pos1;
	CurPhase = EAttackPhase::Phase1;
	CurState = EBossState::PAUSE;
	return;
}

void AWisphyWood::Died(float _DeltaTime)
{
	SpriteRenderer->ChangeAnimation("Die");
	NewFade = GetWorld()->SpawnActor<AFade>();
	NewFade->FadeIn();
	CurTime = 0.0f;
	CurState = EBossState::END;
	return;
}

void AWisphyWood::NextLevel()
{
	if (CurTime >= 2.0f)
	{
		UEngineAPICore::GetCore()->OpenLevel("Ending");
	}
}

void AWisphyWood::CollisionEnter(AActor* _ColActor)
{
	CurTime = 0.0f;
	BossHP--;
	if (true == BGMPlayer.IsPlaying())
	{
		BGMPlayer.Stop();
	}
	BGMPlayer = UEngineSound::Play("HurtNormal.wav");

	_ColActor->SetActive(false);
	
	if (BossHP > 0)
	{
		SpriteRenderer->ChangeAnimation("Hurt");
		CurState = EBossState::HURT;
	}
	else if (BossHP <= 0)
	{
		CurState = EBossState::DIE;
	}
}

void AWisphyWood::MonsterFSM(float _DeltaTime)
{
	switch (CurState)
	{
	case EBossState::PAUSE:
		Pause(_DeltaTime);
		break;
	case EBossState::CHASE:
		Chase(_DeltaTime);
		break;
	case EBossState::ATTACK:
		Attack(_DeltaTime);
		break;
	case EBossState::HURT:
		HurtStart(_DeltaTime);
		break;
	case EBossState::DIE:
		Died(_DeltaTime);
		break;
	case EBossState::END:
		NextLevel();
		break;
	}


}

