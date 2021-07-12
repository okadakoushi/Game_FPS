#pragma once

class Rifle;

#include "IEnemy.h"
//#include "SrcFile/GameObj/Rifle.h"

class RifleEnemy : public Enemy
{
public:
	bool Start() override;
	void Update() override;
	void Move();
private:
	Rifle* m_rifle = nullptr;
};

