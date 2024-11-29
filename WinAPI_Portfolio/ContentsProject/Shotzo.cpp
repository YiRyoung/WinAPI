#include "PreCompile.h"
#include "Shotzo.h"

#include <EngineCore/SpriteRenderer.h>

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
	// Left 
	//SpriteRenderer->SetSprite("Shotzo.png", 0);

	// LeftUpper
	//SpriteRenderer->SetSprite("Shotzo.png", 1);

	// Up
	//SpriteRenderer->SetSprite("Shotzo.png", 2);

	// RightUpper
	//SpriteRenderer->SetSprite("Shotzo.png", 3);

	// Right
	//SpriteRenderer->SetSprite("Shotzo.png", 4);

	// Bullet
	//SpriteRenderer->SetSprite("Shotzo.png", 5);
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
