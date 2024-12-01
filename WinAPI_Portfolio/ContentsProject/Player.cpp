#include "PreCompile.h"
#include "Player.h"

#include <EnginePlatform/EngineInput.h>

#include <EngineCore/EngineAPICore.h>
#include <EngineCore/EngineCoreDebug.h>
#include <EngineCore/ImageManager.h>
#include <EngineCore/2DCollision.h>

#include "Monster.h"
#include "PlayerState.h"

#include "WindBullet.h"
#include "SpitBullet.h"
#include "WaddleBeam.h"
#include "CutterSlider.h"
#include "FireBall.h"

#include "ContentsEnum.h"

int APlayer::PlayerLife = 4;
int APlayer::PlayerHp = 6;
int APlayer::Score = 12345;
EAbilityType APlayer::PlayerAbility = EAbilityType::NORMAL;

APlayer::APlayer()
{
	SetPlayer();
	SetAnimation();
	SetAdjustSize();
	SetPlayerCollision();
	State = new PlayerState(this);
	DebugOn();
}

APlayer::~APlayer()
{
	delete State;
}

void APlayer::BeginPlay()
{
	Super::BeginPlay();
	PlayerRenderer->SetPivotType(PivotType::Bot);
	GetWorld()->SetCameraToMainPawn(false);
}

void APlayer::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	// Debug Log
	UEngineDebug::CoreOutPutString("FPS : " + std::to_string(1.0f / _DeltaTime));
	UEngineDebug::CoreOutPutString("PlayerPos : " + GetActorLocation().ToString());
	UEngineDebug::CoreOutPutString("PlayerState : " + std::to_string(static_cast<int>(CurState)));
	UEngineDebug::CoreOutPutString("PlayerAbility : " + std::to_string(static_cast<int>(PlayerAbility)));

	SetAnimDir();
	SetAdjustSize();
	ChangePlayerCollision();
	FSM(_DeltaTime);
	CameraMove();

	(true == CanHurt) ? PlayerCollision->SetActive(true) : PlayerCollision->SetActive(false);
}

void APlayer::SpawnWind()
{
	AWindBullet* NewWind = GetWorld()->SpawnActor<AWindBullet>();
	NewWind->SetDir(AnimDir);
	NewWind->SetActorLocation(GetActorLocation() + NewWind->SpawnPos);
}

void APlayer::SpawnSpit()
{
	ASpitBullet* NewSpit = GetWorld()->SpawnActor<ASpitBullet>();
	NewSpit->SetDir(AnimDir);
	NewSpit->SetActorLocation(GetActorLocation() + NewSpit->SpawnPos);
}

void APlayer::SpawnBeam()
{
	AWaddleBeam* NewBeam = GetWorld()->SpawnActor<AWaddleBeam>();
	NewBeam->SetPlayerSkillCollision();
	NewBeam->SetActorLocation(GetActorLocation());
	FVector2D Vector = ("_Left" == AnimDir) ? FVector2D::LEFT : FVector2D::RIGHT;
	NewBeam->SetDir(Vector);
}

void APlayer::SpawnSlide()
{
	ACutterSlider* NewSlide = GetWorld()->SpawnActor<ACutterSlider>();
	NewSlide->SetPlayerSkillCollision();
	NewSlide->SetActorLocation(GetActorLocation() + FVector2D ({0.0f, -25.0f}));
	FVector2D Vector = ("_Left" == AnimDir) ? FVector2D::LEFT : FVector2D::RIGHT;
	NewSlide->SetDir(Vector);
}

void APlayer::SpawnFire()
{
	NewFireBall = GetWorld()->SpawnActor<AFireBall>();
	FVector2D Vector = ("_Left" == AnimDir) ? FVector2D::LEFT : FVector2D::RIGHT;
	NewFireBall->SetActorLocation(GetActorLocation() + Vector * 80 + FVector2D({ 0.0f, -25.0f }));
	NewFireBall->SetPlayerSkillCollision();
	NewFireBall->SetDir(Vector);
	NewFireBall->ChangeAnimation("FireBall");
}

