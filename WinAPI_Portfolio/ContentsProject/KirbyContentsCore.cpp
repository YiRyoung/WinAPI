#include "PreCompile.h"
#include "KirbyContentsCore.h"
#include <EngineCore/EngineAPICore.h>

#include "PlayGameMode.h"
#include "Player.h"

KirbyContentsCore::KirbyContentsCore()
{
}

KirbyContentsCore::~KirbyContentsCore()
{
}

void KirbyContentsCore::BeginPlay()
{
	
	UEngineAPICore::GetCore()->CreateLevel<APlayGameMode, APlayer>("Play");

	UEngineAPICore::GetCore()->OpenLevel("Play");

}

void KirbyContentsCore::Tick()
{
}