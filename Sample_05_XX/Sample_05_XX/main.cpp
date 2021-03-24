#include "stdafx.h"
#include "system/system.h"
#include "SrcFile/Game.h"
#include "SrcFile/TestGame.h"


///////////////////////////////////////////////////////////////////
// ウィンドウプログラムのメイン関数。
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
#ifdef NAV_DEBUG
#endif // DEBUG
	AllocConsole();
	freopen("CON", "r", stdin);
	freopen("CON", "w", stdout);
	//ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));
	//エンジンの初期化。
	EngineObj().Init(g_hWnd, FRAME_BUFFER_W, FRAME_BUFFER_H);
	const auto& renderContext = EngineObj().GetGraphicsEngine();

	//ゲームのインスタンス作成。
	NewGO<Game>(EnPriority_Game);

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