void APlayer::CollisionEnter(AActor* _ColActor)
{
	AMonster* Monster = dynamic_cast<AMonster*>(_ColActor);

	if (CurState == EPlayerState::INHALESTART)
	{
		IsFull = true;
		_ColActor->SetActive(false);
	}
	else if (CurState == EPlayerState::SLIDE)
	{
		SliderCollisionSwitch(false);
		if (true == BGMPlayer.IsPlaying())
		{
			BGMPlayer.Stop();
		}
		BGMPlayer = UEngineSound::Play("MonsterDie.wav");
		dynamic_cast<AMonster*>(_ColActor)->SetMonsterState(EMonsterState::DIE);
		return;
	}
	else
	{
		if (APlayer::PlayerHp > 0 && CanHurt == true)
		{
			APlayer::PlayerHp--;
			CanHurt = false;
		}

		if (true == BGMPlayer.IsPlaying())
		{
			BGMPlayer.Stop();
		}
		BGMPlayer = UEngineSound::Play("MonsterDie.wav");

		dynamic_cast<AMonster*>(_ColActor)->SetMonsterState(EMonsterState::DIE);
		SetCurState(EPlayerState::HURTSTART);
		return;
	}
}

void APlayer::CollisionStay(AActor* _ColActor)
{
	if (CurState == EPlayerState::INHALE)
	{
		dynamic_cast<AMonster*>(_ColActor)->SetMonsterState(EMonsterState::INHALE);
	}
}

void APlayer::SetPlayer()
{
	PlayerRenderer = CreateDefaultSubObject<USpriteRenderer>();
	PlayerRenderer->SetSprite("Kirby_Normal_Left.png");
	PlayerRenderer->SetComponentScale({ 100, 100 });
	PlayerScale = PlayerRenderer->GetTransform().Scale;
	CurState = EPlayerState::IDLE;
}

