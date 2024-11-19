#pragma once


enum class ERenderOrder
{
	BACKGROUND = -1000,
	COLGROUND = -998,
	PLAYER = 0,
	HUI = 10000,
	HUITEXT = 10001
};

enum class ECollisionGroup
{
	PlayerBody,
	PlayerFullBody,
	PlayerSkill,
	MonsterBody,
	MonsterSkill,
	StarBlock
};