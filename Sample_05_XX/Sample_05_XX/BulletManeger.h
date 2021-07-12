#pragma once

class Bullet;

class BulletManeger : public IGameObject
{
public:
	BulletManeger();
	~BulletManeger();
	bool Start();
	void Update();

	void RegistBullet(int& index);
private:
	static const unsigned int MAX_BULLET = 128;
	Bullet* m_bulletList[MAX_BULLET];
};

