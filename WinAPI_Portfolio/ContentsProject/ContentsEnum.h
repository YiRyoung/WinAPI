#pragma once


enum class ERenderOrder
{
	BACKGROUND = -1000,
	COLGROUND = -998,
	PLAYER = 0,
	MONSTER = 1,
	SKILL = 2,
	HUI = 10000,
	HUITEXT = 10001
};

enum class ECollisionGroup
{
	PLAYERBODY,
	PLAYERSKILL,
	INHALEBOX,
	MONSTERBODY,
	MONSTERSKILL
};

enum class EAblityType
{
	NORMAL,
	BEAM,
	FIRE,
	CUTTER
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
	FIRE,
	CUTTER
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
	SKILL,
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