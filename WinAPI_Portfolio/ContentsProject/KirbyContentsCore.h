#pragma once
#include <EngineCore/EngineAPICore.h>
#include <EnginePlatform/EngineWindow.h>

class KirbyContentsCore : public UContentsCore
{
public:
		KirbyContentsCore();
	~KirbyContentsCore();

		KirbyContentsCore(const KirbyContentsCore& _Other) = delete;
	KirbyContentsCore(KirbyContentsCore&& _Other) noexcept = delete;
	KirbyContentsCore& operator=(const KirbyContentsCore& _Other) = delete;
	KirbyContentsCore& operator=(KirbyContentsCore&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick() override;

private:
	ULevel* TitleLevel;
};

