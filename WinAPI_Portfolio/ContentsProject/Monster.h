#pragma once
#include <EngineCore/Actor.h>

enum class MonsterState
{
	PAUSE,
	CHASE,
	ATTACK,
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


protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void GetColImage(std::string _ColImageName);
	void SetMonster(std::string _SpriteName, FVector2D _SpriteScale);
	void SetCollision(FVector2D _CollisionScale);
	void SetSkillCollision(FVector2D _CollisionScale, ECollisionType _CollisionType);

	void SetAnimDir();
	void ChangeMonsterAnim(std::string _AnimName);

	bool BottomPixelCheck(UColor _Color);
	bool LeftPixelCheck(UColor _Color);
	bool RightPixelCheck(UColor _Color);

	void MonsterFSM(float _DeltaTime);
	MonsterState GetMonsterState() const
	{
		return CurState;
	}
	void SetMonsterState(MonsterState _State)
	{
		CurState = _State;
	}
	void Gravity(float _DeltaTime);

	virtual void Pause(float _DeltaTime) = 0;
	virtual void Chase(float _DeltaTime) = 0;
	virtual void Attack(float _DeltaTime) = 0;
	virtual void Inhale(float _DeltaTime) = 0;
	virtual void Died(float _DeltaTime) = 0;

private:
	class USpriteRenderer* SpriteRenderer = nullptr;
	class USpriteRenderer* SkillRenderer = nullptr;
	class U2DCollision* CollisionComponent = nullptr;
	class U2DCollision* SkillCollision = nullptr;
	class UEngineWinImage* ColImage = nullptr;

	std::string AnimDir = "_Left";
	MonsterState CurState = MonsterState::PAUSE;
};