void APlayer::SetAnimation()
{
	// IDLE_UNFULLED
	PlayerRenderer->CreateAnimation("Idle_Left", "Kirby_Normal_Left.png", 0, 1, 1.0f);
	PlayerRenderer->CreateAnimation("Idle_Right", "Kirby_Normal_Right.png", 0, 1, 1.0f);

	// IDLE_FULLED
	// PlayerRenderer->SetSprite("Kirby_Normal_Left.png", 30);
	// PlayerRenderer->SetSprite("Kirby_Normal_Right.png", 30);

	// WALK_UNFULLED
	PlayerRenderer->CreateAnimation("Walk_Left", "Kirby_Normal_Left.png", 2, 5, 0.1f);
	PlayerRenderer->CreateAnimation("Walk_Right", "Kirby_Normal_Right.png", 2, 5, 0.1f);

	// WALK_FULLED
	PlayerRenderer->CreateAnimation("WalkFull_Left", "Kirby_Normal_Left.png", 31, 33, 0.1f);
	PlayerRenderer->CreateAnimation("WalkFull_Right", "Kirby_Normal_Right.png", 31, 33, 0.1f);

	// DASH_UNFULLED
	PlayerRenderer->CreateAnimation("Dash_Left", "Kirby_Normal_Left.png", 2, 5, 0.07f);
	PlayerRenderer->CreateAnimation("Dash_Right", "Kirby_Normal_Right.png", 2, 5, 0.07f);

	// DASH_FULLED
	PlayerRenderer->CreateAnimation("DashFull_Left", "Kirby_Normal_Left.png", 31, 33, 0.1f);
	PlayerRenderer->CreateAnimation("DashFull_Right", "Kirby_Normal_Right.png", 31, 33, 0.1f);

	// JUMP_UNFULLED
	// PlayerRenderer->SetSprite("Kirby_Normal_Left.png", 11);
	// PlayerRenderer->SetSprite("Kirby_Normal_Right.png", 11);
	PlayerRenderer->CreateAnimation("Jump_Left", "Kirby_Normal_Left.png", 12, 15, 0.07f, false);
	PlayerRenderer->CreateAnimation("Jump_Right", "Kirby_Normal_Right.png", 12, 15, 0.07f, false);

	// JUMP_FULLED
	// PlayerRenderer->SetSprite("Kirby_Normal_Left.png", 31);
	// PlayerRenderer->SetSprite("Kirby_Normal_Right.png", 31);
	PlayerRenderer->CreateAnimation("JumpFull_Left", "Kirby_Normal_Left.png", { 31, 32, 31 }, 0.1f, false);
	PlayerRenderer->CreateAnimation("JumpFull_Right", "Kirby_Normal_Right.png", { 31, 32, 31 }, 0.1f, false);

	// FLY_UNFULLED
	PlayerRenderer->CreateAnimation("FlyStart_Left", "Kirby_Normal_Left.png", 17, 21, 0.04f, false);
	PlayerRenderer->CreateAnimation("FlyStart_Right", "Kirby_Normal_Right.png", 17, 21, 0.04f, false);
	PlayerRenderer->CreateAnimation("Flying_Left", "Kirby_Normal_Left.png", 21, 22, 0.7f);
	PlayerRenderer->CreateAnimation("Flying_Right", "Kirby_Normal_Right.png", 21, 22, 0.7f);
	PlayerRenderer->CreateAnimation("FlyEnd_Left", "Kirby_Normal_Left.png", 23, 26, 0.05f, false);
	PlayerRenderer->CreateAnimation("FlyEnd_Right", "Kirby_Normal_Right.png", 23, 26, 0.05f, false);

	// FALL_UNFULLED
	// PlayerRenderer->SetSprite("Kirby_Normal_Left.png", 15);
	// PlayerRenderer->SetSprite("Kirby_Normal_Right.png", 15);

	// FALL_FULLED
	// PlayerRenderer->SetSprite("Kirby_Normal_Left.png", 31);
	// PlayerRenderer->SetSprite("Kirby_Normal_Right.png", 31);

	// BEND
	// PlayerRenderer->SetSprite("Kirby_Normal_Left.png", 47);
	// PlayerRenderer->SetSprite("Kirby_Normal_Right.png", 47);

	// Slide
	// PlayerRenderer->SetSprite("Kirby_Normal_Left.png", 7);
	// PlayerRenderer->SetSprite("Kirby_Normal_Right.png", 7);

	// Climb
	PlayerRenderer->CreateAnimation("ClimbUp_Left", "Kirby_Normal_Left.png", { 51, 52, 53, 52 }, 0.07f);
	PlayerRenderer->CreateAnimation("ClimbUp_Right", "Kirby_Normal_Right.png", { 51, 52, 53, 52 }, 0.07f);
	// PlayerRenderer->SetSprite("Kirby_Normal_Left.png", 52);

	// Eat
	PlayerRenderer->CreateAnimation("Eat_Left", "Kirby_Normal_Left.png", 45, 47, 0.1f, false);
	PlayerRenderer->CreateAnimation("Eat_Right", "Kirby_Normal_Right.png", 45, 47, 0.1f, false);

	// Inhale
	PlayerRenderer->CreateAnimation("InhaleStart_Left", "Kirby_Normal_Left.png", 28, 29, 0.07f, false);
	PlayerRenderer->CreateAnimation("InhaleStart_Right", "Kirby_Normal_Right.png", 28, 29, 0.07f, false);
	PlayerRenderer->CreateAnimation("InhaleEnd_Left", "Kirby_Normal_Left.png", 24, 26, 0.05f, false);
	PlayerRenderer->CreateAnimation("InhaleEnd_Right", "Kirby_Normal_Right.png", 24, 26, 0.05f, false);

	// Spit
	PlayerRenderer->CreateAnimation("Spit_Left", "Kirby_Normal_Left.png", 34, 37, 0.1f, false);
	PlayerRenderer->CreateAnimation("Spit_Right", "Kirby_Normal_Right.png", 34, 37, 0.1f, false);

	// HURT_UNFULLED
	PlayerRenderer->CreateAnimation("Hurt_Left", "Kirby_Normal_Left.png", 61, 62, 0.05f);
	PlayerRenderer->CreateAnimation("Hurt_Right", "Kirby_Normal_Right.png", 61, 62, 0.05f);

	// HURT_FULLED
	PlayerRenderer->CreateAnimation("HurtFull_Left", "Kirby_Normal_Left.png", 75, 76, 0.05f);
	PlayerRenderer->CreateAnimation("HurtFull_Right", "Kirby_Normal_Right.png", 75, 76, 0.05f);

	// Beam
	PlayerRenderer->CreateAnimation("Beam_Left", "Kirby_Ability_Left.png", 0, 1, 0.05f);
	PlayerRenderer->CreateAnimation("Beam_Right", "Kirby_Ability_Right.png", 0, 1, 0.05f);

	// Cutter
	PlayerRenderer->CreateAnimation("Cutter_Left", "Kirby_Ability_Left.png", 17, 18, 0.1f, false);
	PlayerRenderer->CreateAnimation("Cutter_Right", "Kirby_Ability_Right.png", 17, 18, 0.1f, false);

	// Fire
	PlayerRenderer->CreateAnimation("Fire_Left", "Kirby_Ability_Left.png", 9, 10, 0.05f);
	PlayerRenderer->CreateAnimation("Fire_Right", "Kirby_Ability_Right.png", 9, 10, 0.05f);

	// Enter NectStage
	PlayerRenderer->CreateAnimation("Fire_Left", "Kirby_Ability_Left.png", 9, 10, 0.05f);
	PlayerRenderer->CreateAnimation("Fire_Right", "Kirby_Ability_Right.png", 9, 10, 0.05f);

	PlayerRenderer->ChangeAnimation("Idle_Right");
}

