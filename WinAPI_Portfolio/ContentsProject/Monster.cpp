#include "PreCompile.h"
#include "Monster.h"

#include <EngineCore/ImageManager.h>

AMonster::AMonster()
{
}

AMonster::~AMonster()
{
}

void AMonster::GetColImage(std::string_view _ColImageName)
{
	ColImage = UImageManager::GetInst().FindImage(_ColImageName);
}

void AMonster::SetMonster()
{
}

void AMonster::SetMonsterAnimation()
{
}

void AMonster::SetDestory()
{
}

void AMonster::BeginPlay()
{
	Super::BeginPlay();
}

void AMonster::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
}

void AMonster::Gravity(float _DeltaTime)
{
}

void AMonster::Move(FVector2D _NextPos)
{
}

void AMonster::Pause(float _DeltaTime)
{
}

void AMonster::Chase(float _DeltaTime)
{
}

void AMonster::Attack(float _DeltaTime)
{
}
