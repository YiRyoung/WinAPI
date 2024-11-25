#include "PreCompile.h"
#include "KirbyContentsCore.h"
#include <EngineCore/EngineAPICore.h>

#include <EngineBase/EngineDirectory.h>

#include <EngineBase/EngineDebug.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/ImageManager.h>
#include <EnginePlatform/EngineSound.h>
#include <EnginePlatform/EngineWinImage.h>

#include "TitleGameMode.h"
#include "Stage101GameMode.h"
#include "MidBossGameMode.h"
#include "BossGameMode.h"
#include "Player.h"

KirbyContentsCore::KirbyContentsCore()
{
}

KirbyContentsCore::~KirbyContentsCore()
{
}

void KirbyContentsCore::BeginPlay()
{
	LoadImages();
	CuttingImages();
	LoadSounds();
	SetWorld();

	UEngineWinImage* BackBufferImage = UEngineAPICore::GetCore()->GetMainWindow().GetBackBuffer();
	HBRUSH OldBrush = static_cast<HBRUSH>(SelectObject(BackBufferImage->GetDC(), GetStockObject(NULL_BRUSH)));
	HPEN OldPen = static_cast<HPEN>(SelectObject(BackBufferImage->GetDC(), GetStockObject(WHITE_PEN)));
	DeleteObject(OldBrush);
	DeleteObject(OldPen);
}

void KirbyContentsCore::Tick()
{
}

void KirbyContentsCore::LoadImages()
{
	UEngineDirectory Dir;

	if (false == Dir.MoveParentToDirectory("Resources"))
	{
		MSGASSERT("리소스 폴더를 찾지 못했습니다.");
		return;
	}

	Dir.Append("Image");
	std::vector<UEngineFile> ImageFiles = Dir.GetAllFile();

	for (size_t i = 0; i < ImageFiles.size(); i++)
	{
		std::string FilePath = ImageFiles[i].GetPathToString();
		UImageManager::GetInst().Load(FilePath);
	}
}

void KirbyContentsCore::CuttingImages()
{	
	// Kirby
	UImageManager::GetInst().CuttingSprite("Kirby_Normal_Left.png", { 100, 100 });
	UImageManager::GetInst().CuttingSprite("Kirby_Normal_Right.png", { 100, 100 });

	// HUI
	UImageManager::GetInst().CuttingSprite("Lives.png", { 41, 39 });
	UImageManager::GetInst().CuttingSprite("Score.png", { 25, 26 });
	UImageManager::GetInst().CuttingSprite("Ability.png", { 100, 125 });
	UImageManager::GetInst().CuttingSprite("HP.png", { 25, 44 });

	// Normal Monster
		// Destory
	UImageManager::GetInst().CuttingSprite("Destory.png", {51, 51});
	UImageManager::GetInst().CuttingSprite("WaddleDee_Left.png", { 50, 50 });
	UImageManager::GetInst().CuttingSprite("WaddleDee_Right.png", { 50, 50 });
	UImageManager::GetInst().CuttingSprite("BroomHatter_Left.png", { 51, 50 });
	UImageManager::GetInst().CuttingSprite("BroomHatter_Right.png", { 51, 50 });

	// Ability Monster
	UImageManager::GetInst().CuttingSprite("WaddleDoo_Left.png", { 50, 51 });
	UImageManager::GetInst().CuttingSprite("WaddleDoo_Right.png", { 50, 51 });
	UImageManager::GetInst().CuttingSprite("HotHead_Left.png", { 50, 51 });
	UImageManager::GetInst().CuttingSprite("HotHead_Right.png", { 50, 51 });
}

void KirbyContentsCore::LoadSounds()
{
	UEngineDirectory Dir;
	if (false == Dir.MoveParentToDirectory("Resources"))
	{
		MSGASSERT("리소스 폴더를 찾지 못했습니다.");
		return;
	}
	Dir.Append("Sound");
	std::vector<UEngineFile> ImageFiles = Dir.GetAllFile();
	for (size_t i = 0; i < ImageFiles.size(); i++)
	{
		std::string FilePath = ImageFiles[i].GetPathToString();
		UEngineSound::Load(FilePath);
	}
}

void KirbyContentsCore::SetWorld()
{
	// Window
	UEngineAPICore::GetCore()->GetMainWindow().SetWindowTitle("Kirby's Adventure");
	UEngineAPICore::GetCore()->GetMainWindow().SetWindowPosAndScale({ 0, 0 }, { 768, 720 });

	// Levels
	UEngineAPICore::GetCore()->CreateLevel<ATitleGameMode, AActor>("Title");
	UEngineAPICore::GetCore()->CreateLevel<AStage101GameMode, APlayer>("Stage101");
	UEngineAPICore::GetCore()->CreateLevel<AMidBossGameMode, APlayer>("MidBoss");
	UEngineAPICore::GetCore()->CreateLevel<ABossGameMode, APlayer>("Boss");

	// StartLevel
	UEngineAPICore::GetCore()->OpenLevel("Stage101");
}