void APlayer::SetAnimDir()
{
	if (EPlayerState::SLIDE != CurState && EPlayerState::SKILL != CurState
		&& EPlayerState::HURT != CurState)
	{
		if (UEngineInput::GetInst().IsPress(VK_LEFT))
		{
			AnimDir = "_Left";
		}
		else if (UEngineInput::GetInst().IsPress(VK_RIGHT))
		{
			AnimDir = "_Right";
		}
	}
}

void APlayer::SetPlayerCollision()
{
	// PlayerCollision
	PlayerCollision = CreateDefaultSubObject<U2DCollision>();
	PlayerCollision->SetComponentLocation({ 0.0f , -(PlayerScale.Y * AdjustValue.X)});
	PlayerCollision->SetComponentScale({ AdjustScale });
	PlayerCollision->SetCollisionGroup(ECollisionGroup::PLAYERBODY);
	PlayerCollision->SetCollisionType(ECollisionType::CirCle);
	
	// Slide Left Collision
	SlideLeftCollision = CreateDefaultSubObject<U2DCollision>();
	SlideLeftCollision->SetComponentLocation({ PlayerScale.X * -0.5f, PlayerScale.Y * -0.2f});
	SlideLeftCollision->SetComponentScale({ 20, 50 });
	SlideLeftCollision->SetCollisionGroup(ECollisionGroup::PLAYERSKILL);
	SlideLeftCollision->SetCollisionType(ECollisionType::Rect);
	SlideLeftCollision->SetActive(false);

	// Slide Right Collison
	SlideRightCollision = CreateDefaultSubObject<U2DCollision>();
	SlideRightCollision->SetComponentLocation({ PlayerScale.X * 0.5f, PlayerScale.Y * -0.2f });
	SlideRightCollision->SetComponentScale({ 20, 50 });
	SlideRightCollision->SetCollisionGroup(ECollisionGroup::PLAYERSKILL);
	SlideRightCollision->SetCollisionType(ECollisionType::Rect);
	SlideRightCollision->SetActive(false);

	// InhaleBox Left Collision
	InhaleBoxLeftCollision = CreateDefaultSubObject<U2DCollision>();
	InhaleBoxLeftCollision->SetComponentLocation({ PlayerScale.X * -1.0f , PlayerScale.Y * -0.35f});
	InhaleBoxLeftCollision->SetComponentScale({ 160, 60 });
	InhaleBoxLeftCollision->SetCollisionGroup(ECollisionGroup::INHALEBOX);
	InhaleBoxLeftCollision->SetCollisionType(ECollisionType::Rect);
	InhaleBoxLeftCollision->SetActive(false);
	 
	// InhaleBox Right Collision
	InhaleBoxRightCollision = CreateDefaultSubObject<U2DCollision>();
	InhaleBoxRightCollision->SetComponentLocation({ PlayerScale.X * 1.0f , PlayerScale.Y * -0.35f});
	InhaleBoxRightCollision->SetComponentScale({ 160, 60 });
	InhaleBoxRightCollision->SetCollisionGroup(ECollisionGroup::INHALEBOX);
	InhaleBoxRightCollision->SetCollisionType(ECollisionType::Rect);
	InhaleBoxRightCollision->SetActive(false);

	// Collision Link
	GetWorld()->CollisionGroupLink(ECollisionGroup::PLAYERBODY, ECollisionGroup::MONSTERBODY);
	GetWorld()->CollisionGroupLink(ECollisionGroup::PLAYERSKILL, ECollisionGroup::MONSTERBODY);
	GetWorld()->CollisionGroupLink(ECollisionGroup::INHALEBOX, ECollisionGroup::MONSTERBODY);

	PlayerCollision->SetCollisionEnter(std::bind(&APlayer::CollisionEnter, this, std::placeholders::_1));
	SlideLeftCollision->SetCollisionEnter(std::bind(&APlayer::CollisionEnter, this, std::placeholders::_1));
	SlideRightCollision->SetCollisionEnter(std::bind(&APlayer::CollisionEnter, this, std::placeholders::_1));
	InhaleBoxLeftCollision->SetCollisionStay(std::bind(&APlayer::CollisionStay, this, std::placeholders::_1));
	InhaleBoxRightCollision->SetCollisionStay(std::bind(&APlayer::CollisionStay, this, std::placeholders::_1));
}

