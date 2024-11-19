#pragma once
#include <EngineCore/Actor.h>

enum class SkillType
{
	NORMAL,
	FIRE,
	CUTTER,
	SPARK
};

enum class MonsterState
{
	PAUSE,
	CHASE,
	ATTACK,
	DIED
};

class AMonster : public AActor
{
public:
	// constrcuter destructer
	AMonster();
	~AMonster();

	// delete Function
	AMonster(const AMonster& _Other) = delete;
	AMonster(AMonster&& _Other) noexcept = delete;
	AMonster& operator=(const AMonster& _Other) = delete;
	AMonster& operator=(AMonster&& _Other) noexcept = delete;

	virtual void SetMonster();
	virtual void SetMonsterAnimation();
	virtual void SetDestory();
	void GetColImage(std::string_view _ColImageName);

	MonsterState GetMonsterState() const
	{
		return CurMonsterState;
	}

	void SetMonsterState(MonsterState _State)
	{
		CurMonsterState = _State;
	}

	void Pause(float _DeltaTime);
	void Chase(float _DeltaTime);
	void Attack(float _DeltaTime);

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	class USpriteRenderer* SpriteRenderer;
	class UEngineWinImage* ColImage = nullptr;
	class U2DCollision* CollisionComponent;

	void Gravity(float _DeltaTime);
	void Move(FVector2D _NextPos);
	MonsterState CurMonsterState = MonsterState::PAUSE;

};