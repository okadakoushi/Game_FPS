#include "stdafx.h"
#include "BulletManeger.h"
#include "SrcFile/GameObj/Bullet.h"

BulletManeger::BulletManeger()
{
}

BulletManeger::~BulletManeger()
{
}

bool BulletManeger::Start()
{
	//ガバっと確保。
	for (auto* bullet : m_bulletList) {

	}
	return true;
}

void BulletManeger::Update()
{
}

void BulletManeger::RegistBullet(int& index)
{
}