void APlayer::ChangePlayerCollision()
{
	if (CurState == EPlayerState::BEND)
	{
		PlayerCollision->SetComponentLocation({ 0.0f , PlayerScale.Y * -0.12f});
		PlayerCollision->SetComponentScale({ 50, 30 });
		PlayerCollision->SetCollisionType(ECollisionType::Rect);
	}
	else
	{
		PlayerCollision->SetComponentLocation({ 0.0f , -(PlayerScale.Y * AdjustValue.X) });
		PlayerCollision->SetComponentScale({ AdjustScale });
		PlayerCollision->SetCollisionType(ECollisionType::CirCle);
	}
}

void APlayer::CameraMove()
{
	// Camera Move
	FVector2D Size = UEngineAPICore::GetCore()->GetMainWindow().GetWindowSize();
	FVector2D CurPos = GetActorLocation() - Size.Half();
	FVector2D CurPosRB = CurPos + Size;
	FVector2D BackImageSize = BackgroundImage->GetImageScale();
	BackImageSize += FVector2D(0, 198);

	// Camera Limit
	if (0.0f >= CurPos.X)
	{
		CurPos = { 0.0f, CurPos.Y };
	}

	if (0.0f >= CurPos.Y)
	{
		CurPos = { CurPos.X, 0.0f };
	}

	if (BackImageSize.X <= CurPosRB.X)
	{
		CurPosRB = { BackImageSize.X, CurPosRB.Y };
		FVector2D TempPos = CurPosRB - Size;
		CurPos = { TempPos.X ,CurPos.Y };
	}

	if (BackImageSize.Y <= CurPosRB.Y)
	{
		CurPosRB = { CurPosRB.X, BackImageSize.Y };
		FVector2D TempPos = CurPosRB - Size;
		CurPos = { CurPos.X, TempPos.Y };
	}

	GetWorld()->SetCameraPos(CurPos);
}

void APlayer::FSM(float _DeltaTime)
{
	switch (CurState)
	{
	case EPlayerState::IDLE:
		State->IdleStart(_DeltaTime);
		break;
	case EPlayerState::WALK:
		State->WalkStart(_DeltaTime);
		break;
	case EPlayerState::DASH:
		State->DashStart(_DeltaTime);
		break;
	case EPlayerState::JUMP:
		State->JumpStart(_DeltaTime);
		break;
	case EPlayerState::FLYSTART:
		State->FlyStart(_DeltaTime);
		break;
	case EPlayerState::FLY:
		State->Fly(_DeltaTime);
		break;
	case EPlayerState::FLYEND:
		State->FlyEnd(_DeltaTime);
		break;
	case EPlayerState::BEND:
		State->BendStart(_DeltaTime);
		break;
	case EPlayerState::SLIDE:
		State->SlideStart(_DeltaTime);
		break;
	case EPlayerState::CLIMB:
		State->ClimbStart(_DeltaTime);
		break;
	case EPlayerState::FALL:
		State->FallStart(_DeltaTime);
		break;
	case EPlayerState::EAT:
		State->EatStart(_DeltaTime);
		break;
	case EPlayerState::INHALESTART:
		State->InhaleStart(_DeltaTime);
		break;
	case EPlayerState::INHALE:
		State->Inhale(_DeltaTime);
		break;
	case EPlayerState::SPIT:
		State->Spit(_DeltaTime);
		break;
	case EPlayerState::SKILLSTART:
		State->SkillStart(_DeltaTime);
		break;
	case EPlayerState::SKILL:
		State->Skill(_DeltaTime);
		break;
	case EPlayerState::HURTSTART:
		State->HurtStart(_DeltaTime);
		break;
	case EPlayerState::HURT:
		State->Hurt(_DeltaTime);
		break;
	case EPlayerState::DIE:
		//State->DieStart(_DeltaTime);
		break;
	default:
		break;
	}
}

