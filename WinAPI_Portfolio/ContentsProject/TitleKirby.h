#pragma once
#include <EngineCore/Actor.h>

class TitleKirby : public AActor
{
public:
	// constrcuter destructer
	TitleKirby();
	~TitleKirby();

	// delete Function
	TitleKirby(const TitleKirby& _Other) = delete;
	TitleKirby(TitleKirby&& _Other) noexcept = delete;
	TitleKirby& operator=(const TitleKirby& _Other) = delete;
	TitleKirby& operator=(TitleKirby&& _Other) noexcept = delete;

protected:

private:

};

