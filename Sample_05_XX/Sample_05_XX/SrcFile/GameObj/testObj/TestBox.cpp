#include "stdafx.h"
#include "TestBox.h"

bool TestBox::Start()
{
	//レンダラー。
	m_render = NewGO<SkinModelRender>(EnPriority_3DModel);
	m_render->Init("Assets/modelData/levelTkm/oldCar.tkm");
	m_pos.x = 100.0f;	
	//m_pos.y = 50.0f;
	//コライダー、剛体。
	m_rot.SetRotationDegY(90.0f);
	m_render->SetRotation(m_rot);
	m_render->SetScale(m_scale);
	//m_psObj.CreateMesh(m_pos, m_rot, m_scale, m_render);
	m_psObj.CreateBox(m_pos, m_rot, m_scale);
	return true;
}

void TestBox::Update()
{
	m_render->SetPosition(m_pos);
}