FVector2D APlayer::GetDir() const
{
	if ("_Left" == AnimDir)
	{
		return FVector2D::LEFT;
	}
	else
	{
		return FVector2D::RIGHT;
	}
}

std::string APlayer::GetAnimDir() const
{
	return AnimDir;
}

void APlayer::ChangeAnimation(std::string _Anim)
{
	PlayerRenderer->ChangeAnimation(_Anim + AnimDir);
}

void APlayer::GetBackgroundImage(std::string_view _BackImage, std::string_view _ColImage)
{
	BackgroundImage = UImageManager::GetInst().FindImage(_BackImage);
	ColliderImage = UImageManager::GetInst().FindImage(_ColImage);
}

void APlayer::SetAdjustSize()
{
	if (!IsFull && EPlayerState::FLYSTART != CurState && EPlayerState::FLY != CurState 
		&& EPlayerState::FLYEND != CurState && EPlayerState::INHALE != CurState)
	{
		AdjustValue = { 0.25f, 0.5f };
		AdjustScale = { 50, 50 };
	}
	else
	{
		AdjustValue = { 0.35f, 0.7f };
		AdjustScale = { 68, 68 };
	}
}

void APlayer::DrawDebugPoint(FVector2D _Point)
{
	FVector2D CheckPoint = GetActorLocation() + _Point;
	FTransform PlayerTransform = GetTransform();
	PlayerTransform.Location += _Point - GetWorld()->GetCameraPos();
	PlayerTransform.Scale = { 6, 6 };
	UEngineDebug::CoreDebugRender(PlayerTransform, UEngineDebug::EDebugPosType::Circle);
}

bool APlayer::DownColorCheck(FVector2D _Power, UColor _Color)
{
	FVector2D LeftBottomPoint = FVector2D({ PlayerScale.X * -AdjustValue.X, 0.0f });
	FVector2D RightBottomPoint = FVector2D({ PlayerScale.X * AdjustValue.X, 0.0f });

	DrawDebugPoint(RightBottomPoint);
	DrawDebugPoint(LeftBottomPoint);

	for (float i = 0.0f; i <= (RightBottomPoint.X - LeftBottomPoint.X); i++)
	{
		FVector2D Point = GetActorLocation() + FVector2D{ (RightBottomPoint.X - i), RightBottomPoint.Y };
		FVector2D NextPoint = Point + _Power;
		UColor CheckColor = ColliderImage->GetColor(NextPoint, UColor::MAGENTA);
		if (_Color.operator==(CheckColor))
		{
			return true;
		}
	}
	return false;
}

