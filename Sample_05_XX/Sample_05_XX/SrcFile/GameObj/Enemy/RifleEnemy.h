#pragma once

#include "IEnemy.h"

class RifleEnemy : public Enemy
{
public:
	bool Start() override;
	void Update() override;
private:
	//基本パラム。
	SkinModelRender* m_modelRender = nullptr;	//レンダー。
	Vector3 m_pos = g_vec3Zero;					//位置。
	Vector3 m_scale = {1.5f, 1.5f, 1.5f};		//拡大。
	Quaternion m_qRot = g_quatIdentity;			//回転。
private:

};

