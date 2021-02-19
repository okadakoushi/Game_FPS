#include "stdafx.h"
#include "TestGame.h"

bool TestGame::Start()
{
	//���b�V�����f�o�b�O�`��B
	char f[128] = "Assets/nvm/NavSample.nvm";
	m_naviMesh.Load(f, false);
	m_naviMesh.InitRender(false);
	m_astar.Search(m_start, m_end, m_naviMesh.GetCellList());
	return true;
}

void TestGame::Update()
{
	auto qRot = g_quatIdentity;
	//�J�������񂷁B
	qRot.SetRotationDegY(g_pad[0]->GetRStickXF());
	auto camPos = GraphicsEngineObj()->GetCamera3D().GetPosition();
	qRot.Apply(camPos);
	auto camTarget = GraphicsEngineObj()->GetCamera3D().GetTarget();

	camPos.z += g_pad[0]->GetLStickYF() * 5;
	if (GetAsyncKeyState(VK_SHIFT)) {
		camPos.y--;
	}
	if (GetAsyncKeyState(VK_SPACE)) {
		camPos.y++;
	}
	camTarget.y = camPos.y;
	GraphicsEngineObj()->GetCamera3D().SetPosition(camPos);
	GraphicsEngineObj()->GetCamera3D().SetTarget(camTarget);
	//�`��B
	m_naviMesh.Render({ 0.0f, 0.0f, 1.0f, 1.0f });
}