#include "stdafx.h"
#include "TestGame.h"

bool TestGame::Start()
{
	//�Z�������O�̃��b�V���B
	char fBase[128] = "Assets/nvm/NavSampleBase.nvm";
	//�Z��������̃��b�V���B
	char f[128] = "Assets/nvm/NavSample.nvm";
	//�i�r���b�V�����[�h�B
	m_naviMeshBase.Load(fBase);
	m_naviMesh.Load(f);
	//�`��p�ɏ������B
	m_naviMesh.InitRender(false);
	m_naviMeshBase.InitRender(true);
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
	m_naviMesh.LineRender({ 0.0f, 0.0f, 1.0f, 1.0f });
	m_naviMeshBase.LineRender({1.0f, 0.0f, 0.0f, 1.0f});
}