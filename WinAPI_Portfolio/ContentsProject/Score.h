#pragma once
#include <vector>
#include <EngineCore/Actor.h>

class AScore : public AActor
{
public:
	// constrcuter destructer
	AScore();
	~AScore();

	// delete Function
	AScore(const AScore& _Other) = delete;
	AScore(AScore&& _Other) noexcept = delete;
	AScore& operator=(const AScore& _Other) = delete;
	AScore& operator=(AScore&& _Other) noexcept = delete;

	void SetHPSpriteName(const std::string _Text);

	void SetTextScale(FVector2D _TextScale)
	{
		TextScale = _TextScale;
	}

	template<typename EnumType>
	void SetOrder(EnumType _Order)
	{
		SetOrder(static_cast<int>(_Order));
	}

	void SetOrder(int _Order);

	void SetValue(int _Score);


protected:

private:
	std::string SpriteName;
	FVector2D TextScale;
	std::vector<class USpriteRenderer*> Renders;
};

