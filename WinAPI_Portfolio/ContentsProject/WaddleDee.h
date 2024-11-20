#pragma once
#include <EngineCore/Actor.h>

#include "ContentsEnum.h"

enum class MonsterState
{
	PAUSE,
	MOVE,
	ATTACK,
	INHALE,
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

	void GetBackImage(std::string_view _ColImageName);
	void SetDestory();

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	class USpriteRenderer* SpriteRenderer;
	class UEngineWinImage* ColImage = nullptr;
	class U2DCollision* CollisionComponent;

	float Speed = 30.0f;
	std::string AnimDir = "_Left";

	FVector2D GravityForce = FVector2D::ZERO;

	MonsterState CurState = MonsterState::PAUSE;

	// 몬스터 초기 설정
	void SetMonster();
	void SetMonsterAnimation();

	// 픽셀 충돌
	bool BottomPointCheck(UColor _Color);
	bool LeftPointCheck(UColor _Color);
	bool RightPointCheck(UColor _Color);

	// FSM 관련
	void MonsterFSM(float _DeltaTime);

	void Gravity(float _DeltaTime);
	void Move(float _DeltaTime);
	void Inhale(float _DeltaTime);
	void Died(float _DeltaTime);
};

