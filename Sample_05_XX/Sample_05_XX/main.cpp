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
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	//ゲームのインスタンス作成。
	NewGO<Game>(0);
	// ここからゲームループ。
	while (DispatchWindowMessage())
	{
		//レンダリング開始。
		EngineObj().BeginFrame();
		gameObjectManager().Start();
		//ゲームオブジェクトマネージャーの更新。
		gameObjectManager().UpdateManager();
		//////////////////////////////////////
		//ここから絵を描くコードを記述する。
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

		//カメラも回す。
		qRot.SetRotationDegY(g_pad[0]->GetLStickXF());
		auto camPos = g_camera3D->GetPosition();
		qRot.Apply(camPos);
		g_camera3D->SetPosition(camPos);
		g_light.eyePos = g_camera3D->GetPosition();
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


