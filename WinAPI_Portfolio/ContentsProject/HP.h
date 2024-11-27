#pragma once

// Ό³Έν :
class HP
{
public:
	// constrcuter destructer
	HP();
	~HP();

	// delete Function
	HP(const HP& _Other) = delete;
	HP(HP&& _Other) noexcept = delete;
	HP& operator=(const HP& _Other) = delete;
	HP& operator=(HP&& _Other) noexcept = delete;

protected:

private:

};

