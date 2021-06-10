#include "stdafx.h"
#include "RifleEnemy.h"

bool RifleEnemy::Start()
{
	const char* tkaFile[]{
		"Assets/animData/Enemy/Rifle/Walk.tka"
	};
	m_modelRender = NewGO<SkinModelRender>(EnPriority_3DModel);
	m_modelRender->Init("Assets/modelData/Chara/soldierMob.tkm", tkaFile);
	//エネミーは赤く～。
	m_modelRender->SetMulColor({ 1.0f, 0.5f, 0.5f, 1.0f });
	return true;
}

void RifleEnemy::Update()
{
	m_modelRender->SetPosition(m_pos);
	m_modelRender->SetScale(m_scale);
}