bool APlayer::PixelPointCheck(ECheckDir _Dir, UColor _Color)
{
	FVector2D CheckPoint = FVector2D::ZERO;

	switch (_Dir)
	{
	case ECheckDir::UP:
		CheckPoint = FVector2D::UP + FVector2D({ 0.0f, PlayerScale.Y * -AdjustValue.Y });
		break;
	case ECheckDir::DOWN:
		CheckPoint = FVector2D::DOWN;
		break;
	case ECheckDir::LEFT:
		CheckPoint = FVector2D::LEFT + FVector2D({ PlayerScale.X * -AdjustValue.X , -(AdjustValue.Y * 0.5f) });
		break;
	case ECheckDir::RIGHT:
		CheckPoint = FVector2D::RIGHT + FVector2D({ PlayerScale.X * AdjustValue.X, -(AdjustValue.Y * 0.5f) });
		break;
	}
	DrawDebugPoint(CheckPoint);

	FVector2D NextPoint = GetActorLocation() + CheckPoint;
	UColor NextPointColor = ColliderImage->GetColor(NextPoint, UColor::MAGENTA);
	if (_Color.operator==(NextPointColor))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool APlayer::PixelLineCheck(ECheckDir _Dir, UColor _Color)
{
	FVector2D LeftUpperPoint = FVector2D({ PlayerScale.X * -AdjustValue.X, PlayerScale.Y * -AdjustValue.Y });
	FVector2D RightUpperPoint = FVector2D({ PlayerScale.X * AdjustValue.X, PlayerScale.Y * -AdjustValue.Y});
	FVector2D LeftBottomPoint = FVector2D({ PlayerScale.X * -AdjustValue.X, 0.0f });
	FVector2D RightBottomPoint = FVector2D({ PlayerScale.X * AdjustValue.X, 0.0f});

	switch (_Dir)
	{
	case ECheckDir::UP:
		DrawDebugPoint(RightUpperPoint);
		DrawDebugPoint(LeftUpperPoint);

		for (float i = 0.0f; i <= (RightUpperPoint.X - LeftUpperPoint.X); i++)
		{
			FVector2D Point = GetActorLocation() + FVector2D{ (RightUpperPoint.X - i), RightUpperPoint.Y };
			FVector2D PointUp = Point + FVector2D::UP;
			UColor CheckColor = ColliderImage->GetColor(PointUp, UColor::MAGENTA);
			if (_Color.operator==(CheckColor))
			{
				return true;
			}
		}
		return false;
		break;
	case ECheckDir::DOWN:
		DrawDebugPoint(RightBottomPoint);
		DrawDebugPoint(LeftBottomPoint);

		for (float i = 0.0f; i <= (RightBottomPoint.X - LeftBottomPoint.X); i++)
		{
			FVector2D Point = GetActorLocation() + FVector2D{ (RightBottomPoint.X - i), RightBottomPoint.Y };
			FVector2D PointDown = Point + FVector2D::DOWN;
			UColor CheckColor = ColliderImage->GetColor(PointDown, UColor::MAGENTA);
			if (_Color.operator==(CheckColor))
			{
				return true;
			}
		}
		return false;
		break;
	case ECheckDir::LEFT:
		DrawDebugPoint(LeftBottomPoint);
		DrawDebugPoint(LeftUpperPoint);

		for (float i = 0; i <= (LeftBottomPoint.Y - LeftUpperPoint.Y); i++)
		{
			FVector2D Point = GetActorLocation() + FVector2D{ LeftUpperPoint.X,(LeftBottomPoint.Y - i) };
			FVector2D PointLeft = Point + FVector2D::LEFT;
			UColor CheckColor = ColliderImage->GetColor(PointLeft, UColor::MAGENTA);
			if (_Color.operator==(CheckColor))
			{
				return true;
			}
		}
		return false;
		break;
	case ECheckDir::RIGHT:
		DrawDebugPoint(RightBottomPoint);
		DrawDebugPoint(RightUpperPoint);

		for (float i = 0; i <= (RightBottomPoint.Y - RightUpperPoint.Y); i++)
		{
			FVector2D Point = GetActorLocation() + FVector2D{ RightUpperPoint.X,(RightBottomPoint.Y - i) };
			FVector2D PointRight = Point + FVector2D::RIGHT;
			UColor CheckColor = ColliderImage->GetColor(PointRight, UColor::MAGENTA);
			if (_Color.operator==(CheckColor))
			{
				return true;
			}
		}
		return false;
		break;
	}
	return false;
}

void APlayer::SliderCollisionSwitch(bool _IsOn)
{
	if ("_Left" == AnimDir)
	{
		SlideLeftCollision->SetActive(_IsOn);
	}
	else if ("_Right" == AnimDir)
	{
		SlideRightCollision->SetActive(_IsOn);
	}
}

void APlayer::SkillBoxCollisionSwitch(bool _IsOn)
{
	if ("_Left" == AnimDir)
	{
		InhaleBoxLeftCollision->SetActive(_IsOn);
	}
	else if ("_Right" == AnimDir)
	{
		InhaleBoxRightCollision->SetActive(_IsOn);
	}
}