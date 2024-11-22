#pragma once
#include <EngineCore/Actor.h>

#include <EngineCore/2DCollision.h>

#include "ContentsEnum.h"

enum class MonsterState
{
	PAUSE,
	CHASE,
	ATTACKSTART,
	ATTACK,
	ATTACKEND,
	HURT,
	INHALE,
	DIED
};

class AMonster : public AActor
{
public:
	// constrcuter destructer
	AMonster();
	virtual ~AMonster();

	// delete Function
	AMonster(const AMonster& _Other) = delete;
	AMonster(AMonster&& _Other) noexcept = delete;
	AMonster& operator=(const AMonster& _Other) = delete;
	AMonster& operator=(AMonster&& _Other) noexcept = delete;

	MonsterState GetMonsterState() const
	{
		return CurState;
	}
	void SetMonsterState(MonsterState _State)
	{
		CurState = _State;
	}
	void SetKirbyDir(std::string _Dir)
	{
		KirbyDir = _Dir;
	}
	void GetColImage(std::string _ColImageName);
	void SetAnimDir(bool _IsLeft)
	{
		AnimDir = _IsLeft;
	}
	void SetAnimDir(FVector2D _PlayerLocation);
	void SetPos(FVector2D _PlayerLocation);

	virtual void Pause(float _DeltaTime);
	virtual void Chase(float _DeltaTime);
	virtual void AttackStart(float _DeltaTime);
	virtual void Attack(float _DeltaTime);
	virtual void AttackEnd(float _DeltaTime);
	virtual void Hurt(float _DeltaTime);
	virtual void Inhale(float _DeltaTime);
	virtual void Died(float _DeltaTime);

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void SetMonster(std::string _SpriteName, FVector2D _SpriteScale);
	void SetCollision(FVector2D _CollisionScale);
	void SetLeftSkillCollision(FVector2D _CollisionLocation, FVector2D _CollisionScale, ECollisionType _CollisionType);
	void SetRightSkillCollision(FVector2D _CollisionLocation, FVector2D _CollisionScale, ECollisionType _CollisionType);
	void SetLeftSkillSwitch(bool _IsOn)
	{
		LeftSkillCollision->SetActive(_IsOn);
	}
	void SetRightSkillSwitch(bool _IsOn)
	{
		RightSkillCollision->SetActive(_IsOn);
	}


	FVector2D GetMonsterScale() const;
	void ChangeMonsterAnim(std::string _AnimName);
	bool MonsterAnimFinish();

	bool PixelLineCheck(CheckDir _Dir, UColor _Color);

	void CollisionEnter(AActor* _ColActor);
	void MonsterFSM(float _DeltaTime);
	void Gravity(float _DeltaTime);

	std::string GetAnimDir() const
	{
		return AnimDir;
	}

	class USpriteRenderer* SpriteRenderer = nullptr;
	class USpriteRenderer* SkillRenderer = nullptr;
	class U2DCollision* CollisionComponent = nullptr;
	class U2DCollision* LeftSkillCollision = nullptr;
	class U2DCollision* RightSkillCollision = nullptr;
	class UEngineWinImage* ColImage = nullptr;

	FVector2D Pos = FVector2D::ZERO;

private:
	float Speed = 45.0f;
	std::string AnimDir = "_Left";
	std::string KirbyDir = "_Right";
	
	FVector2D GravityForce = FVector2D::ZERO;
	MonsterState CurState = MonsterState::CHASE;
};