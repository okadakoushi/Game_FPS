#include "stdafx.h"
#include "system/system.h"
#include "SrcFile/Game.h"

///////////////////////////////////////////////////////////////////
// �E�B���h�E�v���O�����̃��C���֐��B
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//�Q�[���̏������B
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));
	//�G���W���̏������B
	EngineObj().Init(g_hWnd, FRAME_BUFFER_W, FRAME_BUFFER_H);
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	//�Q�[���̃C���X�^���X�쐬�B
	NewGO<Game>(0);
	// ��������Q�[�����[�v�B
	while (DispatchWindowMessage())
	{
		//�����_�����O�J�n�B
		EngineObj().BeginFrame();
		gameObjectManager().Start();
		//�Q�[���I�u�W�F�N�g�}�l�[�W���[�̍X�V�B
		gameObjectManager().UpdateManager();
		//////////////////////////////////////
		//��������G��`���R�[�h���L�q����B
		//////////////////////////////////////

		Quaternion qRot;
		if (g_pad[0]->IsPress(enButtonRight)) {
			qRot.SetRotationDegY(1.0f);
		}
		else if (g_pad[0]->IsPress(enButtonLeft)) {
			qRot.SetRotationDegY(-1.0f);
		}

		for (auto& lig : g_light.directionalLight) {
			qRot.Apply(lig.direction);
		}

		//�J�������񂷁B
		qRot.SetRotationDegY(g_pad[0]->GetLStickXF());
		auto camPos = g_camera3D->GetPosition();
		qRot.Apply(camPos);
		g_camera3D->SetPosition(camPos);
		g_light.eyePos = g_camera3D->GetPosition();
		//////////////////////////////////////
		//�G��`���R�[�h�������̂͂����܂ŁI�I�I
		//////////////////////////////////////
		//�����_�����O�I���B
		EngineObj().EndFrame();
	}
	//�G���W���̏I�������B
	EngineObj().Final();
	return 0;
}


