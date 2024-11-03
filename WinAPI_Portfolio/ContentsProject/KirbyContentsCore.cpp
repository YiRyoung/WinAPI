#include "PreCompile.h"
#include "KirbyContentsCore.h"

#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineDebug.h>
#include <EngineBase/EngineFile.h>

#include <EngineCore/ImageManager.h>
#include <EngineCore/EngineAPICore.h>

#include "TitleGameMode.h"
#include "Stage1_1_GameMode.h"

KirbyContentsCore::KirbyContentsCore()
{
}

KirbyContentsCore::~KirbyContentsCore()
{
}

void KirbyContentsCore::BeginPlay()
{
	UEngineDirectory Dir;

	if (false == Dir.MoveParentToDirectory("Resources"))
	{
		MSGASSERT("Resources 폴더를 찾지 못했습니다.");
		return;
	}

	std::vector<UEngineFile> ImageFiles = Dir.GetAllFile();

	for (size_t i = 0; i < ImageFiles.size(); i++)
	{
		std::string FilePath = ImageFiles[i].GetPathToString();
		UImageManager::GetInst().Load(FilePath);
	}
	UImageManager::GetInst().CuttingSprite("Title_Kirby_64.png", { 64, 64 });
	UImageManager::GetInst().CuttingSprite("Kirby_Idle_Right_18.png", { 18, 18 });

	UEngineAPICore::GetCore()->GetMainWindow().SetWindowTitle("Kirby's Adventure");
	UEngineAPICore::GetCore()->GetMainWindow().SetWindowPosAndScale({ 0, 0 }, { 1024, 960 });
	UEngineAPICore::GetCore()->CreateLevel<ATitleGameMode, AActor>("Title");
	UEngineAPICore::GetCore()->CreateLevel<AStage1_1_GameMode, AActor>("Stage1_1");
	UEngineAPICore::GetCore()->OpenLevel("Title");
}

void KirbyContentsCore::Tick()
{
}

