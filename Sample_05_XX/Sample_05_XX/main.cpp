#include "stdafx.h"
#include "system/system.h"
#include "SrcFile/Game.h"
#include "SrcFile/TestGame.h"
#include "SrcFile/Fade.h"
#include "SrcFile/GameObj/Title.h"


///////////////////////////////////////////////////////////////////
// �E�B���h�E�v���O�����̃��C���֐��B
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)	
{
#ifdef NAV_DEBUG
#endif // DEBUG
	ShowCursor(FALSE);
#ifdef MASTER
	//AllocConsole();
	//freopen("CON", "r", stdin);
	//freopen("CON", "w", stdout);
#endif
	//�Q�[���̏������B
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));
	//�G���W���̏������B
	EngineObj().Init(g_hWnd, FRAME_BUFFER_W, FRAME_BUFFER_H);
	const auto& renderContext = EngineObj().GetGraphicsEngine();

#ifdef PHYSICS_DEBUG
	//�t�B�W�b�N�X�̃f�o�b�O�B
	EngineObj().GetPhysicsWorld().SetPhysicsDebgug(btIDebugDraw::DBG_DrawWireframe);
#endif //PHYSICS_DEBUG

	//�Q�[���̃C���X�^���X�쐬�B
	NewGO<Title>(EnPriority_Game);

	// ��������Q�[�����[�v�B
	while (DispatchWindowMessage())
	{
		//�����_�����O�J�n�B
		EngineObj().BeginFrame();
		EngineObj().Update();

		g_light.eyePos = GraphicsEngineObj()->GetCamera3D().GetPosition();
	}
	//�G���W���̏I�������B
	EngineObj().Final();
	return 0;
}




