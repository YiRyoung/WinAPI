#pragma once
#include <EnginePlatform/EngineWinImage.h>

class UEngineSprite : public UObject
{
public:
	class USpriteData
	{
	public:
		UEngineWinImage* Image;
		FTransform Transform;
	};

	// constrcuter destructer
	UEngineSprite();
	~UEngineSprite();

	// delete Function
	UEngineSprite(const UEngineSprite& _Other) = delete;
	UEngineSprite(UEngineSprite&& _Other) noexcept = delete;
	UEngineSprite& operator=(const UEngineSprite& _Other) = delete;
	UEngineSprite& operator=(UEngineSprite&& _Other) noexcept = delete;

	void PushData(UEngineWinImage* Image, const FTransform& _Trans);

	USpriteData GetSpriteData(int _index = 0)
	{
		return Data[_index];
	}

	void ClearSpriteData()
	{
		Data.clear();
	}

protected:
	std::vector<USpriteData> Data;
};

