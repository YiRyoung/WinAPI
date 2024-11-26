#pragma once
#include <EngineCore/Actor.h>

#include <EngineCore/2DCollision.h>

#include "ContentsEnum.h"

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

	void GetColImage(std::string _ColImageName);

	EMonsterState GetMonsterState() const
	{
		return CurState;
	}
	void SetMonsterState(EMonsterState _State);
	void SetDistance(FVector2D _PlayerLocation);

	virtual void Pause(float _DeltaTime);
	virtual void Chase(float _DeltaTime);
	virtual void AttackStart(float _DeltaTime);
	virtual void Attack(float _DeltaTime);
	virtual void AttackEnd(float _DeltaTime);
	virtual void Inhale(float _DeltaTime);
	virtual void Die(float _DeltaTime);

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void SetMonster(std::string _SpriteName, FVector2D _SpriteScale, EAbilityType _AbilityType);
	void SetCollision(FVector2D _CollisionScale);

	void ChangeMonsterAnim(std::string _AnimName);
	bool MonsterAnimFinish();

	void MonsterFSM(float _DeltaTime);
	bool PixelPointCheck(ECheckDir _Dir, UColor _Color);
	void Gravity(float _DeltaTime);

	class UEngineWinImage* ColImage = nullptr;
	class USpriteRenderer* SpriteRenderer = nullptr;
	class U2DCollision* CollisionComponent = nullptr;

	float PlayerDistance = 0.0f;
	std::string AnimDir = "_Left";
private:
	float Speed = 45.0f;

	FVector2D MonsterScale = FVector2D::ZERO;
	FVector2D GravityForce = FVector2D::ZERO;
	EMonsterState CurState = EMonsterState::CHASE;
	EAbilityType MonsterAbility = EAbilityType::MAX;
};