#include "stdafx.h"
#include "TestGame.h"

bool TestGame::Start()
{
	//セル除去前のメッシュ。
	char fBase[128] = "Assets/nvm/NavSampleBase.nvm";
	//セル除去後のメッシュ。
	char f[128] = "Assets/nvm/NavSample.nvm";
	//ナビメッシュロード。
	m_naviMeshBase.Load(fBase);
	m_naviMesh.Load(f);
	//描画用に初期化。
	m_naviMesh.InitRender(false);
	m_naviMeshBase.InitRender(true);
	return true;
}

void TestGame::Update()
{
	auto qRot = g_quatIdentity;
	//カメラも回す。
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
	//描画。
	m_naviMesh.LineRender({ 0.0f, 0.0f, 1.0f, 1.0f });
	m_naviMeshBase.LineRender({1.0f, 0.0f, 0.0f, 1.0f});
}