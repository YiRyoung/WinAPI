#pragma once
#include <EngineCore/Actor.h>

class AHUI : public AActor
{
public:
	// constrcuter destructer
	AHUI();
	~AHUI();

	// delete Function
	AHUI(const AHUI& _Other) = delete;
	AHUI(AHUI&& _Other) noexcept = delete;
	AHUI& operator=(const AHUI& _Other) = delete;
	AHUI& operator=(AHUI&& _Other) noexcept = delete;

protected:

private:

};

