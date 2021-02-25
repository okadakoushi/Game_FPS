#include "stdafx.h"
#include "TestGame.h"

bool TestGame::Start()
{
	NaviMeshObj()->InitRender();
	return true;
}

void TestGame::Update()
{
	auto qRot = g_quatIdentity;
	//ƒJƒƒ‰‚à‰ñ‚·B
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
}