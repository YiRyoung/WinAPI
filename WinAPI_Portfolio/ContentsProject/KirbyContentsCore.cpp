#include "PreCompile.h"
#include "KirbyContentsCore.h"
#include <EngineCore/EngineAPICore.h>

#include <EngineBase/EngineDirectory.h>

#include <EngineBase/EngineDebug.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/ImageManager.h>

#include "TitleGameMode.h"
#include "Stage101GameMode.h"
#include "Stage102GameMode.h"
#include "Stage103GameMode.h"
#include "Stage104GameMode.h"
#include "BossStageGameMode.h"
#include "Player.h"


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
		MSGASSERT("리소스 폴더를 찾지 못했습니다.");
		return;
	}

	std::vector<UEngineFile> ImageFiles = Dir.GetAllFile();

	for (size_t i = 0; i < ImageFiles.size(); i++)
	{
		std::string FilePath = ImageFiles[i].GetPathToString();
		UImageManager::GetInst().Load(FilePath);
	}

	UImageManager::GetInst().CuttingSprite("Title_Kirby_64.png", {64, 64});
	UImageManager::GetInst().CuttingSprite("Kirby_Walk_Right_18.png", {18, 18});
	UImageManager::GetInst().CuttingSprite("Kirby_Idle_Right_18.png", {18, 18});

	UEngineAPICore::GetCore()->GetMainWindow().SetWindowTitle("Kirby's Adventure");

	// 이거 꼭 호출해줘야 합니다.
	UEngineAPICore::GetCore()->GetMainWindow().SetWindowPosAndScale({ 0, 0 }, {1040, 960});

	UEngineAPICore::GetCore()->CreateLevel<ATitleGameMode, AActor>("Title");
	UEngineAPICore::GetCore()->CreateLevel<AStage101GameMode, APlayer>("Stage101");
	UEngineAPICore::GetCore()->CreateLevel<AStage102GameMode, APlayer>("Stage102");
	UEngineAPICore::GetCore()->CreateLevel<AStage103GameMode, APlayer>("Stage103");
	UEngineAPICore::GetCore()->CreateLevel<AStage104GameMode, APlayer>("Stage104");
	UEngineAPICore::GetCore()->CreateLevel<ABossStageGameMode, APlayer>("BossStage");

	UEngineAPICore::GetCore()->OpenLevel("Title");

}

void KirbyContentsCore::Tick()
{
	// 지금 전혀 용도를 찾지 못해서 사용하지 않는 함수입니다.
}