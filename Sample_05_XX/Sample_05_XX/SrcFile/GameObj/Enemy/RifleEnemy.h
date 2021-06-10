#pragma once

#include "IEnemy.h"

class RifleEnemy : public Enemy
{
public:
	bool Start() override;
	void Update() override;
private:
	//��{�p�����B
	SkinModelRender* m_modelRender = nullptr;	//�����_�[�B
	Vector3 m_pos = g_vec3Zero;					//�ʒu�B
	Vector3 m_scale = {1.5f, 1.5f, 1.5f};		//�g��B
	Quaternion m_qRot = g_quatIdentity;			//��]�B
private:

};

