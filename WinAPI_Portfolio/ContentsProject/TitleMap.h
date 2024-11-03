#pragma once
#include <EngineCore/Actor.h>

class ATitleMap : public AActor
{
public:
	// constrcuter destructer
	ATitleMap();
	~ATitleMap();

	// delete Function
	ATitleMap(const ATitleMap& _Other) = delete;
	ATitleMap(ATitleMap&& _Other) noexcept = delete;
	ATitleMap& operator=(const ATitleMap& _Other) = delete;
	ATitleMap& operator=(ATitleMap&& _Other) noexcept = delete;

protected:

private:

};

