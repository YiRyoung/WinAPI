#pragma once
#include <EngineCore/EngineAPICore.h>
#include <EnginePlatform/EngineWindow.h>

// Ό³Έν :
class EduContentsCore : public UContentsCore
{
public:
	// constrcuter destructer
	EduContentsCore();
	~EduContentsCore();

	// delete Function
	EduContentsCore(const EduContentsCore& _Other) = delete;
	EduContentsCore(EduContentsCore&& _Other) noexcept = delete;
	EduContentsCore& operator=(const EduContentsCore& _Other) = delete;
	EduContentsCore& operator=(EduContentsCore&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick() override;

private:
	ULevel* TitleLevel;
};

