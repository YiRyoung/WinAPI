#pragma once

class WindBullet 
{
public:
	// constrcuter destructer
	WindBullet();
	~WindBullet();

	// delete Function
	WindBullet(const WindBullet& _Other) = delete;
	WindBullet(WindBullet&& _Other) noexcept = delete;
	WindBullet& operator=(const WindBullet& _Other) = delete;
	WindBullet& operator=(WindBullet&& _Other) noexcept = delete;

protected:

private:

};

