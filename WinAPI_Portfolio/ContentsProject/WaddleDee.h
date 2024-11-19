#pragma once
#include <EngineCore/Actor.h>

enum class MonsterState
{
	PAUSE,
	CHASE,
	ATTACK,
	DIED
};

class AWaddleDee : public AActor
{
public:
	// constrcuter destructer
	AWaddleDee();
	~AWaddleDee();

	// delete Function
	AWaddleDee(const AWaddleDee& _Other) = delete;
	AWaddleDee(AWaddleDee&& _Other) noexcept = delete;
	AWaddleDee& operator=(const AWaddleDee& _Other) = delete;
	AWaddleDee& operator=(AWaddleDee&& _Other) noexcept = delete;

	void SetDestory();

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	class USpriteRenderer* SpriteRenderer;
	class UEngineWinImage* ColImage = nullptr;
	class U2DCollision* CollisionComponent;
	MonsterState CurState = MonsterState::PAUSE;

	void SetMonster();
	void SetMonsterAnimation();

	void MonsterFSM(float _DeltaTime);

	void Died(float _DeltaTime);
};

