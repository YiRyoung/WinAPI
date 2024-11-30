#pragma once

enum class ERenderOrder
{
	BACKGROUND = -1000,
	COLGROUND = -998,
	PLAYER = 0,
	MONSTER = 1,
	SKILL = 2,
	HUI = 10000,
	HUITEXT = 10001,
	FADE = 20000
};

enum class ECollisionGroup
{
	PLAYERBODY,
	PLAYERSKILL,
	INHALEBOX,
	MONSTERBODY,
	MONSTERSKILL,
	OBSTACLE,
	BOSS
};

enum class ECheckDir
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	MAX
};

enum class EAbilityType
{
	NORMAL,
	BEAM,
	CUTTER,
	FIRE,
	MAX
};

enum class EPlayerState
{
	IDLE,
	WALK,
	DASH,
	FLYSTART,
	FLY,
	FLYEND,
	JUMP,
	BEND,
	SLIDE,
	CLIMB,
	FALL,
	EAT,
	INHALESTART,
	INHALE,
	SPIT,
	SKILLSTART,
	SKILL,
	HURTSTART,
	HURT,
	DIE
};

enum class EMonsterState
{
	PAUSE,
	CHASE,
	ATTACKSTART,
	ATTACK,
	ATTACKEND,
	INHALE,
	DIE
};