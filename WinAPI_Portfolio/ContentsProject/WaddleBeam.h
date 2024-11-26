#pragma once
#include <EngineCore/Actor.h>

enum class Phase
{
	Phase01,
	Phase02,
	Phase03,
	Phase04,
	Phase05,
	Phase06
};

class AWaddleBeam : public AActor
{
public:
	// constrcuter destructer
	AWaddleBeam();
	~AWaddleBeam();

	// delete Function
	AWaddleBeam(const AWaddleBeam& _Other) = delete;
	AWaddleBeam(AWaddleBeam&& _Other) noexcept = delete;
	AWaddleBeam& operator=(const AWaddleBeam& _Other) = delete;
	AWaddleBeam& operator=(AWaddleBeam&& _Other) noexcept = delete;


	void SetDir(const FVector2D& _Dir);
	void SetEffect(FVector2D _Dir);

	void Phase01(float _DeltaTime);
	void Phase02(float _DeltaTime);
	void Phase03(float _DeltaTime);
	void Phase04(float _DeltaTime);
	void Phase05(float _DeltaTime);
	void Phase06(float _DeltaTime);

	void SetMosnterSkillCollision();
	void SetPlayerSkillCollision();
protected:
	void Tick(float _DeltaTime);
private:
	class USpriteRenderer* WaddleBeamRender[3] = { nullptr, };
	class U2DCollision* WaddleBeamCollision[3] = { nullptr, };

	std::string AnimDir = "_Left";
	FVector2D Dir = FVector2D::ZERO;
	FVector2D ArrDir[static_cast<int>(Phase::Phase06)];
	float InterTime = 0.2f;
	float Time = InterTime;
	Phase PhaseValue = Phase::Phase01;
};

