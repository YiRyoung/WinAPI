#pragma once
#include <EngineCore/Actor.h>

class ATitleKirby : public AActor
{
public:
	// constrcuter destructer
	ATitleKirby();
	~ATitleKirby();

	// delete Function
	ATitleKirby(const ATitleKirby& _Other) = delete;
	ATitleKirby(ATitleKirby&& _Other) noexcept = delete;
	ATitleKirby& operator=(const ATitleKirby& _Other) = delete;
	ATitleKirby& operator=(ATitleKirby&& _Other) noexcept = delete;

protected:

private:

};

