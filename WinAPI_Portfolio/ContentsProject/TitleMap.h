#pragma once
#include <EngineCore/Actor.h>

class TitleMap : public AActor
{
public:
	// constrcuter destructer
	TitleMap();
	~TitleMap();

	// delete Function
	TitleMap(const TitleMap& _Other) = delete;
	TitleMap(TitleMap&& _Other) noexcept = delete;
	TitleMap& operator=(const TitleMap& _Other) = delete;
	TitleMap& operator=(TitleMap&& _Other) noexcept = delete;

protected:

private:

};

