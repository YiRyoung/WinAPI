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

	virtual void Pause(float _DeltaTime);
	virtual void Chase(float _DeltaTime);
	virtual void Attack(float _DeltaTime);
	virtual void Inhale(float _DeltaTime);
	virtual void Died(float _DeltaTime);

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void GetColImage(std::string _ColImageName);
	void SetMonster(std::string _SpriteName, FVector2D _SpriteScale);
	void SetCollision(FVector2D _CollisionScale);
	void SetSkillCollision(FVector2D _CollisionScale, ECollisionType _CollisionType);

	FVector2D GetMonsterScale() const;
	void ChangeMonsterAnim(std::string _AnimName);

	bool BottomPixelCheck(UColor _Color);
	bool LeftPixelCheck(UColor _Color);
	bool RightPixelCheck(UColor _Color);
	void CollisionEnter(AActor* _ColActor);


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

	class USpriteRenderer* SpriteRenderer = nullptr;
	class USpriteRenderer* SkillRenderer = nullptr;
	class U2DCollision* CollisionComponent = nullptr;
	class U2DCollision* SkillCollision = nullptr;
	class UEngineWinImage* ColImage = nullptr;

private:
	std::string AnimDir = "_Left";
	MonsterState CurState = MonsterState::PAUSE;
};

