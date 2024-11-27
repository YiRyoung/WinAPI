#pragma once

class GameManager
{
public:
	// constrcuter destructer
	GameManager();
	~GameManager();

	// delete Function
	GameManager(const GameManager& _Other) = delete;
	GameManager(GameManager&& _Other) noexcept = delete;
	GameManager& operator=(const GameManager& _Other) = delete;
	GameManager& operator=(GameManager&& _Other) noexcept = delete;

protected:

private:

};

