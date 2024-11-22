#pragma once
#include <EngineCore/GameMode.h>

class AStage101GameMode : public AGameMode
{
public:
	// constrcuter destructer
	AStage101GameMode();
	~AStage101GameMode();

	// delete Function
	AStage101GameMode(const AStage101GameMode& _Other) = delete;
	AStage101GameMode(AStage101GameMode&& _Other) noexcept = delete;
	AStage101GameMode& operator=(const AStage101GameMode& _Other) = delete;
	AStage101GameMode& operator=(AStage101GameMode&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;

	void Tick(float _DeltaTime) override;

private:
	class APlayer* NewPlayer = nullptr;
	class AStage* NewActor = nullptr;
	class AHUI* NewUI = nullptr;
	class AWindBullet* NewBullet = nullptr;
	class ASpitBullet* NewStar = nullptr;
	class AMonster* NewHotHead = nullptr;
	class AMonster* NewWaddleDoo = nullptr;
	class AWaddleBeam* NewWaddleBeam = nullptr;

	std::string PlayerDir = " ";
};

