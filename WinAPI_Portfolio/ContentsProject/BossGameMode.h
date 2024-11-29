#pragma once
#include <EngineCore/GameMode.h>
#include <EnginePlatform/EngineSound.h>

class ABossGameMode : public AGameMode
{
public:
	// constrcuter destructer
	ABossGameMode();
	~ABossGameMode();

	// delete Function
	ABossGameMode(const ABossGameMode& _Other) = delete;
	ABossGameMode(ABossGameMode&& _Other) noexcept = delete;
	ABossGameMode& operator=(const ABossGameMode& _Other) = delete;
	ABossGameMode& operator=(ABossGameMode&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;

	void Tick(float _DeltaTime) override;

private:
	class AStage* NewActor = nullptr;
	class AHUI* NewUI = nullptr;
	class AScore* NewPlayerLife = nullptr;
	class AScore* NewPlayerScore = nullptr;
};

