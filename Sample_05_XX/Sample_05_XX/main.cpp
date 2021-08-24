#include "stdafx.h"
#include "system/system.h"
#include "SrcFile/Game.h"
#include "SrcFile/TestGame.h"
#include "SrcFile/Fade.h"
#include "SrcFile/GameObj/Title.h"


///////////////////////////////////////////////////////////////////
// ウィンドウプログラムのメイン関数。
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
	//ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));
	//エンジンの初期化。
	EngineObj().Init(g_hWnd, FRAME_BUFFER_W, FRAME_BUFFER_H);
	const auto& renderContext = EngineObj().GetGraphicsEngine();

#ifdef PHYSICS_DEBUG
	//フィジックスのデバッグ。
	EngineObj().GetPhysicsWorld().SetPhysicsDebgug(btIDebugDraw::DBG_DrawWireframe);
#endif //PHYSICS_DEBUG

	//ゲームのインスタンス作成。
	NewGO<Title>(EnPriority_Game);

	// ここからゲームループ。
	while (DispatchWindowMessage())
	{
		//レンダリング開始。
		EngineObj().BeginFrame();
		EngineObj().Update();

		g_light.eyePos = GraphicsEngineObj()->GetCamera3D().GetPosition();
	}
	//エンジンの終了処理。
	EngineObj().Final();
	return 0;
}




