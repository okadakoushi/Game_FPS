#include "stdafx.h"
#include "system/system.h"
#include "SrcFile/Game.h"

///////////////////////////////////////////////////////////////////
// ウィンドウプログラムのメイン関数。
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
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
		//////////////////////////////////////
		//ここから絵を描くコードを記述する。
		//////////////////////////////////////

		g_light.eyePos = GraphicsEngineObj()->GetCamera3D().GetPosition();

		//////////////////////////////////////
		//絵を描くコードを書くのはここまで！！！
		//////////////////////////////////////
		//レンダリング終了。
		EngineObj().EndFrame();
	}
	//エンジンの終了処理。
	EngineObj().Final();
	return 0;
}


