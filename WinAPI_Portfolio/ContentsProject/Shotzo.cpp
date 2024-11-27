#include "PreCompile.h"
#include "Shotzo.h"

AShotzo::AShotzo()
{
}

AShotzo::~AShotzo()
{
}

void AShotzo::BeginPlay()
{
	AMonster::BeginPlay();
}

void AShotzo::Tick(float _DeltaTime)
{
	AMonster::Tick(_DeltaTime);
}

void AShotzo::SetAnimation()
{

}

void AShotzo::Pause(float _DetlaTime)
{
}

void AShotzo::Chase(float _DetlaTime)
{
}

void AShotzo::AttackStart(float _DetlaTime)
{
}

void AShotzo::Attack(float _DetlaTime)
{
}

void AShotzo::AttackEnd(float _DetlaTime)
{
}
