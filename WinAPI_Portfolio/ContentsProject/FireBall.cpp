#include "PreCompile.h"
#include "FireBall.h"

#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/2DCollision.h>

#include "Player.h"
#include "Monster.h"

#include "ContentsEnum.h"

AFireBall::AFireBall()
{
	FireballRender = CreateDefaultSubObject<USpriteRenderer>();
	FireballRender->SetSprite("HotHeadFire_Left.png");
	FireballRender->SetOrder(ERenderOrder::SKILL);
	FireballRender->SetComponentScale({ 104, 67 });

	FireballRender->CreateAnimation("Fireball_Left", "HotHeadFire_Left.png", 0, 1, 0.08f);
	FireballRender->CreateAnimation("Fireball_Right", "HotHeadFire_Right.png", 0, 1, 0.08f);
	FireballRender->ChangeAnimation("Fireball_Left");

	if (true == BGMPlayer.IsPlaying())
	{
		BGMPlayer.Stop();
	}
	BGMPlayer = UEngineSound::Play("Fire.wav");
	BGMPlayer.Loop(2);
	DebugOn();
}

AFireBall::~AFireBall()
{
}

void AFireBall::SetDir(const FVector2D& _Dir)
{
	Dir = _Dir;
	AnimDir = (_Dir == FVector2D::LEFT) ? "_Left" : "_Right";
}

void AFireBall::SetMosnterSkillCollision()
{
	FireballCollision = CreateDefaultSubObject<U2DCollision>();
	FireballCollision->SetComponentLocation({ 0.0f ,0.0f });
	FireballCollision->SetComponentScale({ 104, 67 });
	FireballCollision->SetCollisionGroup(ECollisionGroup::MONSTERSKILL);
	FireballCollision->SetCollisionType(ECollisionType::Rect);
}

void AFireBall::SetPlayerSkillCollision()
{
	FireballCollision = CreateDefaultSubObject<U2DCollision>();
	FireballCollision->SetComponentLocation({ 0.0f ,0.0f });
	FireballCollision->SetComponentScale({ 104, 67 });
	FireballCollision->SetCollisionGroup(ECollisionGroup::PLAYERSKILL);
	FireballCollision->SetCollisionType(ECollisionType::Rect);
}

void AFireBall::ChangeAnimation(std::string _AnimName)
{
	FireballRender->ChangeAnimation(_AnimName + AnimDir);
}

void AFireBall::StopSound()
{
	if (true == BGMPlayer.IsPlaying())
	{
		BGMPlayer.Stop();
	}
}

void AFireBall::BeginPlay()
{
	Super::BeginPlay();
}

void AFireBall::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	CurTime += _DeltaTime;


	AActor* PlayerAct = FireballCollision->CollisionOnce(ECollisionGroup::PLAYERBODY);
	if (nullptr != PlayerAct && (static_cast<int>(ECollisionGroup::MONSTERSKILL) == FireballCollision->GetGroup()))
	{
		APlayer* Player = dynamic_cast<APlayer*>(PlayerAct);
		if (APlayer::PlayerHp > 0 && true == Player->GetCanHurt())
		{
			Player->SetCanHurt(false);
			APlayer::PlayerHp--;
		}

		(true == Player->GetIsFull()) ? Player->ChangeAnimation("HurtFull") : Player->ChangeAnimation("Hurt");

		if (APlayer::PlayerAbility == EAbilityType::NORMAL)
		{
			BGMPlayer = UEngineSound::Play("HurtNormal.wav");
		}
		else
		{
			BGMPlayer = UEngineSound::Play("HurtAbility.wav");
			APlayer::PlayerAbility = EAbilityType::NORMAL;
		}

		Player->SetCurState(EPlayerState::HURT);
		return;
	}

	AActor* MonsterAct = FireballCollision->CollisionOnce(ECollisionGroup::MONSTERBODY);
	if (nullptr != MonsterAct && (static_cast<int>(ECollisionGroup::PLAYERSKILL) == FireballCollision->GetGroup()))
	{
		APlayer::Score += 700;
		AMonster* Monster = dynamic_cast<AMonster*>(MonsterAct);
		Monster->SetMonsterState(EMonsterState::DIE);
		return;
	}

	if (CurTime >= 3.2f)
	{
		if (true == BGMPlayer.IsPlaying())
		{
			BGMPlayer.Stop();
		}
		SetActive(false);
	}
}

